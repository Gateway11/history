#include <jni.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <list>
#include <atomic>

#include "json.h"
#include "VoiceService.h"

shared_ptr<VoiceService> voice_service = make_shared<VoiceService>();

JavaVM* _vm;
JNIEnv* _env;
jobject jobj;
jmethodID method_id;

std::atomic_flag thread_flag = ATOMIC_FLAG_INIT;

extern "C" {
    JNIEXPORT jboolean JNICALL
    Java_com_rokid_openvoice_VoiceManager_init(JNIEnv *env, jclass);
    JNIEXPORT void JNICALL
    Java_com_rokid_openvoice_VoiceManager_startSiren(JNIEnv *env, jclass, jboolean isopen);
    JNIEXPORT void JNICALL
    Java_com_rokid_openvoice_VoiceManager_setSirenState(JNIEnv *env, jclass, jint state);
    JNIEXPORT void JNICALL
    Java_com_rokid_openvoice_VoiceManager_networkStateChange(JNIEnv *env, jclass, jboolean isconnect);
    JNIEXPORT void JNICALL
    Java_com_rokid_openvoice_VoiceManager_updateStack(JNIEnv *env, jclass, jstring appid);
    JNIEXPORT void JNICALL
    Java_com_rokid_openvoice_VoiceManager_updateConfig(JNIEnv *env, jclass,
                                                       jstring device_id, jstring device_type_id, jstring key, jstring secret);
    JNIEXPORT void JNICALL
    Java_com_rokid_openvoice_VoiceManager_registCallback(JNIEnv *env, jclass, jobject obj);
    JNIEXPORT int JNICALL
    Java_com_rokid_openvoice_VoiceManager_addVtWord(JNIEnv *env, jclass, jobject vt_word);
    JNIEXPORT int JNICALL
    Java_com_rokid_openvoice_VoiceManager_removeVtWord(JNIEnv *env, jclass, jstring vt_word);
    JNIEXPORT jobject JNICALL
    Java_com_rokid_openvoice_VoiceManager_getVtWords(JNIEnv *env, jclass);
}

class Handle{
public:
    Handle(JNIEnv *env){
        jclass callback = env->FindClass("com/rokid/openvoice/VoiceCallback");
        if(callback == NULL){
            ALOGI("find class error");
            return;
        }
        callback_method_table[MSG_VOICE_EVENT_ID]
        = env->GetMethodID(callback, "onVoiceEvent", "(IIDD)V");
        callback_method_table[MSG_INTERMEDIATE_RESULT_ID]
        = env->GetMethodID(callback, "onIntermediateResult", "(IILjava/lang/String;)V");
        callback_method_table[MSG_VOICE_COMMAND_ID]
        = env->GetMethodID(callback, "onVoiceCommand", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
        callback_method_table[MSG_SPEECH_ERROR_ID]
        = env->GetMethodID(callback, "onSpeechError", "(II)V");
    }
    
    void start(jobject jobj){
        this->jobj = jobj;
        std::call_once(start_flag, [this]{m_thread = std::thread(&Handle::thread_loop, this);});
    }
    
    class Message{
    public:
        Message(){}
        Message(int32_t what, const string& data){
            this->what = what;
            this->data = data;
        }
        int32_t what;
        string data;
    };
    
    enum{
        MSG_VOICE_EVENT_ID = 0,
        MSG_INTERMEDIATE_RESULT_ID,
        MSG_VOICE_COMMAND_ID,
        MSG_SPEECH_ERROR_ID,
    };
    
    void send_message(const Message& msg){
        std::lock_guard<std::mutex> lock(m_mutex);
        message_queue.push_back(msg);
        m_cond.notify_one();
    }
    
private:
    void thread_loop(){
        _vm->AttachCurrentThread(&_env, NULL);
        std::unique_lock<std::mutex> lk(m_mutex, std::defer_lock);
        while(true){
            lk.lock();
            m_cond.wait(lk, [this]{return !message_queue.empty();});
            _env->PushLocalFrame(64);
            handle_message(message_queue.front());
            _env->PopLocalFrame(NULL);
            message_queue.pop_front();
            lk.unlock();
        }
        _vm->DetachCurrentThread();
    }
    
    void handle_message(Handle::Message& msg){
        json_object *json_obj = json_tokener_parse(msg.data.c_str());
        switch(msg.what){
            case MSG_VOICE_EVENT_ID:{
                json_object *id_obj, *event_obj, *sl_obj, *energy_obj;
                int32_t id, event;
                double sl, energy;
                if(TRUE == json_object_object_get_ex(json_obj, "id", &id_obj))
                    id = json_object_get_int(id_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "event", &event_obj))
                    event = json_object_get_int(event_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "sl", &sl_obj))
                    sl = json_object_get_double(sl_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "energy", &energy_obj))
                    energy = json_object_get_double(energy_obj);
                _env->CallVoidMethod(jobj, callback_method_table[msg.what], id, event, sl, energy);
            }
                break;
            case MSG_INTERMEDIATE_RESULT_ID:{
                jstring jasr = 0;
                json_object *id_obj, *type_obj, *asr_obj;
                int32_t id, type;
                std::string asr;
                if(TRUE == json_object_object_get_ex(json_obj, "id", &id_obj))
                    id = json_object_get_int(id_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "type", &type_obj))
                    type = json_object_get_int(type_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "asr", &asr_obj))
                    asr = json_object_get_string(asr_obj);
                if(asr.length() > 0) jasr = _env->NewStringUTF(asr.c_str());
                _env->CallVoidMethod(jobj, callback_method_table[msg.what], id, type, jasr);
            }
                break;
            case MSG_VOICE_COMMAND_ID:{
                jstring jasr = 0, jnlp = 0, jaction = 0;
                int32_t id;
                json_object *id_obj, *asr_obj, *nlp_obj, *action_obj;
                std::string asr, nlp, action;
                if(TRUE == json_object_object_get_ex(json_obj, "id", &id_obj))
                    id = json_object_get_int(id_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "asr", &asr_obj))
                    asr = json_object_get_string(asr_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "nlp", &nlp_obj))
                    nlp = json_object_get_string(nlp_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "action", &action_obj))
                    action = json_object_get_string(action_obj);
                if(asr.length() > 0) jasr = _env->NewStringUTF(asr.c_str());
                if(nlp.length() > 0) jnlp = _env->NewStringUTF(nlp.c_str());
                if(action.length() > 0) jaction = _env->NewStringUTF(action.c_str());
                _env->CallVoidMethod(jobj, callback_method_table[msg.what], id, jasr, jnlp, jaction);
            }
                break;
            case MSG_SPEECH_ERROR_ID:{
                json_object *id_obj, *errcode_obj;
                int32_t id, errcode;
                if(TRUE == json_object_object_get_ex(json_obj, "id", &id_obj))
                    id = json_object_get_int(id_obj);
                if(TRUE == json_object_object_get_ex(json_obj, "errcode", &errcode_obj))
                    errcode = json_object_get_int(errcode_obj);
                _env->CallVoidMethod(jobj, callback_method_table[msg.what], id, errcode);
            }
                break;
        }
        json_object_put(json_obj);
    }
    
    std::list<Message> message_queue;
    
    std::thread m_thread;
    std::condition_variable m_cond;
    std::mutex m_mutex;
    std::once_flag start_flag;
    
    jmethodID callback_method_table[4];
    JNIEnv *_env;
    jobject jobj;
};
shared_ptr<Handle> handle;
JNIEXPORT jboolean JNICALL 
Java_com_rokid_openvoice_VoiceManager_init(JNIEnv *env, jclass)
{
    ALOGD("%s", __FUNCTION__);
    jclass clazz = env->FindClass("com/rokid/openvoice/VoiceService");
    if(clazz == NULL) {
        ALOGE("find class VoiceService error");
        return -1;
    }
    method_id = env->GetMethodID(clazz, "getSkillOptions", "()Ljava/lang/String;");
    jobj = env->NewGlobalRef(env->AllocObject(clazz));
    
    handle = make_shared<Handle>(env);
    return voice_service->init([&]()->std::string{
        while(!thread_flag.test_and_set()) _vm->AttachCurrentThread(&_env, NULL);
        std::string skill_options;
        
        _env->PushLocalFrame(8);
        jstring jstr = (jstring)_env->CallObjectMethod(jobj, method_id);
        if(jstr != NULL){
            const char* str = _env->GetStringUTFChars(jstr, NULL);
            if (strlen(str) > 0) skill_options = str;
            _env->ReleaseStringUTFChars(jstr, str);
        }
        _env->PopLocalFrame(NULL);
        return skill_options;});
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_startSiren(JNIEnv *env, jclass, jboolean isopen)
{
    ALOGD("%s", __FUNCTION__);
    voice_service->start_siren((bool)isopen);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_setSirenState(JNIEnv *env, jclass, jint state)
{
    ALOGD("%s", __FUNCTION__);
    voice_service->set_siren_state((int)state);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_networkStateChange(JNIEnv *env, jclass, jboolean isconnect)
{
    ALOGD("%s", __FUNCTION__);
    voice_service->network_state_change((bool)isconnect);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_updateStack(JNIEnv *env, jclass, jstring appid)
{
    ALOGD("%s", __FUNCTION__);
    std::string _appid;
    if(appid != NULL){
        const char* str = env->GetStringUTFChars(appid, NULL);
        //jsize size = env->GetStringLength(vt_word);
        if (strlen(str) > 0) _appid = str;
        env->ReleaseStringUTFChars(appid, str);
    }
    voice_service->update_stack(_appid);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_updateConfig(JNIEnv *env, jclass, 
                                                   jstring device_id, jstring device_type_id, jstring key, jstring secret)
{
    ALOGD("%s", __FUNCTION__);
    std::string _device_id, _device_type_id, _key, _secret;
    
    if(device_id != NULL){
        const char* str = env->GetStringUTFChars(device_id, NULL);
        if (strlen(str) > 0) _device_id = str;
        env->ReleaseStringUTFChars(device_id, str);
    }
    if(device_type_id != NULL){
        const char* str = env->GetStringUTFChars(device_type_id, NULL);
        if (strlen(str) > 0) _device_type_id = str;
        env->ReleaseStringUTFChars(device_type_id, str);
    }
    if(key != NULL){
        const char* str = env->GetStringUTFChars(key, NULL);
        if (strlen(str) > 0) _key = str;
        env->ReleaseStringUTFChars(key, str);
    }
    if(secret != NULL){
        const char* str = env->GetStringUTFChars(secret, NULL);
        if (strlen(str) > 0) _secret = str;
        env->ReleaseStringUTFChars(secret, str);
    }
    voice_service->update_config(_device_id, _device_type_id, _key, _secret);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_registCallback(JNIEnv *env, jclass, jobject obj){
    ALOGD("%s", __FUNCTION__);
    if(handle.get()) handle->start(env->NewGlobalRef(obj));
    voice_service->regist_callback(
                        [&](int32_t method_id, const string& data){if(handle.get()) handle->send_message(Handle::Message(method_id, data));});
}

JNIEXPORT int JNICALL
Java_com_rokid_openvoice_VoiceManager_addVtWord(JNIEnv *env, jclass, jobject vt_word)
{
    ALOGD("%s", __FUNCTION__);
    siren_vt_word _vt_word;
    jclass clazz = env->GetObjectClass(vt_word);
    
    jfieldID field = env->GetFieldID(clazz, "vt_type", "I");
    _vt_word.vt_type = env->GetIntField(vt_word, field);
    
    field = env->GetFieldID(clazz, "vt_word", "Ljava/lang/String;");
    jstring jstr = (jstring)env->GetObjectField(vt_word, field);
    if(jstr != NULL){
        const char* str = env->GetStringUTFChars(jstr, NULL);
        if (strlen(str) > 0) _vt_word.vt_word = str;
        env->ReleaseStringUTFChars(jstr, str);
    }
                              
    field = env->GetFieldID(clazz, "vt_pinyin", "Ljava/lang/String;");
    jstr = (jstring)env->GetObjectField(vt_word, field);
    if(jstr != NULL){
        const char* str = env->GetStringUTFChars(jstr, NULL);
        if (strlen(str) > 0) _vt_word.vt_pinyin = str;
        env->ReleaseStringUTFChars(jstr, str);
    }
    _vt_word.use_default_config = true;
        
    return voice_service->add_vt_word(_vt_word);
}

JNIEXPORT int JNICALL
Java_com_rokid_openvoice_VoiceManager_removeVtWord(JNIEnv *env, jclass, jstring vt_word)
{
    ALOGD("%s", __FUNCTION__);
    std::string _vt_word;
    if(vt_word != NULL){
        const char* str = env->GetStringUTFChars(vt_word, NULL);
        if (strlen(str) > 0) _vt_word = str;
        env->ReleaseStringUTFChars(vt_word, str);
    }
    return voice_service->remove_vt_word(_vt_word);
}

JNIEXPORT jobject JNICALL
Java_com_rokid_openvoice_VoiceManager_getVtWords(JNIEnv *env, jclass)
{
    ALOGD("%s", __FUNCTION__);
    vector<siren_vt_word> _vt_words_in;
    int32_t count = voice_service->get_vt_word(_vt_words_in);
    
    jclass class_ArrayList = env->FindClass("java/util/ArrayList");
    jmethodID construct_array_list = env->GetMethodID(class_ArrayList, "<init>", "()V");
    jobject obj_ArrayList = env->NewObject(class_ArrayList,construct_array_list, "");
    jmethodID arrayList_add = env->GetMethodID(class_ArrayList, "add", "(Ljava/lang/Object;)Z");
    
    jclass class_vt_word = env->FindClass("com/rokid/openvoice/VoiceManager$VtWord");
    jmethodID construct_vt_word = env->GetMethodID(class_vt_word,"<init>", "()V");
    
    for(int i = 0; i < count; i++){
        //new a object
        //ALOGD("vt_type : %d, vt_word : %s, vt_pinyin : %s", _vt_words_in[i].vt_type, _vt_words_in[i].vt_word.c_str(), _vt_words_in[i].vt_pinyin.c_str());
        jobject obj_vt_word = env->NewObject(class_vt_word, construct_vt_word, "");
        
        jfieldID field_vt_type = env->GetFieldID(class_vt_word, "vt_type", "I");
        env->SetIntField(obj_vt_word, field_vt_type, _vt_words_in[i].vt_type);
        
        if(_vt_words_in[i].vt_word.length() > 0){
            jfieldID field_vt_word = env->GetFieldID(class_vt_word, "vt_word", "Ljava/lang/String;");
            env->SetObjectField(obj_vt_word, field_vt_word, env->NewStringUTF(_vt_words_in[i].vt_word.c_str()));
        }
        
        if(_vt_words_in[i].vt_pinyin.length() > 0){
            jfieldID field_vt_pinyin = env->GetFieldID(class_vt_word, "vt_pinyin", "Ljava/lang/String;");
            env->SetObjectField(obj_vt_word, field_vt_pinyin, env->NewStringUTF(_vt_words_in[i].vt_pinyin.c_str()));
        }
        
        env->CallBooleanMethod(obj_ArrayList, arrayList_add, obj_vt_word);
    }
    return obj_ArrayList;
}

static JNINativeMethod method_table[] = {
    { "init", "()Z", (void*)Java_com_rokid_openvoice_VoiceManager_init},
    { "startSiren", "(Z)V", (void*)Java_com_rokid_openvoice_VoiceManager_startSiren},
    { "setSirenState", "(I)V", (void*)Java_com_rokid_openvoice_VoiceManager_setSirenState},
    { "networkStateChange", "(Z)V", (void*)Java_com_rokid_openvoice_VoiceManager_networkStateChange},
    { "updateStack", "(Ljava/lang/String;)V", (void*)Java_com_rokid_openvoice_VoiceManager_updateStack},
    { "updateConfig", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void*)Java_com_rokid_openvoice_VoiceManager_updateConfig},
    { "registCallback", "(Lcom/rokid/openvoice/VoiceCallback;)V", (void*)Java_com_rokid_openvoice_VoiceManager_registCallback},
    
    { "addVtWord", "(Lcom/rokid/openvoice/VoiceManager$VtWord;)I", (void*)Java_com_rokid_openvoice_VoiceManager_addVtWord},
    { "removeVtWord", "(Ljava/lang/String;)I", (void*)Java_com_rokid_openvoice_VoiceManager_removeVtWord},
    { "getVtWords", "()Ljava/util/ArrayList;", (void*)Java_com_rokid_openvoice_VoiceManager_getVtWords},
};

int register_com_rokid_openvoice_VoiceManager(JNIEnv* env)
{
    const char* className = "com/rokid/openvoice/VoiceManager";
    jclass target = env->FindClass(className);
    if (target == NULL) {
        ALOGD("find class for %s failed", className);
        return -1;
    }
#define NELEM(x)            (sizeof(x)/sizeof(*(x)))
    if (env->RegisterNatives(target, method_table, NELEM(method_table)) < 0) {
        char* msg;
        asprintf(&msg, "RegisterNatives failed for '%s'; aborting...", className);
        env->FatalError(msg);
    }
    return 0;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env;
    // store a global java vm pointer
    // for voice_manager java callback
    _vm = vm;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGD("%s: JNI_OnLoad failed", "VoiceManager");
        return -1;
    }
    register_com_rokid_openvoice_VoiceManager(env);
    return JNI_VERSION_1_4;
}
