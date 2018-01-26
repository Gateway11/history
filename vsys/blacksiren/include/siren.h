//
//  siren.h
//  blacksiren
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __siren__
#define __siren__

#include "siren_types.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    siren_handle_t r2_siren_init(siren_handle_t handle, siren_input_t *siren_input);
    
    int r2_siren_put_data(siren_handle_t handle, char *buff, int len);
    
    int r2_siren_start_process(siren_handle_t handle);
    
    int r2_siren_stop_process(siren_handle_t handle);
    
    int r2_siren_release(siren_handle_t handle);
    
    int r2_siren_set_state(siren_handle_t handle, siren_status_t status);
    
    int r2_siren_add_vt_word(siren_handle_t handle, siren_vt_word_t *vt_word);
    
    int r2_siren_remove_vt_word(siren_handle_t handle);
    
    int r2_siren_get_vt_word(siren_handle_t handle, siren_vt_word_t **vt_words_in);
    
#ifdef __cplusplus
}
#endif

#endif /* __siren__ */
