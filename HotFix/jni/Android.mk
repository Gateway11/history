LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := patch
LOCAL_SRC_FILES := \
	blocksort.c \
	bzip2.c \
	bzip2recover.c \
	huffman.c \
	mk251.c \
	crctable.c \
	spewG.c \
	unzcrash.c \
	randtable.c \
	compress.c \
	decompress.c \
	dlltest.c \
	bzlib.c
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := bspatch
LOCAL_SRC_FILES := bspatch.c
LOCAL_C_INCLUDES := bspatch.h
LOCAL_STATIC_LIBRARIES := patch 
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := patcher_tools
LOCAL_SRC_FILES := patcher.cpp 
LOCAL_STATIC_LIBRARIES := bspatch
LOCAL_LDLIBS :=  -llog
include $(BUILD_SHARED_LIBRARY)
