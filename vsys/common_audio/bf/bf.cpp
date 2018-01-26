//
//  bf.cpp
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#define LOG_TAG "vsys_bf"

#include <string.h>
#include <math.h>
#include <assert.h>

#include "bf.h"
#include "log.h"
#include "buf_manager.h"

namespace __vsys__ {
    
Bf::Bf(const audio_format_t& audio_format, const vsys_uint_t* mic_ids,
       const vsys_float_t* mic_pos, const vsys_float_t* mic_delay)
    :audio_format(audio_format){
    
    frame_size = audio_format.sampleRateHz / 1000 * 10;
        
    _mic_pos = new vsys_float_t[audio_format.numChannels * 3];
    _mic_delay = new vsys_float_t[audio_format.numChannels];
    _mic_ids = new vsys_uint_t[audio_format.numChannels];

    memcpy(_mic_ids, mic_ids, audio_format.numChannels * sizeof(vsys_uint_t));
    memcpy(_mic_pos, mic_pos, audio_format.numChannels * 3 * sizeof(vsys_float_t));
    memcpy(_mic_delay, mic_delay, audio_format.numChannels * sizeof(vsys_float_t));
        
    buff_o_size = frame_size;
    buff = new vsys_float_t[frame_size * audio_format.numChannels];
    buff_o = new vsys_float_t[buff_o_size];
        
    bf_handle = VSYS_NULL;
    reset();
}

vsys_int_t Bf::process(const vsys_float_t **data_in, const vsys_uint_t len_in,
                       vsys_float_t *&data_out, vsys_uint_t &len_out){
    assert(len_in >= 0);
    if(bf_handle){
        if(len_in > buff_o_size){
            buff_o_size = len_in;
            vsys_reallocate(&buff_o, 0, buff_o_size);
        }
        for (vsys_uint_t i = 0; i < len_in; i += frame_size) {
            for(vsys_uint_t j = 0; j < audio_format.numChannels; j++){
                memcpy(buff + j * frame_size, data_in[_mic_ids[j]] + i, frame_size * sizeof(vsys_float_t));
            }
            r2ssp_bf_process(bf_handle, buff, frame_size * audio_format.numChannels, audio_format.numChannels, buff_o + i);
        }
        data_out = buff_o;
        len_out = len_in;
        return 0;
    }
    return -1;
}

void Bf::steer(vsys_float_t azimuth, vsys_float_t elevation, vsys_int_t steer){
    sl_info[0] = azimuth;
    sl_info[1] = elevation;
    if(steer > 0 && bf_handle){
        r2ssp_bf_steer(bf_handle, sl_info[0], sl_info[1], sl_info[0] + 3.1415926f, 0);
    }
}
    
const vsys_float_t* Bf::get_sl_info(){
    vsys_float_t __sl_info[2];
    vsys_int_t azimuth = (sl_info[0] - 3.1415936f) * 180 / 3.1415936f + 0.1f;
    while (azimuth < 0) azimuth += 360;
    while (azimuth >= 360) azimuth -= 360;
    __sl_info[0] = azimuth;
    __sl_info[1] *= sl_info[1] * 180 / 3.1415936f;
    return __sl_info;
}
    
vsys_bool_t Bf::check_sl(vsys_float_t azimuth, vsys_float_t elevation){
    vsys_float_t delt = fabs(sl_info[0] - azimuth);
    vsys_float_t delt2 = acos(sin(sl_info[0]) * sin(azimuth) + cos(sl_info[0]) * cos(azimuth) * cos(elevation - sl_info[1]));
    if(delt > 3.14) delt = fabs(delt - 6.28f);
    return (delt < 0.52f || delt2 < 0.52f) ? VSYS_TRUE : VSYS_FALSE;
}
    
void Bf::reset(){
    if(audio_format.numChannels > 1){
        r2ssp_bf_free(bf_handle);
        bf_handle = r2ssp_bf_create(_mic_pos, audio_format.numChannels);
        r2ssp_bf_init(bf_handle, 10, audio_format.sampleRateHz);
        r2ssp_bf_set_mic_delays(bf_handle, _mic_delay, audio_format.numChannels);
        
        memset(sl_info, 0, sizeof(vsys_float_t) * 2);
        steer(sl_info[0], sl_info[1]);
    }else{
        bf_handle = VSYS_NULL;
    }
}

Bf::~Bf(){
    if(bf_handle) r2ssp_bf_free(bf_handle);
    delete[] _mic_ids;
    delete[] _mic_pos;
    delete[] _mic_delay;
    delete[] buff;
    delete[] buff_o;
 }
}
