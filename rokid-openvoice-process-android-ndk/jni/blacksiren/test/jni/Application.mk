NDK_TOOLCHAIN_VERSION := clang

#APP_STL := stlport_static
APP_STL := gnustl_static
#APP_STL := c++_static

#APP_ABI := armeabi-v7a arm64-v8a
#APP_ABI := arm64-v8a
APP_ABI := armeabi-v7a

#APP_OPTIM := debug
APP_PLATFORM := android-19

#-Wno-error=unused-but-set-variable 
APP_CFLAGS += -Wno-error=format-security -Wno-error=sign-compare 
