LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE	:= libopenvoice
LOCAL_SRC_FILES := ../../../../libs/armeabi-v7a/libopenvoice.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libbsiren
LOCAL_SRC_FILES := ../../libbsiren/libs/armeabi-v7a/libbsiren.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := ../test.cpp

LOCAL_C_INCLUDES += \
		../../include \
		../../libbsiren/prebuilt/libcurl/include \
		../../libbsiren/include \
		../../../include

LOCAL_MODULE := test
LOCAL_SHARED_LIBRARIES := libbsiren libopenvoice 
LOCAL_CFLAGS:= -Wall -Wextra -std=c++11

include $(BUILD_EXECUTABLE)
#include $(wildcard $(LOCAL_PATH)/java/jni/Android.mk)
