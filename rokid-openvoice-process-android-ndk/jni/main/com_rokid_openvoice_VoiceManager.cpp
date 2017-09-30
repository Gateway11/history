#include <jni.h>
#include "VoiceService.h"

shared_ptr<VoiceService> voice_service = make_shared<VoiceService>();

JavaVM* _vm;
static jmethodID callback_method_table[4];

enum{
        VOICE_EVENT_ID = 0,
        INTERMEDIATE_RESULT_ID,
        VOICE_COMMAND_ID,
        SPEECH_ERROR_ID,
};

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
jint JNI_OnLoad(JavaVM* vm, void* reserved);
}


JNIEXPORT jboolean JNICALL 
Java_com_rokid_openvoice_VoiceManager_init(JNIEnv *env, jclass)
{
	LOGD("%s", __FUNCTION__);
    jclass callback = env->FindClass("com/rokid/openvoice/VoiceCallback");
    if(callback == NULL){
        LOGI("find class error");
        return false;
    }
    callback_method_table[VOICE_EVENT_ID]
        = env->GetMethodID(callback, "onVoiceEvent", "(IIDD)V");
    callback_method_table[INTERMEDIATE_RESULT_ID]
        = env->GetMethodID(callback, "onIntermediateResult", "(IILjava/lang/String;)V");
    callback_method_table[VOICE_COMMAND_ID]
        = env->GetMethodID(callback, "onVoiceCommand", "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    callback_method_table[SPEECH_ERROR_ID]
        = env->GetMethodID(callback, "onSpeechError", "(II)V");
    return voice_service->init();
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_startSiren(JNIEnv *env, jclass, jboolean isopen)
{
	LOGD("%s", __FUNCTION__);
	voice_service->start_siren((bool)isopen);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_setSirenState(JNIEnv *env, jclass, jint state)
{
	LOGD("%s", __FUNCTION__);
	voice_service->set_siren_state((int)state);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_networkStateChange(JNIEnv *env, jclass, jboolean isconnect)
{
	LOGD("%s", __FUNCTION__);
	voice_service->network_state_change((bool)isconnect);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_updateStack(JNIEnv *env, jclass, jstring appid)
{
	LOGD("%s", __FUNCTION__);
    voice_service->update_stack(string(env->GetStringUTFChars(appid, NULL)));
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_updateConfig(JNIEnv *env, jclass, 
        jstring device_id, jstring device_type_id, jstring key, jstring secret)
{
	LOGD("%s", __FUNCTION__);
    voice_service->update_config(
                string(env->GetStringUTFChars(device_id, NULL)),
                string(env->GetStringUTFChars(device_type_id, NULL)),
                string(env->GetStringUTFChars(key, NULL)),
                string(env->GetStringUTFChars(secret, NULL)));
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_registCallback(JNIEnv *env, jclass, jobject obj){
	LOGD("%s", __FUNCTION__);
//    env->CallVoidMethod(obj, callback_method_table[VOICE_EVENT_ID], 1, 2, 3L, 4L);
    voice_service->regist_callback([&](int32_t method_id, va_list ap){
        if(obj){env->CallVoidMethod(obj, callback_method_table[method_id], ap);
    }});
}

static JNINativeMethod method_table[] = {
	{ "init", "()Z", (void*)Java_com_rokid_openvoice_VoiceManager_init},
	{ "startSiren", "(Z)V", (void*)Java_com_rokid_openvoice_VoiceManager_startSiren},
	{ "setSirenState", "(I)V", (void*)Java_com_rokid_openvoice_VoiceManager_setSirenState},
	{ "networkStateChange", "(Z)V", (void*)Java_com_rokid_openvoice_VoiceManager_networkStateChange},
	{ "updateStack", "(Ljava/lang/String;)V", (void*)Java_com_rokid_openvoice_VoiceManager_updateStack},
	{ "updateConfig", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", (void*)Java_com_rokid_openvoice_VoiceManager_updateConfig},
	{ "registCallback", "(Lcom/rokid/openvoice/VoiceCallback;)V", (void*)Java_com_rokid_openvoice_VoiceManager_registCallback},
};

int register_com_rokid_openvoice_VoiceManager(JNIEnv* env)
{
	const char* className = "com/rokid/openvoice/VoiceManager";
	jclass target = env->FindClass(className);
	if (target == NULL) {
		LOGD("find class for %s failed", className);
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
		LOGD("%s: JNI_OnLoad failed", "VoiceManager");
		return -1;
	}
    register_com_rokid_openvoice_VoiceManager(env);
	return JNI_VERSION_1_4;
}

