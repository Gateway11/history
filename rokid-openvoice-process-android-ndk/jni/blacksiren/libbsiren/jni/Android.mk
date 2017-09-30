LOCAL_PATH:= $(call my-dir)

define all-named-files-under
$(patsubst ./%,%, \
  $(shell cd $(LOCAL_PATH) ; \
          find -L $(2) -name "$(1)" -and -not -name ".*") \
 )
endef

include $(CLEAR_VARS)
LOCAL_MODULE := r2ssp
LOCAL_SRC_FILES := ../prebuilt/support/libs/android/armv7eabi/libr2ssp.so 
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ztvad
LOCAL_SRC_FILES := ../prebuilt/support/libs/android/armv7eabi/libztvad.so 
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := r2vt
LOCAL_SRC_FILES := ../prebuilt/support/libs/android/armv7eabi/legacy/libr2vt.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := r2mvdrbf
LOCAL_SRC_FILES := ../prebuilt/support/libs/android/armv7eabi/libr2mvdrbf.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := opus
LOCAL_SRC_FILES := ../prebuilt/support/libs/android/armv7eabi/libopus.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := jsonc_static
LOCAL_SRC_FILES := ../../libjsonc/obj/local/armeabi-v7a/libjsonc_static.a
include $(PREBUILT_STATIC_LIBRARY)

include $(LOCAL_PATH)/android.config
L_CFLAGS := -DANDROID -DANDROID_LOG_TAG=\"BlackSiren\"

ifdef CONFIG_ANDROID_LOG
$(info CONFIG_ANDROID_LOG)
L_CFLAGS += -DCONFIG_ANDROID_LOG
endif

ifdef CONFIG_LEGACY_SIREN_TEST
$(info CONFIG_LEGACY_SIREN_TEST)
L_CFLAGS += -DCONFIG_LEGACY_SIREN_TEST
endif

ifdef CONFIG_USE_FIFO
$(info CONFIG_USE_FIFO)
L_CFLAGS += -DCONFIG_USE_FIFO
endif

ifdef CONFIG_DEBUG_CHANNEL
$(info CONFIG_DEBUG_CHANNEL)
L_CFLAGS += -DCONFIG_DEBUG_CHANNEL
endif

ifdef CONFIG_BACKUP_FILE_PATH
$(info CONFIG_BACKUP_FILE_PATH)
L_CFLAGS += -DCONFIG_BACKUP_FILE_PATH=\"$(CONFIG_BACKUP_FILE_PATH)\"
endif

ifdef CONFIG_REMOTE_CONFIG_FILE_URL
$(info CONFIG_REMOTE_CONFIG_FILE_URL)
L_CFLAGS += -DCONFIG_REMOTE_CONFIG_FILE_URL=\"$(CONFIG_REMOTE_CONFIG_FILE_URL)\"
endif

ifdef CONFIG_STORE_FILE_PATH
$(info CONFIG_STORE_FILE_PATH)
L_CFLAGS += -DCONFIG_STORE_FILE_PATH=\"$(CONFIG_STORE_FILE_PATH)\"
endif

ifdef CONFIG_REMOTE_CONFIG_HOSTNAME
$(info CONFIG_REMOTE_CONFIG_HOSTNAME)
L_CFLAGS += -DCONFIG_REMOTE_CONFIG_HOSTNAME=\"$(CONFIG_REMOTE_CONFIG_HOSTNAME)\"
endif

ifdef CONFIG_USE_AD1
$(info CONFIG_USE_AD1)
L_CFLAGS += -DCONFIG_USE_AD1
endif

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= $(call all-named-files-under,*.cpp, ../src) 
LOCAL_C_INCLUDES += \
		$(THIRD_INCLUDES) \
		$(LOCAL_PATH)/../include \
		$(LOCAL_PATH)/../prebuilt/libcurl/include \
		$(LOCAL_PATH)/../prebuilt/support/include \
		$(LOCAL_PATH)/../../libjsonc/include

LOCAL_CFLAGS:= $(L_CFLAGS) -Wall -Wextra -std=c++11 -DANDROID_VERSION=$(ANDROID_VERSION)
LOCAL_MODULE:= libbsiren
LOCAL_LDLIBS:= -L$(SYSROOT)/usr/lib -llog
LOCAL_SHARED_LIBRARIES := libr2ssp libztvad libr2vt 
LOCAL_STATIC_LIBRARIES += libjsonc_static libopus

include $(BUILD_SHARED_LIBRARY)
