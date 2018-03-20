MY_LOCAL_PATH := $(call my-dir)

include $(MY_LOCAL_PATH)/sources.mk

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto_static
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),arm64)
ifeq ($(USE_CLANG_PLATFORM_BUILD),true)
LOCAL_ASFLAGS += -march=armv8-a+crypto
endif
endif
LOCAL_CFLAGS := -I$(MY_LOCAL_PATH)/include -I$(MY_LOCAL_PATH)/src/crypto -Wno-unused-parameter
ifeq (,$(filter -DOPENSSL_NO_ASM,$(LOCAL_CFLAGS)))
LOCAL_SRC_FILES_x86 = $(linux_x86_sources)
LOCAL_SRC_FILES_x86_64 = $(linux_x86_64_sources)
LOCAL_SRC_FILES_arm = $(linux_arm_sources)
LOCAL_SRC_FILES_arm64 = $(linux_aarch64_sources)
endif
LOCAL_CLANG_ASFLAGS_arm += -no-integrated-as
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_PATH)/src/include
LOCAL_SRC_FILES := $(crypto_sources) $(LOCAL_SRC_FILES_$(TARGET_ARCH))
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl_static
LOCAL_SRC_FILES := $(ssl_sources)
LOCAL_C_INCLUDES := $(MY_LOCAL_PATH)/include
LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS = -Wno-unused-parameter
LOCAL_EXPORT_C_INCLUDE_DIRS := $(MY_LOCAL_PATH)/src/include
include $(BUILD_STATIC_LIBRARY)
