MY_LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libuWS

LOCAL_SRC_FILES := \
		$(MY_LOCAL_PATH)/src/Epoll.cpp \
		$(MY_LOCAL_PATH)/src/Extensions.cpp \
		$(MY_LOCAL_PATH)/src/Group.cpp \
		$(MY_LOCAL_PATH)/src/HTTPSocket.cpp \
		$(MY_LOCAL_PATH)/src/Hub.cpp \
		$(MY_LOCAL_PATH)/src/Networking.cpp \
		$(MY_LOCAL_PATH)/src/Node.cpp \
		$(MY_LOCAL_PATH)/src/Socket.cpp \
		$(MY_LOCAL_PATH)/src/WebSocket.cpp

LOCAL_C_INCLUDES := \
		$(MY_LOCAL_PATH)/src \
		$(LOCAL_PATH)/include/$(MY_TARGET_PLATFORM_LEVEL)

LOCAL_LDLIBS := -lz

LOCAL_CPPFLAGS := -std=c++11 -fexceptions

ifneq ($(PLATFORM_SDK_VERSION), 23)
LOCAL_CFLAGS += -D__STDC_FORMAT_MACROS
endif

LOCAL_SHARED_LIBRARIES := libssl libcrypto

LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/src

include $(BUILD_SHARED_LIBRARY)
