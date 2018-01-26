//
//  aec.cpp
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#include "aec.h"
#include "buf_manager.h"

namespace __vsys__ {

Aec::Aec(const audio_format_t& audio_format, vsys_uint_t* mic_ids,
    vsys_uint_t speaker_num, vsys_uint_t* speaker_ids, vsys_float_t scaling)
    :audio_format(audio_format), speaker_num(speaker_num), scaling(scaling), buff_index(0){
        
    _mic_ids = (vsys_uint_t *)vsys_allocate(audio_format.numChannels * sizeof(vsys_uint_t));
    _speaker_ids = (vsys_uint_t *)vsys_allocate(speaker_num * sizeof(vsys_uint_t));
    memcpy(_mic_ids, mic_ids, audio_format.numChannels * sizeof(vsys_uint_t));
    memcpy(_speaker_ids, speaker_ids, speaker_num * sizeof(vsys_uint_t));
     
    aec_handle = r2ssp_aec_create(0);
    r2ssp_aec_init(aec_handle, audio_format.sampleRateHz, audio_format.numChannels, speaker_num);
        
    frame_size = audio_format.sampleRateHz / 1000 * 10;
    buff_o_size = frame_size;
    buff = (vsys_float_t *)vsys_allocate((audio_format.numChannels + speaker_num) * frame_size * sizeof(vsys_float_t));
    buff_o = vsys_allocate2(audio_format.numChannels, buff_o_size);
}
    
vsys_int_t Aec::process(const vsys_float_t **data_in, const vsys_size_t len_in,
                            vsys_float_t **&data_out, vsys_size_t &len_out){
    assert(len_in >= 0);
    vsys_uint_t total = len_in + buff_index;
    vsys_uint_t num_frames = total / frame_size;
    vsys_size_t buff_len = buff_index, buff_len2 = 0;

    if(num_frames * frame_size > buff_o_size) {
        buff_o_size = num_frames * frame_size;
        vsys_reallocate2(&buff_o, audio_format.numChannels, 0, buff_o_size);
    }
    for (vsys_uint_t i = 0; i < num_frames; i++) {
        for (vsys_uint_t j = 0; j < audio_format.numChannels; j++) {
            memcpy(buff + j * frame_size + buff_len, data_in[_mic_ids[j]] + i * frame_size - buff_len2,
                   frame_size * sizeof(vsys_float_t) - buff_len);
        }
        for (vsys_uint_t j = 0; j < speaker_num; j++) {
            memcpy(buff + (audio_format.numChannels + j) * frame_size + buff_len,
                   data_in[_speaker_ids[j]] + i * frame_size - buff_len2, frame_size * sizeof(vsys_float_t) - buff_len);
        }
        for (vsys_uint_t j = 0; j < (audio_format.numChannels + speaker_num) * frame_size; j++) {
            buff[j] /= scaling;
        }
        r2ssp_aec_buffer_farend(aec_handle, buff + audio_format.numChannels * frame_size, speaker_num * frame_size);
        r2ssp_aec_process(aec_handle, buff, audio_format.numChannels * frame_size, buff, 0);
        for (vsys_uint_t j = 0; j < audio_format.numChannels; j++) {
            memcpy(buff_o[j] + i * frame_size, buff + j * frame_size, frame_size * sizeof(vsys_float_t));
        }
        buff_len = 0;
        buff_len2 = buff_index;
        len_out += frame_size;
    }
    data_out = buff_o;
    buff_index = total % frame_size;
    for (vsys_uint_t i = 0; i < audio_format.numChannels; i++) {
        memcpy(buff + i * frame_size, data_in[_mic_ids[i]] + num_frames * frame_size, buff_index * sizeof(vsys_float_t));
    }
    for (vsys_uint_t i = 0; i < speaker_num; i++) {
        memcpy(buff + (audio_format.numChannels + i) * frame_size,
               data_in[_speaker_ids[i]] + num_frames * frame_size, buff_index * sizeof(vsys_float_t));
    }
    return 0;
}
    
Aec::~Aec(){
    r2ssp_aec_free(aec_handle);
    
    vsys_free(_mic_ids);
    vsys_free(_speaker_ids);
    vsys_free(buff);
    vsys_free2((void **)buff_o);
}
}
