//
//  bf.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_bf__
#define __vsys_bf__

#include "r2ssp.h"

#include "audio_format.h"

namespace __vsys__ {
    
class Bf{
public:
    Bf(const audio_format_t& audio_format, const vsys_uint_t* mic_ids,
       const vsys_float_t* mic_pos, const vsys_float_t* mic_delay);
    
    vsys_int_t process(const vsys_float_t** data_in, const vsys_uint_t len_in,
                       vsys_float_t*& data_out, vsys_uint_t& len_out);
    
    void steer(vsys_float_t azimuth, vsys_float_t elevation, vsys_int_t steer = 1);
    
    const vsys_float_t* get_sl_info();
    
    vsys_bool_t check_sl(vsys_float_t azimuth, vsys_float_t elevation);
    
    void reset();
    
    Bf(const Bf& __bf) = delete;
    
    Bf& operator=(const Bf& __bf) = delete;

    ~Bf();

private:
    const audio_format_t& audio_format;
    
    r2ssp_handle bf_handle;
    
    vsys_float_t* _mic_pos;
    vsys_float_t* _mic_delay;
    vsys_uint_t* _mic_ids;
    
    vsys_uint_t frame_size;
    
    vsys_float_t* buff;
    vsys_float_t* buff_o;
    vsys_uint_t buff_o_size;
    
    vsys_float_t sl_info[2];
};
}
#endif /* __vsys_bf__ */
