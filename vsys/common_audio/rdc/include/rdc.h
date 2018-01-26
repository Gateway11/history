//
//  rdc.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_rdc__
#define __vsys_rdc__

#include "typedefs.h"
#include "audio_format.h"

namespace __vsys__ {
    class Rdc{
    public:
        Rdc(const audio_format_t& audio_foramt, vsys_uint_t* mic_ids, vsys_uint_t* mic_ids_aec, vsys_uint_t max_len);
        
        vsys_int_t process(vsys_float_t** data, vsys_uint_t& length);
        
        vsys_uint_t* blocking;
        
    private:
        const audio_format_t& audio_format;
        
        vsys_uint_t max_len;
        vsys_uint_t index;
        
        vsys_uint_t* mic_ids;
        vsys_uint_t* mic_ids_aec;
        
        vsys_float_t* count;
        vsys_float_t* count_var;
    };
}

#endif /* __vsys_rdc__ */

