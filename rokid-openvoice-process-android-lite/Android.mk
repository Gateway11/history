LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_MODULE_TAGS := optional
LOCAL_CERTIFICATE := platform
LOCAL_PRIVILEGED_MODULE := true
LOCAL_PROGUARD_ENABLED := disabled
LOCAL_STATIC_JAVA_LIBRARIES := activation speech player
LOCAL_PACKAGE_NAME := RKVoiceActivationExample
include $(BUILD_PACKAGE)

############################################activation#########################################
include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := activation:libs/activation.jar
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := libc++_shared:libs/$(TARGET_CPU_ABI)/libc++_shared.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := libr2ssp:libs/$(TARGET_CPU_ABI)/libr2ssp.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := libr2vt:libs/$(TARGET_CPU_ABI)/libr2vt.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := librkvacti_jni:libs/$(TARGET_CPU_ABI)/librkvacti_jni.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := librkvacti:libs/$(TARGET_CPU_ABI)/librkvacti.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := libsourcelocation:libs/$(TARGET_CPU_ABI)/libsourcelocation.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := libztvad:libs/$(TARGET_CPU_ABI)/libztvad.so
include $(BUILD_MULTI_PREBUILT)
#############################################END###############################################

#############################################speech############################################
include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := libspeech:libs/$(TARGET_CPU_ABI)/libspeech.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := librkcodec:libs/$(TARGET_CPU_ABI)/librkcodec.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := librokid_speech_jni:libs/$(TARGET_CPU_ABI)/librokid_speech_jni.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := librokid_opus_jni:libs/$(TARGET_CPU_ABI)/librokid_opus_jni.so
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := speech:libs/speech.jar
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_PREBUILT_STATIC_JAVA_LIBRARIES := player:libs/player.jar
include $(BUILD_MULTI_PREBUILT)
#############################################END###############################################

