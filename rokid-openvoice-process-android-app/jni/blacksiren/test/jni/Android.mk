LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	test.cpp

LOCAL_C_INCLUDES += \
		$(LOCAL_PATH)/include \
		$(LOCAL_PATH)/../../hardware/include \
		external/curl/include

LOCAL_MODULE := test
LOCAL_SHARED_LIBRARIES := libbsiren LOCAL_STATIC_LIBRARIES := libjsonc_static

include $(BUILD_EXECUTABLE)
include $(wildcard $(LOCAL_PATH)/java/jni/Android.mk)
