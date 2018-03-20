MY_LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libuWS
LOCAL_MODULE_TAGS := optional
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
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
	$(LOCAL_PATH)/boringssl/include
LOCAL_STATIC_LIBRARIES := libssl_static libcrypto_static
LOCAL_LDLIBS := -lz
LOCAL_CPPFLAGS := -std=c++11 -fexceptions -DUWS_THREADSAFE
LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/src
include $(BUILD_SHARED_LIBRARY)
