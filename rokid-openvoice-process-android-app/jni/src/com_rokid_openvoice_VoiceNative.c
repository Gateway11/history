#include <jni.h>
#include "VoiceService.h"

static unique_ptr<VoiceService> service(make_unique<VoiceService>());

JNIEXPORT jboolean JNICALL Java_com_rokid_openvoice_VoiceNavive_init(JNIEnv *env, jobject thiz)
{
	LOGD("%s", __FUNCTION__);
    return service->init();
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_startSiren(JNIEnv *env, jobject thiz, jint isopen)
{
	LOGD("%s", __FUNCTION__);
	service->start_siren((int)isopen);
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_setSirenState(JNIEnv *env, jobject thiz, jint state)
{
	LOGD("%s", __FUNCTION__);
	service->set_siren_state((int)state);
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_networkStateChange(JNIEnv *env, jobject thiz, jboolean isconnect)
{
	LOGD("%s", __FUNCTION__);
	service->network_state_change((bool)isconnect);
}

JNIEXPORT void JNICALL Java_com_rokid_openvoice_VoiceNavive_updateStack(JNIEnv *env, jobject thiz, jstring appid)
{
	LOGD("%s", __FUNCTION__);
	//service->update_stack(s);
}
