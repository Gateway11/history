LOCAL_PATH:= $(call my-dir)

$(shell cp $(LOCAL_PATH)/etc/openvoice_profile.json $(TARGET_OUT_ETC))
$(shell cp $(LOCAL_PATH)/etc/blacksiren.json $(TARGET_OUT_ETC))
$(shell cp -r $(LOCAL_PATH)/workdir_cn $(TARGET_OUT)/workdir_cn)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_PACKAGE_NAME := openvoice_process
include $(BUILD_PACKAGE)
##########################################################################
#
include $(CLEAR_VARS)

LOCAL_PREBUILT_LIBS := \
		libopenvoice2:libs/armeabi-v7a/libopenvoice2.so \
		libbsiren:libs/armeabi-v7a/libbsiren.so \
		libopenvoice:libs/armeabi-v7a/libopenvoice.so \
		libpoco:libs/armeabi-v7a/libpoco.so \
		libprotobuf-rokid-cpp-full:libs/armeabi-v7a/libprotobuf-rokid-cpp-full.so \
		libr2mvdrbf:libs/armeabi-v7a/libr2mvdrbf.so \
		libr2ssp:libs/armeabi-v7a/libr2ssp.so \
		libr2vt:libs/armeabi-v7a/libr2vt.so \
		libspeech:libs/armeabi-v7a/libspeech.so \
		libztvad:libs/armeabi-v7a/libztvad.so \
#		libcrypto:libs/armeabi-v7a/libcrypto.so \
		libssl:libs/armeabi-v7a/libssl.so \
		libtinyalsa:libs/armeabi-v7a/libtinyalsa.so \
		libz:libs/armeabi-v7a/libz.so \

include $(BUILD_MULTI_PREBUILT)
