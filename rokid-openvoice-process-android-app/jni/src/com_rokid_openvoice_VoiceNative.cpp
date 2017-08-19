#include <jni.h>

#include "VoiceService.h"

shared_ptr<VoiceService> service = make_shared<VoiceService>();

JNIEXPORT jboolean JNICALL Java_com_rokid_openvoice_VoiceNavive_native_setup(JNIEnv *env, jobject thiz, jobject callback)
{
	LOGD("%s", __FUNCTION__);
    return service->setup();
	//return JNI_TRUE;
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_native_start_Siren(JNIEnv *env, jobject thiz, jint isopen)
{
	LOGD("%s", __FUNCTION__);
	service->start_siren((int)isopen);
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_native_set_siren_state(JNIEnv *env, jobject thiz, jint state)
{
	LOGD("%s", __FUNCTION__);
	service->set_siren_state((int)state);
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_native_network_state_hange(JNIEnv *env, jobject thiz, jboolean isconnect)
{
	LOGD("%s", __FUNCTION__);
	service->network_state_change((bool)isconnect);
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_native_update_tack(JNIEnv *env, jobject thiz, jstring appid)
{
	LOGD("%s", __FUNCTION__);
	//service->update_stack(s);
}
