#ifndef VOICE_CALLBACK_H
#define VOICE_CALLBACK_H

#include <string>

using std::string;

class VoiceCallback{
private:
    void* callback;

public:
    void set_callback(const void* callback){}

    void voice_event(const int32_t id, const int32_t event, const double sl = 0.0, const double energy = 0.0);

    void intermediate_result(const int32_t id, const int32_t type, const string& asr);

    void voice_command(const int32_t id, const string& asr, const string& nlp, const string& action);

    void speech_error(const int32_t id, const int32_t errcode);
};

#endif
