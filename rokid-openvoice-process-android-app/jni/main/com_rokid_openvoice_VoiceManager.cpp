#include <jni.h>
#include "VoiceService.h"

shared_ptr<VoiceService> service = make_shared<VoiceService>();

extern "C" {

JNIEXPORT jboolean JNICALL 
Java_com_rokid_openvoice_VoiceManager_init(JNIEnv *env, jclass)
{
	LOGD("%s", __FUNCTION__);
    return service->init();
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_startSiren(JNIEnv *env, jclass, jboolean isopen)
{
	LOGD("%s", __FUNCTION__);
	service->start_siren((bool)isopen);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_setSirenState(JNIEnv *env, jclass, jint state)
{
	LOGD("%s", __FUNCTION__);
	service->set_siren_state((int)state);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_networkStateChange(JNIEnv *env, jclass, jboolean isconnect)
{
	LOGD("%s", __FUNCTION__);
	service->network_state_change((bool)isconnect);
}

JNIEXPORT void JNICALL 
Java_com_rokid_openvoice_VoiceManager_updateStack(JNIEnv *env, jclass, jstring appid)
{
	LOGD("%s", __FUNCTION__);
    service->update_stack(string(env->GetStringUTFChars(appid, NULL)));
}

}
