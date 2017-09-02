#ifndef EVENT_TYPES_H
#define EVENT_TYPES_H

namespace openvoice_process{

enum VoiceEvent{
    VOICE_COMING = 0,
    VOICE_START,
    VOICE_ACCEPT,
    VOICE_REJECT,
    VOICE_CANCEL,
};

enum ASRResultType{
    ASR_INTER_RESULT_BEGIN   = 0,
    ASR_INTER_RESULT_END     = 2,
};

enum SpeechError{
    SPEECH_ERROR_UNAVAILABLE = 101,
    SPEECH_ERROR_TIMEOUT     = 103,
};

} // namespace openvoice_process
#endif
