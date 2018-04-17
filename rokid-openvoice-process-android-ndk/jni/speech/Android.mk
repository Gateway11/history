MY_LOCAL_PATH := $(call my-dir)

IGNORED_WARNINGS := -Wno-sign-compare -Wno-unused-parameter -Wno-sign-promo -Wno-error=return-type -Wno-error=non-virtual-dtor
COMMON_CFLAGS := \
	$(IGNORED_WARNINGS) \
	-DSPEECH_LOG_ANDROID \
	-DSPEECH_SDK_STREAM_QUEUE_TRACE \
	-DSPEECH_SDK_DETAIL_TRACE

include $(CLEAR_VARS)

LOCAL_MODULE := libspeech
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_CPP_EXTENSION := .cc

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/boringssl/include \
	$(MY_LOCAL_PATH)/include \
	$(MY_LOCAL_PATH)/src/common \
	$(MY_LOCAL_PATH)/nanopb \
	$(MY_LOCAL_PATH)/nanopb-gen

COMMON_SRC := \
	$(MY_LOCAL_PATH)/src/common/rlog.c \
	$(MY_LOCAL_PATH)/src/common/speech_connection.cc \
	$(MY_LOCAL_PATH)/src/common/nanopb_encoder.cc \
	$(MY_LOCAL_PATH)/src/common/nanopb_decoder.cc

TTS_SRC := \
	$(MY_LOCAL_PATH)/src/tts/tts_impl.cc

SPEECH_SRC := \
	$(MY_LOCAL_PATH)/src/speech/speech_impl.cc

PB_SRC := \
	$(MY_LOCAL_PATH)/nanopb-gen/auth.pb.c \
	$(MY_LOCAL_PATH)/nanopb-gen/speech_types.pb.c \
	$(MY_LOCAL_PATH)/nanopb-gen/speech.pb.c \
	$(MY_LOCAL_PATH)/nanopb-gen/tts.pb.c \
	$(MY_LOCAL_PATH)/nanopb/pb_common.c \
	$(MY_LOCAL_PATH)/nanopb/pb_decode.c \
	$(MY_LOCAL_PATH)/nanopb/pb_encode.c

LOCAL_SRC_FILES := \
	$(COMMON_SRC) \
	$(TTS_SRC) \
	$(SPEECH_SRC) \
	$(PB_SRC)

LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_CFLAGS := $(COMMON_CFLAGS)
LOCAL_CPPFLAGS := -std=c++11
ifeq ($(ANDROID_VERSION), 19)
	LOCAL_CPPFLAGS += -D__STDC_FORMAT_MACROS
endif
LOCAL_LDLIBS += -llog -lz
LOCAL_SHARED_LIBRARIES := libuWS
LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/include
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := librkcodec
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := $(MY_LOCAL_PATH)/src/codec/rkcodec.cc
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/blacksiren/libbsiren/prebuilt/support/include/opus \
	$(MY_LOCAL_PATH)/include \
	$(MY_LOCAL_PATH)/src/common
LOCAL_SHARED_LIBRARIES := libspeech
LOCAL_CPPFLAGS := -std=c++11
LOCAL_LDLIBS := -lz -L$(LOCAL_PATH)/blacksiren/libbsiren/prebuilt/support/libs/android/$(TARGET_ARCH_ABI) -lopus
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := librokid_speech_jni
LOCAL_CPP_EXTENSION := .cc
LOCAL_C_INCLUDES := \
	$(MY_LOCAL_PATH)/include \
	$(MY_LOCAL_PATH)/src/common
LOCAL_CPPFLAGS := $(COMMON_FLAGS) -std=c++11
LOCAL_SHARED_LIBRARIES := libspeech
LOCAL_SRC_FILES := \
		$(MY_LOCAL_PATH)/android/jni/com_rokid_speech_Tts.cc \
		$(MY_LOCAL_PATH)/android/jni/com_rokid_speech_Speech.cc \
		$(MY_LOCAL_PATH)/android/jni/common.cc
LOCAL_LDLIBS := -llog -ldl
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := librokid_opus_jni
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(COMMON_FLAGS) -std=c++11
LOCAL_SHARED_LIBRARIES := libspeech librkcodec
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/blacksiren/libbsiren/prebuilt/support/include/opus \
	$(MY_LOCAL_PATH)/src/common
LOCAL_SRC_FILES := $(MY_LOCAL_PATH)/android/jni/com_rokid_speech_Opus.cc
LOCAL_LDLIBS := -llog -ldl
include $(BUILD_SHARED_LIBRARY)
