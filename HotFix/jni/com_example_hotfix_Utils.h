#include <jni.h>

#ifndef _Included_com_example_hotfix_Utils
#define _Included_com_example_hotfix_Utils
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_example_hotfix_Utils_patch(JNIEnv *, jobject, jstring, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif
