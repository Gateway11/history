//
//  rs.cpp
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#include "rs.h"

namespace __vsys__ {

rs::rs(audio_format_t audio_format){
    rs_filter = ZNS_LIBRESAMPLE::fa_resample_filter_init(1, 3, 1.0f, ZNS_LIBRESAMPLE::HAMMING);
}
    
int rs::process(float **pDataIn, int iLenIn, float **&pDataOut, int &iLenOut){
    return 0;
}

}
