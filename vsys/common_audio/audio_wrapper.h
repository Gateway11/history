//
//  audio_wrapper.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#ifndef __vsys_audio_wrapper__ 
#define __vsys_audio_wrapper__ 

#include <dlfcn.h>

#include "log.h"

struct pcm;
struct pcm_config;

struct pcm *(*pcm_open)(unsigned int card, unsigned int device,
                             unsigned int flags, struct pcm_config *config);

static void __attribute__((constructor)) before_main(void){
    void *handle = dlopen("libandroid.so", RTLD_NOW | RTLD_LOCAL);
    if (handle == nullptr) {
        ALOGE("Could not open libandroid.so to dynamically load tracing symbols");
    } else {

#define GET_PROC(s) s = dlsym(handle, #s)

        GET_PROC(pcm_open);
    }
}

#endif /* __vsys_audio_wrapper__ */
