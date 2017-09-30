LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES :=	\
	$(LOCAL_PATH)/../src/arraylist.c \
	$(LOCAL_PATH)/../src/debug.c \
	$(LOCAL_PATH)/../src/json_c_version.c \
	$(LOCAL_PATH)/../src/json_object.c \
	$(LOCAL_PATH)/../src/json_object_iterator.c \
	$(LOCAL_PATH)/../src/json_tokener.c \
	$(LOCAL_PATH)/../src/json_util.c \
	$(LOCAL_PATH)/../src/libjson.c \
	$(LOCAL_PATH)/../src/linkhash.c \
	$(LOCAL_PATH)/../src/printbuf.c \
	$(LOCAL_PATH)/../src/random_seed.c

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include
LOCAL_CFLAGS := -Werror -Wall -Wno-error=deprecated-declarations -Wextra -Wwrite-strings -Wno-unused-parameter -std=gnu99 -D_GNU_SOURCE -D_REENTRANT
LOCAL_MODULE := libjsonc_static
include $(BUILD_STATIC_LIBRARY)

