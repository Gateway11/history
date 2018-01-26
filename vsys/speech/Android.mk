MY_LOCAL_PATH := $(call my-dir)

IGNORED_WARNINGS := -Wno-sign-compare -Wno-unused-parameter -Wno-sign-promo -Wno-error=return-type -Wno-error=non-virtual-dtor
COMMON_CFLAGS := \
	$(IGNORED_WARNINGS) \
	-DSPEECH_LOG_ANDROID \
	-DSPEECH_SDK_STREAM_QUEUE_TRACE \
	-DSPEECH_SDK_DETAIL_TRACE

include $(CLEAR_VARS)

LOCAL_MODULE := libspeech
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/include/$(ANDROID_VERSION) \
	$(MY_LOCAL_PATH)/proto \
	$(MY_LOCAL_PATH)/include \
	$(MY_LOCAL_PATH)/src/common

COMMON_SRC := \
	$(MY_LOCAL_PATH)/proto/speech_types.pb.cc \
	$(MY_LOCAL_PATH)/proto/auth.pb.cc \
	$(MY_LOCAL_PATH)/proto/tts.pb.cc \
	$(MY_LOCAL_PATH)/proto/speech.pb.cc \
	$(MY_LOCAL_PATH)/src/common/log.cc \
	$(MY_LOCAL_PATH)/src/common/log.h \
	$(MY_LOCAL_PATH)/src/common/speech_connection.cc \
	$(MY_LOCAL_PATH)/src/common/speech_connection.h

TTS_SRC := \
	$(MY_LOCAL_PATH)/src/tts/tts_impl.cc \
	$(MY_LOCAL_PATH)/src/tts/tts_impl.h \
	$(MY_LOCAL_PATH)/src/tts/types.h

SPEECH_SRC := \
	$(MY_LOCAL_PATH)/src/speech/speech_impl.cc \
	$(MY_LOCAL_PATH)/src/speech/speech_impl.h \
	$(MY_LOCAL_PATH)/src/speech/types.h

LOCAL_SRC_FILES := \
	$(COMMON_SRC) \
	$(TTS_SRC) \
	$(SPEECH_SRC)

LOCAL_CFLAGS := $(COMMON_CFLAGS) \
	-std=c++11 -frtti -fexceptions
LOCAL_LDLIBS += -llog
LOCAL_SHARED_LIBRARIES := libpoco libcrypto libprotobuf-rokid-cpp-full

LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/include

include $(BUILD_SHARED_LIBRARY)
