//
//  siren_impl.h
//  blacksiren
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __siren_impl__
#define __siren_impl__

#include "siren.h"

namespace __blacksiren__ {

class  SirenImpl{
public:
    int init();
    
    int start_process();
    
    int stop_process();
    
    int release();
    
    int set_state(int state);
    
    int add_vt_word();
    
    int remove_vt_word();
    
    int get_vt_word();
};

} /* namespace __blacksiren__ */

#endif /* __siren_impl__ */
