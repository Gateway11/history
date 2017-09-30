NDK_TOOLCHAIN_VERSION := clang

APP_PLATFORM := android-19
#APP_ABI := arm64-v8a
#APP_ABI := armeabi-v7a arm64-v8a
APP_ABI := armeabi-v7a
#-Wno-error=unused-but-set-variable 
APP_CFLAGS += -Wno-error=format-security -Wno-error=sign-compare 

ANDROID_VERSION = $(shell if [ $(subst android-,,$(APP_PLATFORM)) -ge 21 ]; then echo 21; else echo 19; fi)

ifeq ($(ANDROID_VERSION), 19)
APP_STL := gnustl_static
else
APP_STL := c++_static
endif

