//
//  rs.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_rs__
#define __vsys_rs__

#include "audio_format.h"
#include "fa_resample.h"
#include "fa_fir.h"

namespace __vsys__ {
    
class rs{
public:
    rs(audio_format_t audio_format);
    int process(float **pDataIn, int iLenIn, float **&pDataOut, int &iLenOut);
    
private :
    int frame_size;
    ZNS_LIBRESAMPLE::uintptr_t rs_filter;
};

#endif /* __vsys_rs__ */
    
}
