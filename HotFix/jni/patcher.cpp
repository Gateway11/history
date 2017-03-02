#include <jni.h>
#include "com_example_hotfix_Utils.h"
#include "bspatch.h"
#include <stdlib.h>
#include <android/log.h>
#include<string.h>


char* Jstring2CStr(JNIEnv* env, jstring jstr) {

	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("GB2312");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes","(Ljava/lang/String;)[B");

	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);

	jsize alen = env->GetArrayLength(barr);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}
;

JNIEXPORT void JNICALL Java_com_example_test_MainActivity_patch(JNIEnv* env,
		jobject othis, jstring old_path, jstring new_path, jstring patch_path) {
	char ** argv;
	int loopVar, result;

	__android_log_print(ANDROID_LOG_INFO, "bspatch.c", "正在打包。。。");
	argv = (char**) malloc(4 * sizeof(char*));
	for (loopVar = 0; loopVar < 4; loopVar++) {
		argv[loopVar] = (char*) malloc(100 * sizeof(char));
	}

	argv[0] = "bspatch";
	argv[1] = Jstring2CStr(env, old_path);
	argv[2] = Jstring2CStr(env, new_path);
	argv[3] = Jstring2CStr(env, patch_path);

	result = bspatch(argv);

	env->ReleaseStringUTFChars(old_path, argv[1]);
	env->ReleaseStringUTFChars(new_path, argv[2]);
	env->ReleaseStringUTFChars(patch_path, argv[3]);
}

