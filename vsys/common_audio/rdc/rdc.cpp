//
//  rdc.cpp
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#include <math.h>

#include "rdc.h"
#include "log.h"
#include "buf_manager.h"

#define LOG_TAG "vsys_rdc"

namespace __vsys__ {
    Rdc::Rdc(const audio_format_t& audio_format, vsys_uint_t* mid_ids, vsys_uint_t* mic_ids_aec, vsys_uint_t max_len)
    :audio_format(audio_format), max_len(max_len), index(0){
        
        count = (vsys_float_t *)vsys_allocate(audio_format.numChannels * sizeof(vsys_float_t));
        count_var = (vsys_float_t *)vsys_allocate(audio_format.numChannels * sizeof(vsys_float_t));
        blocking = (vsys_uint_t *)vsys_allocate(audio_format.numChannels * sizeof(vsys_uint_t));
        memset(count, 0, audio_format.numChannels * sizeof(vsys_float_t));
        memset(count_var, 0, audio_format.numChannels * sizeof(vsys_float_t));
        
        this->mic_ids = (vsys_uint_t *)vsys_allocate(audio_format.numChannels * sizeof(vsys_uint_t));
        this->mic_ids_aec = (vsys_uint_t *)vsys_allocate(audio_format.numChannels * sizeof(vsys_uint_t));
        memcpy(this->mic_ids, mic_ids, audio_format.numChannels * sizeof(vsys_float_t));
//        memcpy(this->mic_ids_aec, mic_ids_aec, audio_format.numChannels * sizeof(vsys_float_t));
     }
    
    vsys_int_t Rdc::process(vsys_float_t **data, vsys_uint_t &length){
        assert(length >= 0);
        if(index < max_len){
            for (vsys_uint_t i = 0; i < audio_format.numChannels; i++) {
                vsys_uint_t id = mic_ids[i];
                for (vsys_uint_t j = 0; j < length; j++) {
                    count[id] += data[id][j];
                    count_var[id] += data[id][j] * data[id][j];
                }
            }
            index += length;
            length = 0;
            if(index == max_len){
                for (vsys_uint_t i = 0; i < audio_format.numChannels; i++) {
                    vsys_uint_t id = mic_ids[i];
                    count[id] /= max_len;
                    count_var[id] = sqrt(count_var[id] / max_len - (count[id] * count[id]) + 0.1f);
                    ALOGD("----Calculate Mic AVG %d: %f     %f", id, count[id], count_var[id]);
                    if(count_var[id] > 200000){
                        blocking[id] = 0;
                        ALOGD("-----Mic Error %d:-------------", id);
                    }else blocking[id] = 1;
                }
            }
        }else{
            for (vsys_uint_t i = 0; i < audio_format.numChannels; i++) {
                vsys_uint_t id = mic_ids[i];
                for (vsys_uint_t j = 0; j < length; j++) {
                    data[id][j] -= count[id];
                }
            }
        }
        return 0;
    }
}
