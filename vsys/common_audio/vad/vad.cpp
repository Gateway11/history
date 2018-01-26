//
//  vad.cpp
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#define LOG_TAG "vsys_vad"

#include <string.h>

#include "vad.h"
#include "buf_manager.h"

namespace __vsys__ {

Vad::Vad(const audio_format_t& audio_format, vsys_bool_t has_aec)
    :vad_end_frame_num(45), vad_frame_index(0), vad_last_frame(0),
    buff_index(0), begin_frame_index(0), end_frame_index(0), has_vad(VSYS_FALSE),
    reset(VSYS_FALSE), audio_format(audio_format), has_aec(has_aec){
    
    frame_size = audio_format.sampleRateHz / 1000 * 10;
    vad_hanlder = VD_NewVad(1);

//    int speech_frame_num = 1000;
//    VD_SetVadParam(vad_hanlder, VD_PARAM_MAXSPEECHFRAMENUM, &speech_frame_num);
//    set_vad_end_frame_num(vad_end_frame_num);
        
    buff_size = frame_size;
    buff = (vsys_float_t *)vsys_allocate(buff_size * sizeof(float));
        
    vad_buff_size = frame_size;
    buff_vad = (vsys_float_t *)vsys_allocate(vad_buff_size * sizeof(float));
}

vsys_int_t Vad::process(const vsys_float_t* data_in, vsys_size_t len_in, vsys_bool_t force_start,
                        vsys_float_t*& data_out, vsys_size_t& len_out){
    assert(len_in >= 0);
    
    vsys_uint_t num_frames = (len_in + buff_index) / frame_size;
    put_buff(&buff, &buff_index, &buff_size, data_in, len_in);
    buff_index %= frame_size;
   
    vsys_int_t i, ret = 0;
    vad_index = 0;
    
    if(reset) VD_ResetVad(vad_hanlder);
    reset = VSYS_FALSE;
    
    for (i = 0; i < num_frames; i++) {
        if(force_start){
            VD_SetStart(vad_hanlder, has_aec ? 1 : 0);
            set_vad_end_frame_num(2000);
        }
        vad_frame_index++;
        VD_InputFloatWave(vad_hanlder, buff + i * frame_size, frame_size, false, has_aec);
        if(!has_vad){
            if(VD_GetVoiceStartFrame(vad_hanlder) >= 0){
                ret |= VAD_ON_MASK;
                has_vad = true;
                begin_frame_index = 0;
                end_frame_index = 0;
            }
        }else{
            vsys_int_t frame_index = VD_GetVoiceStopFrame(vad_hanlder);
            if(frame_index > 0){
                ret |= VAD_OFF_MASK;
                end_frame_index = vad_frame_index - frame_index;
                end_frame_index = end_frame_index < 0 ? 0 : end_frame_index;
                begin_frame_index = end_frame_index + VD_GetVoiceFrameNum(vad_hanlder);
            }
        }
        if(has_vad){
            int curr_frame = VD_GetVoiceFrameNum(vad_hanlder);
            if(curr_frame > vad_last_frame){
                vsys_float_t* vad_data = (vsys_float_t *)VD_GetFloatVoice(vad_hanlder) + vad_last_frame * frame_size;
                vsys_size_t vad_len = (curr_frame - vad_last_frame) * frame_size;
                put_buff(&buff_vad, &vad_index, &vad_buff_size, vad_data, vad_len);
                vad_last_frame = curr_frame;
            }
            if(VAD_OFF(ret)){
                vsys_uint_t num = num_frames -  (i + 1);
                for (vsys_uint_t j = 0; j < num; j++)
                    buff_index += frame_size;
                reset = VSYS_TRUE;
                has_vad = VSYS_FALSE;
                vad_last_frame = 0;
                vad_frame_index = 0;
                break;
            }
        }
    }
    data_out = buff_vad;
    len_out = vad_index;
    return ret;
}
    
vsys_int_t Vad::set_vad_end_frame_num(vsys_uint_t frame_num){
    VD_SetVadParam(vad_hanlder, VD_PARAM_MINSILFRAMENUM, frame_num > 0 ? &frame_num : &vad_end_frame_num);
    return 0;
}

vsys_int_t Vad::get_vad_end_frame_index(vsys_int_t& begin, int& end){
    begin = begin_frame_index * frame_size + buff_index;
    end = end_frame_index * frame_size + buff_index;
    return 0;
}

vsys_float_t Vad::get_energy_last_frame(){
    return VD_GetLastFrameEnergy(vad_hanlder);
}
    
vsys_float_t Vad::get_energy_threshold(){
    return VD_GetThresholdEnergy(vad_hanlder);
}
}
