LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE 	:=wifi		 
LOCAL_SRC_FILES :=com_example_temp_WifiMonitor.cpp
include $(BUILD_SHARED_LIBRARY)
