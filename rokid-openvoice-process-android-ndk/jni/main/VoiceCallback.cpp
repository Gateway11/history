#include "VoiceCallback.h"

void VoiceCallback::voice_event(const int32_t id, const int32_t event, const double sl, const double energy){
}

void VoiceCallback::intermediate_result(const int32_t id, const int32_t type, const string& asr){
}

void VoiceCallback::voice_command(const int32_t id, const string& asr, const string& nlp, const string& action){
}

void VoiceCallback::speech_error(const int32_t id, const int32_t errcode){
}

