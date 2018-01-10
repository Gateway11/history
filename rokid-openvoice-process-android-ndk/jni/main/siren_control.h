#ifndef AUDIO_RECORDER_H
#define AUDIO_RECORDER_H

#include <vector>
#include <string>

#include "siren.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    bool setup(void*, on_voice_event_t);
    
    int insert_vt_word(siren_vt_word& vt_word);
    
    int delete_vt_word(const std::string& vt_word);
    
    int query_vt_word(std::vector<siren_vt_word>& _vt_words_in);
    
    void set_siren_state_change(int state);
    
    int init_input(void*);
    
    void release_input(void*);
    
    int start_input(void*);
    
    void stop_input(void*);
    
    int read_input(void*, char*, int);
    
    void on_err_input(void*);
    
    void state_changed_callback(void*, int);
    
    void _start_siren_process_stream();
    
    void _stop_siren_process_stream();
    
    int find_card(const char*);
    
#ifdef __cplusplus
};
#endif

#endif
