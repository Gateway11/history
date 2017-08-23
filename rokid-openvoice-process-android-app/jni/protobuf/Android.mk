# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#

#LOCAL_PATH := $(call my-dir)
CURRENT_DIR := protobuf

IGNORED_WARNINGS := -Wno-sign-compare -Wno-unused-parameter -Wno-sign-promo -Wno-error=return-type

CC_LITE_SRC_FILES := \
   	$(CURRENT_DIR)/src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc         \
    $(CURRENT_DIR)/src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc        \
    $(CURRENT_DIR)/src/google/protobuf/stubs/common.cc                              \
    $(CURRENT_DIR)/src/google/protobuf/stubs/once.cc                                \
    $(CURRENT_DIR)/src/google/protobuf/stubs/hash.h                                 \
    $(CURRENT_DIR)/src/google/protobuf/stubs/map_util.h                             \
    $(CURRENT_DIR)/src/google/protobuf/stubs/shared_ptr.h                           \
    $(CURRENT_DIR)/src/google/protobuf/stubs/stringprintf.cc                        \
    $(CURRENT_DIR)/src/google/protobuf/stubs/stringprintf.h                         \
    $(CURRENT_DIR)/src/google/protobuf/extension_set.cc                             \
    $(CURRENT_DIR)/src/google/protobuf/generated_message_util.cc                    \
    $(CURRENT_DIR)/src/google/protobuf/message_lite.cc                              \
    $(CURRENT_DIR)/src/google/protobuf/repeated_field.cc                            \
    $(CURRENT_DIR)/src/google/protobuf/wire_format_lite.cc                          \
    $(CURRENT_DIR)/src/google/protobuf/io/coded_stream.cc                           \
    $(CURRENT_DIR)/src/google/protobuf/io/coded_stream_inl.h                        \
    $(CURRENT_DIR)/src/google/protobuf/io/zero_copy_stream.cc                       \
    $(CURRENT_DIR)/src/google/protobuf/io/zero_copy_stream_impl_lite.cc

# C++ full library
# =======================================================
protobuf_cc_full_src_files := \
    $(CC_LITE_SRC_FILES)                                             \
    $(CURRENT_DIR)/src/google/protobuf/stubs/strutil.cc                             \
    $(CURRENT_DIR)/src/google/protobuf/stubs/strutil.h                              \
    $(CURRENT_DIR)/src/google/protobuf/stubs/substitute.cc                          \
    $(CURRENT_DIR)/src/google/protobuf/stubs/substitute.h                           \
    $(CURRENT_DIR)/src/google/protobuf/stubs/structurally_valid.cc                  \
    $(CURRENT_DIR)/src/google/protobuf/descriptor.cc                                \
    $(CURRENT_DIR)/src/google/protobuf/descriptor.pb.cc                             \
    $(CURRENT_DIR)/src/google/protobuf/descriptor_database.cc                       \
    $(CURRENT_DIR)/src/google/protobuf/dynamic_message.cc                           \
    $(CURRENT_DIR)/src/google/protobuf/extension_set_heavy.cc                       \
    $(CURRENT_DIR)/src/google/protobuf/generated_message_reflection.cc              \
    $(CURRENT_DIR)/src/google/protobuf/message.cc                                   \
    $(CURRENT_DIR)/src/google/protobuf/reflection_ops.cc                            \
    $(CURRENT_DIR)/src/google/protobuf/service.cc                                   \
    $(CURRENT_DIR)/src/google/protobuf/text_format.cc                               \
    $(CURRENT_DIR)/src/google/protobuf/unknown_field_set.cc                         \
    $(CURRENT_DIR)/src/google/protobuf/wire_format.cc                               \
    $(CURRENT_DIR)/src/google/protobuf/io/gzip_stream.cc                            \
    $(CURRENT_DIR)/src/google/protobuf/io/printer.cc                                \
    $(CURRENT_DIR)/src/google/protobuf/io/strtod.cc                                 \
    $(CURRENT_DIR)/src/google/protobuf/io/tokenizer.cc                              \
    $(CURRENT_DIR)/src/google/protobuf/io/zero_copy_stream_impl.cc                  \
    $(CURRENT_DIR)/src/google/protobuf/compiler/importer.cc                         \
    $(CURRENT_DIR)/src/google/protobuf/compiler/parser.cc

# C++ full library - libc++ version for the platform
# =======================================================
include $(CLEAR_VARS)

LOCAL_MODULE := libprotobuf-rokid-cpp-full
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := $(protobuf_cc_full_src_files)
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/$(CURRENT_DIR)/android \
    $(LOCAL_PATH)/$(CURRENT_DIR)/src

LOCAL_CFLAGS := -frtti -std=c++11 $(IGNORED_WARNINGS)
LOCAL_SHARED_LIBRARIES := libz

include $(BUILD_SHARED_LIBRARY)
