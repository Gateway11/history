//
//  siren_types.h
//  blacksiren
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __siren_types__
#define __siren_types__

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef unsigned long siren_handle_t;
    
    typedef unsigned int siren_status_t;
    typedef unsigned int siren_vt_type_t;
    typedef unsigned int siren_event_t;
    
    typedef struct{
        int (*start_input_stream)(void);
        int (*read_input_stream)(char *buff, int len);
        int (*stop_input_stream)(void);
    }siren_input_t;
    
    typedef struct{
        typedef struct {
            int start;
            int end;
            float energy;
        } vt_event_t;
        
        siren_event_t event;
        void *buff;
        int length;
        int flag;
        
        float sl;
        float background_energy;
        float background_threshold;
        
        vt_event_t vt;
    }voice_event_t;
    
    typedef struct {
        typedef struct {
            float vt_block_avg_score;
            float vt_block_min_score;
            float vt_classify_shield;
            
            bool vt_left_sil_det;
            bool vt_right_sil_det;
            bool vt_remote_check_with_aec;
            bool vt_remote_check_without_aec;
            bool vt_local_classify_check;
            
            char nnet_path[128];
        } siren_vt_alg_config_t;
        
        siren_vt_type_t vt_type;
        char vt_word[128];
        char vt_pinyin[128];
        char vt_phone[256];
        bool default_config;
        siren_vt_alg_config_t alg_config;
    } siren_vt_word_t;
    
    typedef void (*callback_t)(voice_event_t *);
    
    enum {
        SIREN_EVENT_VAD_COMING = 100,
        SIREN_EVENT_VAD_START,
        SIREN_EVENT_VAD_DATA,
        SIREN_EVENT_VAD_END,
        SIREN_EVENT_VAD_CANCEL,
        SIREN_EVENT_LOCAL_WAKE,
        SIREN_EVENT_LOCAL_SLEEP,
        SIREN_EVENT_HOTWORD,
        SIREN_EVENT_VOICE_PRINT,
    };
    
    enum {
        SIREN_STATE_AWAKE =1 ,
        SIREN_STATE_SLEEP
    };
    
    enum{
        VT_TYPE_AWAKE   = 1,
        VT_TYPE_SLEEP   = 2,
        VT_TYPE_HOTWORD = 3,
        VT_TYPE_OTHER   = 4
    };
    
#ifdef __cplusplus
}
#endif

#endif /* __siren_types__ */
