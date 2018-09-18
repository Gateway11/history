NDK_TOOLCHAIN_VERSION := clang

APP_PLATFORM := android-23
#APP_ABI := arm64-v8a
#APP_ABI := armeabi-v7a arm64-v8a
APP_ABI := armeabi-v7a
#-Wno-error=unused-but-set-variable 
APP_CFLAGS += -Wno-error=format-security -Wno-error=sign-compare 
APP_STL := c++_shared

