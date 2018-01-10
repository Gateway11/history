/*
 * Copyright 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_LOG_H
#define ANDROID_LOG_H

#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "openvoice_process"
#endif

#if defined(__ANDROID__) || defined(ANDROID)
#include <android/log.h>
#ifndef ALOGV
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#endif
#ifndef ALOGD
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif
#ifndef ALOGI
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif
#ifndef ALOGW
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#endif
#ifndef ALOGE
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif
#else
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#define ALOGV(...){ __dx_logtime_print('V', LOG_TAG); printf(__VA_ARGS__); printf("\n");}
#define ALOGD(...){ __dx_logtime_print('D', LOG_TAG); printf(__VA_ARGS__); printf("\n");}
#define ALOGI(...){ __dx_logtime_print('I', LOG_TAG); printf(__VA_ARGS__); printf("\n");}
#define ALOGW(...){ __dx_logtime_print('W', LOG_TAG); printf(__VA_ARGS__); printf("\n");}
#define ALOGE(...){ __dx_logtime_print('E', LOG_TAG); printf(__VA_ARGS__); printf("\n");}

#define __dx_logtime_print(level, tag) \
    struct timeval tv; \
    struct tm ltm; \
    gettimeofday(&tv, NULL); \
    localtime_r(&tv.tv_sec, &ltm); \
    printf("%02d-%02d %02d:%02d:%02d.%03d  %04d %c %s: ", \
    ltm.tm_mon, ltm.tm_mday, \
    ltm.tm_hour, ltm.tm_min, ltm.tm_sec, \
    tv.tv_usec / 1000, getpid(), level, tag);
#endif
#endif //ANDROID_LOG_H
