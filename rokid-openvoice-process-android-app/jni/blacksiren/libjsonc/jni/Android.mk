LOCAL_PATH:= $(call my-dir)/..
include $(CLEAR_VARS)

LIBJSONC_DIR := $(LOCAL_PATH)

$(info LOCAL_PATH:$(LIBJSONC_DIR))
LOCAL_MODULE := libjsonc_static
LOCAL_SRC_FILES :=	\
	$(LIBJSONC_DIR)/src/arraylist.c \
	$(LIBJSONC_DIR)/src/debug.c \
	$(LIBJSONC_DIR)/src/json_c_version.c \
	$(LIBJSONC_DIR)/src/json_object.c \
	$(LIBJSONC_DIR)/src/json_object_iterator.c \
	$(LIBJSONC_DIR)/src/json_tokener.c \
	$(LIBJSONC_DIR)/src/json_util.c \
	$(LIBJSONC_DIR)/src/libjson.c \
	$(LIBJSONC_DIR)/src/linkhash.c \
	$(LIBJSONC_DIR)/src/printbuf.c \
	$(LIBJSONC_DIR)/src/random_seed.c

LOCAL_C_INCLUDES += $(LIBJSONC_DIR)/include/
LOCAL_CFLAGS := -Werror -Wall -Wno-error=deprecated-declarations -Wextra -Wwrite-strings -Wno-unused-parameter -std=gnu99 -D_GNU_SOURCE -D_REENTRANT
include $(BUILD_STATIC_LIBRARY)

