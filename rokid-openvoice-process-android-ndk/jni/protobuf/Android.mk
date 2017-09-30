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

MY_LOCAL_PATH := $(call my-dir)

IGNORED_WARNINGS := -Wno-sign-compare -Wno-unused-parameter -Wno-sign-promo -Wno-error=return-type

CC_LITE_SRC_FILES := \
   	$(MY_LOCAL_PATH)/src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc         \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc        \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/common.cc                              \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/once.cc                                \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/hash.h                                 \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/map_util.h                             \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/shared_ptr.h                           \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/stringprintf.cc                        \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/stringprintf.h                         \
    $(MY_LOCAL_PATH)/src/google/protobuf/extension_set.cc                             \
    $(MY_LOCAL_PATH)/src/google/protobuf/generated_message_util.cc                    \
    $(MY_LOCAL_PATH)/src/google/protobuf/message_lite.cc                              \
    $(MY_LOCAL_PATH)/src/google/protobuf/repeated_field.cc                            \
    $(MY_LOCAL_PATH)/src/google/protobuf/wire_format_lite.cc                          \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/coded_stream.cc                           \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/coded_stream_inl.h                        \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/zero_copy_stream.cc                       \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/zero_copy_stream_impl_lite.cc

# C++ full library
# =======================================================
protobuf_cc_full_src_files := \
    $(CC_LITE_SRC_FILES)                                             \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/strutil.cc                             \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/strutil.h                              \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/substitute.cc                          \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/substitute.h                           \
    $(MY_LOCAL_PATH)/src/google/protobuf/stubs/structurally_valid.cc                  \
    $(MY_LOCAL_PATH)/src/google/protobuf/descriptor.cc                                \
    $(MY_LOCAL_PATH)/src/google/protobuf/descriptor.pb.cc                             \
    $(MY_LOCAL_PATH)/src/google/protobuf/descriptor_database.cc                       \
    $(MY_LOCAL_PATH)/src/google/protobuf/dynamic_message.cc                           \
    $(MY_LOCAL_PATH)/src/google/protobuf/extension_set_heavy.cc                       \
    $(MY_LOCAL_PATH)/src/google/protobuf/generated_message_reflection.cc              \
    $(MY_LOCAL_PATH)/src/google/protobuf/message.cc                                   \
    $(MY_LOCAL_PATH)/src/google/protobuf/reflection_ops.cc                            \
    $(MY_LOCAL_PATH)/src/google/protobuf/service.cc                                   \
    $(MY_LOCAL_PATH)/src/google/protobuf/text_format.cc                               \
    $(MY_LOCAL_PATH)/src/google/protobuf/unknown_field_set.cc                         \
    $(MY_LOCAL_PATH)/src/google/protobuf/wire_format.cc                               \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/gzip_stream.cc                            \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/printer.cc                                \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/strtod.cc                                 \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/tokenizer.cc                              \
    $(MY_LOCAL_PATH)/src/google/protobuf/io/zero_copy_stream_impl.cc                  \
    $(MY_LOCAL_PATH)/src/google/protobuf/compiler/importer.cc                         \
    $(MY_LOCAL_PATH)/src/google/protobuf/compiler/parser.cc

# C++ full library - libc++ version for the platform
# =======================================================
include $(CLEAR_VARS)

LOCAL_MODULE := libprotobuf-rokid-cpp-full
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc
LOCAL_SRC_FILES := $(protobuf_cc_full_src_files)
LOCAL_C_INCLUDES := \
    $(MY_LOCAL_PATH)/android \
    $(MY_LOCAL_PATH)/src

LOCAL_CFLAGS := -frtti -std=c++11 $(IGNORED_WARNINGS)
LOCAL_SHARED_LIBRARIES := libz

LOCAL_EXPORT_C_INCLUDES := $(MY_LOCAL_PATH)/src

include $(BUILD_SHARED_LIBRARY)
