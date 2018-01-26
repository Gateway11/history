//
//  vad.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __common_audio_vad__
#define __common_audio_vad__

#include "NNVadIntf.h"

#include "audio_format.h"
#include "typedefs.h"

namespace __vsys__ {

#define VAD_ON_MASK (0x1 << 0)
#define VAD_OFF_MASK (0x1 << 1)
#define VAD_ON(flag) ((flag & VAD_ON_MASK) != 0)
#define VAD_OFF(flag) ((flag & VAD_OFF_MASK) != 0)

class Vad{
public:
    Vad(const audio_format_t& audio_format, vsys_bool_t has_aec);
    vsys_int_t process(const vsys_float_t* data_in, vsys_uint_t len_in, vsys_bool_t force_start,
                       vsys_float_t*& data_out, vsys_uint_t& len_out);
    vsys_int_t set_vad_end_frame_num(vsys_uint_t frame_num);
    vsys_int_t get_vad_end_frame_index(vsys_int_t& begin, vsys_int_t& end);
    vsys_float_t get_energy_last_frame();
    vsys_float_t get_energy_threshold();
    
    vsys_bool_t reset;
    
private:
    const audio_format_t& audio_format;
    
    VD_HANDLE vad_hanlder;
    
    vsys_size_t frame_size;
    vsys_uint_t vad_end_frame_num;
    vsys_uint_t vad_frame_index;
    vsys_uint_t vad_last_frame;
    
    vsys_float_t* buff;
    vsys_size_t buff_index;
    vsys_size_t buff_size;
    
    vsys_float_t* buff_vad;
    vsys_size_t vad_index;
    vsys_size_t vad_buff_size;

    vsys_int_t begin_frame_index;
    vsys_int_t end_frame_index;
    
    vsys_bool_t has_aec;
    vsys_bool_t has_vad;
};
}
#endif /* __common_audio_vad__ */
