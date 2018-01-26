//
//  siren.cpp
//  blacksiren
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#include "siren.h"
#include "siren_impl.h"

using namespace __blacksiren__;

siren_handle_t r2_siren_init(siren_input_t *siren_input){
    return reinterpret_cast<siren_handle_t>(new SirenImpl);
}

int r2_siren_start_process(siren_handle_t handle){
    return ((SirenImpl *)handle)->start_process();
}

int r2_siren_stop_process(siren_handle_t handle){
    return ((SirenImpl *)handle)->stop_process();
}

int r2_siren_release(siren_handle_t handle){
    return ((SirenImpl *)handle)->release();
}

int r2_siren_set_state(siren_handle_t handle, siren_status_t status){
    return ((SirenImpl *)handle)->set_state(status);
}

int r2_siren_add_vt_word(siren_handle_t handle){
    return ((SirenImpl *)handle)->add_vt_word();
}

int r2_siren_remove_vt_word(siren_handle_t handle){
    return ((SirenImpl *)handle)->remove_vt_word();
}

int r2_siren_get_vt_word(siren_handle_t handle){
    return ((SirenImpl *)handle)->get_vt_word();
}
