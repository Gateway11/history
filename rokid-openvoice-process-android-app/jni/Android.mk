LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libopenvoice_process

LOCAL_SRC_FILES := \
		src/com_rokid_openvoice_VoiceNative.c \
		src/opensl_io.c
		
LOCAL_LDLIBS := -llog -lOpenSLES

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

include $(BUILD_SHARED_LIBRARY)