include $(CLEAR_VARS)
LOCAL_MODULE := libbsiren
LOCAL_SRC_FILES := support/libbsiren.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libr2audio
LOCAL_SRC_FILES := support/libr2audio.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libr2ssp
LOCAL_SRC_FILES := support/libr2ssp.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libztvad
LOCAL_SRC_FILES := support/libztvad.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libr2vt
LOCAL_SRC_FILES := support/libr2vt.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpoco
LOCAL_SRC_FILES := support/libpoco.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libprotobuf-rokid-cpp-full
LOCAL_SRC_FILES := support/libprotobuf-rokid-cpp-full.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libspeech
LOCAL_SRC_FILES := support/libspeech.so
include $(PREBUILT_SHARED_LIBRARY)