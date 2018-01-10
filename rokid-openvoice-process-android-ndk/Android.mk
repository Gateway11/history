LOCAL_PATH:= $(call my-dir)

SDK_VERSION = $(shell if [ $(PLATFORM_SDK_VERSION) -ge 21 ]; then echo 23; else echo 19; fi)
SHARED_LIBRARIES_PATH := libs/$(SDK_VERSION)/$(TARGET_CPU_ABI)
ifndef ROKID_BLACKSIREN_CONFIG
ROKID_BLACKSIREN_CONFIG := default
endif

$(shell mkdir -p $(TARGET_OUT_ETC))
$(shell cp $(LOCAL_PATH)/assets/etc/openvoice_profile.json $(TARGET_OUT_ETC))
$(shell cp $(LOCAL_PATH)/assets/etc/blacksiren_$(ROKID_BLACKSIREN_CONFIG).json $(TARGET_OUT_ETC)/blacksiren.json)
$(shell cp -r $(LOCAL_PATH)/assets/workdir_cn $(TARGET_OUT)/workdir_cn)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
		$(call all-java-files-under, src) \
		src/com/rokid/tts/ITts.aidl \
		src/com/rokid/tts/ITtsCallback.aidl \
		src/com/rokid/voicerec/BearKid.aidl
LOCAL_STATIC_JAVA_LIBRARIES := opus_player rokid_speech
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_PACKAGE_NAME := openvoice_process
include $(BUILD_PACKAGE)

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := \
		opus_player:libs/opus_player.jar \
		rokid_speech:libs/rokid_speech.jar 
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := \
		libopenvoice2:$(SHARED_LIBRARIES_PATH)/libopenvoice2.so \
		libbsiren:$(SHARED_LIBRARIES_PATH)/libbsiren.so \
		libopenvoice:$(SHARED_LIBRARIES_PATH)/libopenvoice.so \
		libpoco:libs/$(PLATFORM_SDK_VERSION)/$(TARGET_CPU_ABI)/libpoco.so \
		libprotobuf-rokid-cpp-full:$(SHARED_LIBRARIES_PATH)/libprotobuf-rokid-cpp-full.so \
		libr2mvdrbf:$(SHARED_LIBRARIES_PATH)/libr2mvdrbf.so \
		libr2ssp:$(SHARED_LIBRARIES_PATH)/libr2ssp.so \
		libr2vt:$(SHARED_LIBRARIES_PATH)/libr2vt.so \
		libspeech:$(SHARED_LIBRARIES_PATH)/libspeech.so \
		libztvad:$(SHARED_LIBRARIES_PATH)/libztvad.so \
		librokid_speech_jni:libs/$(TARGET_CPU_ABI)/librokid_speech_jni.so \
		librokid_opus_jni:libs/$(TARGET_CPU_ABI)/librokid_opus_jni.so
ifndef ALTER_MIC_ARRAY
LOCAL_PREBUILT_LIBS += libmic_array:$(SHARED_LIBRARIES_PATH)/libmic_array.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
		$(call all-java-files-under, jar) \
		src/com/rokid/voicerec/ActionToComponent.java \
		src/com/rokid/voicerec/CustomWord.java \
		src/com/rokid/voicerec/BearKidResult.java \
		src/com/rokid/voicerec/BearKid.aidl
LOCAL_JACK_ENABLED = disabled
LOCAL_MODULE := BearKidAdapter
include $(BUILD_STATIC_JAVA_LIBRARY)
