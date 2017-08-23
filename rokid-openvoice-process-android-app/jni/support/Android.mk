SDK_BLACKSIREN := blacksiren/libbsiren/libs/armeabi-v7a
SDK_SPEECH	   := support

include $(CLEAR_VARS)
LOCAL_MODULE := bsiren
LOCAL_SRC_FILES := $(SDK_BLACKSIREN)/libbsiren.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libr2audio
LOCAL_SRC_FILES := $(SDK_BLACKSIREN)/libr2audio.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libr2ssp
LOCAL_SRC_FILES := $(SDK_BLACKSIREN)/libr2ssp.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libztvad
LOCAL_SRC_FILES := $(SDK_BLACKSIREN)/libztvad.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libr2vt
LOCAL_SRC_FILES := $(SDK_BLACKSIREN)/libr2vt.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := $(SDK_BLACKSIREN)/libcurl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpoco
LOCAL_SRC_FILES := $(SDK_SPEECH)/libpoco.so
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libprotobuf-rokid-cpp-full
LOCAL_SRC_FILES := $(SDK_SPEECH)/libprotobuf-rokid-cpp-full.so
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libspeech
LOCAL_SRC_FILES := $(SDK_SPEECH)/libspeech.so
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libtinyalsa
LOCAL_SRC_FILES := support/libtinyalsa.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := support/libssl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := support/libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libz
LOCAL_SRC_FILES := support/libz.so
include $(PREBUILT_SHARED_LIBRARY)
