#ifndef VOICE_CALLBACK_H
#define VOICE_CALLBACK_H

#include <string>
#include <mutex>
#include <functional>

using std::string;

class VoiceCallback{
private:
    enum{
        MSG_VOICE_EVENT_ID = 0,
        MSG_INTERMEDIATE_RESULT_ID,
        MSG_VOICE_COMMAND_ID,
        MSG_SPEECH_ERROR_ID,
    };
    
    std::function<void(int32_t what, const std::string&)> send;
    
    std::mutex g_i_mutex;
    
    void send_message(int32_t what, const std::string& data){
        //        std::lock_guard<std::mutex> lock(g_i_mutex);
        if(send) send(what, data);
    }
    
public:
    
    template<typename F>
    void set_callback(const F& send){this->send = send;}
    
    void voice_event(const int32_t id, const int32_t event, const double sl = 0.0, const double energy = 0.0);
    
    void intermediate_result(const int32_t id, const int32_t type, const string& asr);
    
    void voice_command(const int32_t id, const string& asr, const string& nlp, const string& action);
    
    void speech_error(const int32_t id, const int32_t errcode);
};

#endif
