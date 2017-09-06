#git clone https://bitbucket.org/victorlazzarini/android-audiotest


LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)
LOCAL_MODULE := libopenvoice
LOCAL_SRC_FILES := \
		main/audio_recorder.cpp \
		main/mic_array.c \
        main/VoiceService.cpp \
        main/VoiceCallback.cpp
    
LOCAL_SHARED_LIBRARIES := \
        libbsiren \
		libtinyalsa \
        libspeech

LOCAL_C_INCLUDES := \
		$(LOCAL_PATH)/include \
		$(LOCAL_PATH)/blacksiren/libbsiren/include \
		$(LOCAL_PATH)/blacksiren/libjsonc/include


LOCAL_LDLIBS := -llog -lOpenSLES
LOCAL_CPPFLAGS += -std=c++11
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_SHARED_LIBRARY)

#===========================================
#
include $(CLEAR_VARS)
LOCAL_MODULE := libopenvoice2
LOCAL_SRC_FILES := main/com_rokid_openvoice_VoiceManager.cpp
        
LOCAL_LDLIBS := -llog
LOCAL_SHARED_LIBRARIES := libopenvoice
LOCAL_CFLAGS += -std=c++11
   
include $(BUILD_SHARED_LIBRARY)
