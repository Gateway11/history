LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)
LOCAL_MODULE := libmic_array
LOCAL_SRC_FILES := main/mic_array.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_SHARED_LIBRARIES := libtinyalsa
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libopenvoice
LOCAL_SRC_FILES := \
        main/siren_control.cpp \
        main/VoiceService.cpp \
        main/VoiceCallback.cpp
LOCAL_C_INCLUDES := \
        $(LOCAL_PATH)/include \
        $(LOCAL_PATH)/blacksiren/libbsiren/include \
        $(LOCAL_PATH)/blacksiren/libjsonc/include
LOCAL_SHARED_LIBRARIES := libmic_array libbsiren libspeech
LOCAL_LDLIBS := -llog #-lOpenSLES
LOCAL_CPPFLAGS += -std=c++11
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libopenvoice2
LOCAL_SRC_FILES := main/com_rokid_openvoice_VoiceManager.cpp
LOCAL_SHARED_LIBRARIES := libopenvoice libbsiren
LOCAL_LDLIBS := -llog
LOCAL_CPPFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)
