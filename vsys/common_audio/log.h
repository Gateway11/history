//
//  log.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_log__
#define __vsys_log__

#ifndef LOG_TAG
#define LOG_TAG NULL
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
#endif /* __vsys_log__ */
