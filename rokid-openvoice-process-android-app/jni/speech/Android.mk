#LOCAL_PATH := $(call my-dir)
CURRENT_DIR := speech

PocoFoundationDir := poco/Foundation
PocoNetDir := poco/Net
PocoNetSSLDir := poco/NetSSL_OpenSSL
PocoCryptoDir := poco/Crypto
PocoUtilDir := poco/Util
PocoXMLDir := poco/XML
PocoJSONDir := poco/JSON

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
	$(LOCAL_PATH)/include \
	$(LOCAL_PATH)/protobuf/src \
	$(LOCAL_PATH)/$(PocoFoundationDir)/include \
	$(LOCAL_PATH)/$(PocoNetDir)/include \
	$(LOCAL_PATH)/$(PocoNetSSLDir)/include \
	$(LOCAL_PATH)/$(PocoCryptoDir)/include \
	$(LOCAL_PATH)/$(PocoUtilDir)/include \
	$(LOCAL_PATH)/$(PocoXMLDir)/include \
	$(LOCAL_PATH)/$(PocoJSONDir)/include \
	$(LOCAL_PATH)/$(CURRENT_DIR)/proto \
	$(LOCAL_PATH)/$(CURRENT_DIR)/include \
	$(LOCAL_PATH)/$(CURRENT_DIR)/src/common

COMMON_SRC := \
	$(CURRENT_DIR)/proto/speech.pb.cc \
	$(CURRENT_DIR)/src/common/speech_config.cc \
	$(CURRENT_DIR)/src/common/speech_config.h \
	$(CURRENT_DIR)/src/common/log.cc \
	$(CURRENT_DIR)/src/common/log.h \
	$(CURRENT_DIR)/src/common/speech_connection.cc \
	$(CURRENT_DIR)/src/common/speech_connection.h

TTS_SRC := \
	$(CURRENT_DIR)/src/tts/tts_impl.cc \
	$(CURRENT_DIR)/src/tts/tts_impl.h \
	$(CURRENT_DIR)/src/tts/types.h

ASR_SRC := \
	$(CURRENT_DIR)/src/asr/asr_impl.cc \
	$(CURRENT_DIR)/src/asr/asr_impl.h \
	$(CURRENT_DIR)/src/asr/types.h

SPEECH_SRC := \
	$(CURRENT_DIR)/src/speech/speech_impl.cc \
	$(CURRENT_DIR)/src/speech/speech_impl.h \
	$(CURRENT_DIR)/src/speech/types.h

LOCAL_SRC_FILES := \
	$(COMMON_SRC) \
	$(TTS_SRC) \
	$(ASR_SRC) \
	$(SPEECH_SRC)

LOCAL_CFLAGS := $(COMMON_CFLAGS) \
	-std=c++11 -frtti -fexceptions
LOCAL_LDLIBS += -llog
LOCAL_SHARED_LIBRARIES := libpoco libcrypto libprotobuf-rokid-cpp-full

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := roots.pem
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT)/etc
LOCAL_SRC_FILES := etc/$(LOCAL_MODULE)
#include $(BUILD_PREBUILT)
