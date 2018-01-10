#ifndef VOICE_SERVICE_H
#define VOICE_SERVICE_H

#define EXTRA_ACCEPT "accept"
#define EXTRA_NONE "none"

#include <log.h>
#include <thread>
#include <condition_variable>
#include <stdlib.h>
#include <list>
#include <vector>
#include <functional>

#include "siren.h"
#include "speech.h"
#include "voice_config.h"
#include "VoiceCallback.h"
#include "EventTypes.h"

using namespace std;
using namespace rokid::speech;
using namespace openvoice_process;

class VoiceService {
public:
    VoiceService();
    ~VoiceService(){}
    
    bool init(std::function<std::string()> fun);
    void start_siren(const bool isopen);
    void set_siren_state(const int32_t state);
    void network_state_change(const bool isconnect);
    void update_stack(const string& appid);
    void update_config(const string&, const string&, const string&, const string&);
    
    template<typename F>
    void regist_callback(const F& send){this->_callback->set_callback(send);}
    
    int32_t add_vt_word(siren_vt_word&);
    int32_t remove_vt_word(const string&);
    int32_t get_vt_word(vector<siren_vt_word>& _vt_words_in);
    
private:
#ifdef USB_AUDIO_DEVICE
    bool wait_for_alsa_usb_card();                                                             
#endif
    int mCurrentSirenState = SIREN_STATE_UNKNOWN;
    int mCurrentSpeechState = SPEECH_STATE_UNKNOWN;
    enum {
        SIREN_STATE_UNKNOWN = 0,
        SIREN_STATE_INITED,
        SIREN_STATE_STARTED,
        SIREN_STATE_STOPED
    };
    enum {
        SPEECH_STATE_UNKNOWN = 0, SPEECH_STATE_PREPARED, SPEECH_STATE_RELEASED
    };
    
    static inline int32_t transform_string_to_event(const std::string& extra){
        if(extra == EXTRA_ACCEPT || extra == EXTRA_NONE){
            return VoiceEvent::VOICE_ACCEPT;
        }else{
            return VoiceEvent::VOICE_REJECT;
        }
    }
    
    inline void clear(const int32_t id = -1){
        std::lock_guard<std::mutex> lg(session_mutex);
        if(id == -1 || id == session_id) session_id = -1;
    }
    
    std::mutex event_mutex;
    std::mutex speech_mutex;
    std::mutex siren_mutex;
    std::mutex session_mutex;
    std::condition_variable event_cond;
    std::thread event_thread;
    std::thread response_thread;
    std::once_flag start_flag;
    
    void onEvent();
    void onResponse();
    
    int32_t vad_start();
    void voice_print(const voice_event_t *);
    void voice_event_callback(voice_event_t *voice_event);
    
    shared_ptr<Speech> _speech;
    shared_ptr<VoiceCallback> _callback;
    shared_ptr<VoiceConfig> _voice_config;
    list<voice_event_t*> _events;
    std::function<std::string()> get_skill_options;
    
    string appid;
    string vt_word;
    int32_t session_id;
    int32_t vt_start;
    int32_t vt_end;
    float vt_energy;
    bool asr_finished = false;
    bool has_vt = false;
    bool openSiren = true;
    bool local_sleep = false;
};

#endif // VOICE_SERVICE_H
