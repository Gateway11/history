APP_ABI := armeabi-v7a
#APP_ABI := armeabi-v7a arm64-v8a
APP_PLATFORM := android-23

ANDROID_VERSION = $(shell if [ $(subst android-,,$(APP_PLATFORM)) -ge 21 ]; then echo 21; else echo 19; fi)
MY_TARGET_PLATFORM_LEVEL = $(shell if [ $(TARGET_PLATFORM_LEVEL) -ge 23 ]; then echo 21; else echo 19; fi)

ifeq ($(ANDROID_VERSION), 19)
APP_STL := gnustl_static
else
APP_STL := c++_static
endif
