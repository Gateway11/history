#include "VoiceCallback.h"

static const void* _callback = nullptr;

VoiceCallback::VoiceCallback(const void* callback){
	_callback = callback;
}

void VoiceCallback::voice_command(const string& asr, const string& nlp, const string& action){

}

void VoiceCallback::voice_event(int event, bool has_sl, double sl, double energy, double threshold){

}

void VoiceCallback::arbitration(const std::string& extra){

}

void VoiceCallback::speech_error(int errcode){

}
