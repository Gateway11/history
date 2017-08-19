LOCAL_PATH := $(call my-dir)
include $(call all-subdir-makefiles)

include $(CLEAR_VARS)
LOCAL_MODULE := libopenvoice
LOCAL_SRC_FILES := \
		src/VoiceService.cpp \
		src/VoiceCallback.cpp
		
LOCAL_SHARED_LIBRARIES := \
		libbsiren \
		libspeech

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)

#===========================================
#
include $(CLEAR_VARS)
LOCAL_MODULE := libopenvoicedd
LOCAL_SRC_FILES += \
		src/com_rokid_openvoice_VoiceNative.cpp \
		src/opensl_io.c
		
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS := -llog -lOpenSLES
LOCAL_CFLAGS += -std=c++11
LOCAL_SHARED_LIBRARIES := libopenvoice
		
include $(BUILD_SHARED_LIBRARY)

