#include "siren_control.h"

#include <SLES/OpenSLES.h>

#include "VoiceService.h"
//#include "opensl_io.h"
#include "mic_array.h"
#include "log.h"

#include <string.h>
#define SAMPLERATE 48000
#define CHANNELS 4
#define PERIOD_TIME 20 //ms
#define FRAME_SIZE SAMPLERATE*PERIOD_TIME/1000
#define BUFFER_SIZE FRAME_SIZE*CHANNELS

siren_t _siren;
siren_proc_callback_t event_callback;
struct mic_array_device_t *mic_array_device = NULL;
void* __token = nullptr;

//OPENSL_STREAM* stream = nullptr;

siren_input_if_t siren_input = { init_input, release_input, start_input,
    stop_input, read_input, on_err_input };

siren_state_changed_callback_t siren_state_change = { state_changed_callback };

bool setup(void*token, on_voice_event_t callback) {
    __token = token;
    
    if(mic_array_device_open(&mic_array_device) != 0){
        ALOGE("open mic_array failed");
        return false;
    }
    event_callback.voice_event_callback = callback;
    _siren = init_siren(token, NULL, &siren_input);
    return true;
}

int insert_vt_word(siren_vt_word& vt_word){
    return add_vt_word(_siren, &vt_word, true);
}

int delete_vt_word(const std::string& vt_word){
    return remove_vt_word(_siren, vt_word.c_str());
}

int query_vt_word(std::vector<siren_vt_word>& _vt_words_in){
    siren_vt_word *_vt_words_out = nullptr;
    int32_t count = get_vt_word(_siren, &_vt_words_out);
    if(count > 0 && _vt_words_out != nullptr){
        _vt_words_in.reserve(count);
        for(int i = 0; i < count; i++){
            _vt_words_in.push_back(siren_vt_word());
            _vt_words_in[i].vt_type = _vt_words_out[i].vt_type;
            _vt_words_in[i].vt_word = _vt_words_out[i].vt_word;
            _vt_words_in[i].vt_pinyin = _vt_words_out[i].vt_pinyin;
        }
    }
    return count;
}

void _start_siren_process_stream() {
    start_siren_process_stream(_siren, &event_callback);
}

void _stop_siren_process_stream() {
    stop_siren_process_stream(_siren);
}

void set_siren_state_change(int state) {
    set_siren_state(_siren, state, &siren_state_change);
}

int init_input(void *token) {
    return 0;
}

void release_input(void *token) {
    mic_array_device->finish_stream(mic_array_device);
}

int start_input(void *token) {
    //	stream = android_OpenAudioDevice(48000, 1, 0, 1024);
    //	if (stream == nullptr) {
    //		ALOGE("failed to open audio device ! \n");
    //		return -1;
    //	}
    return mic_array_device->start_stream(mic_array_device);
    //	ALOGE("open audio device success! \n");
    //	return 0;
}

void stop_input(void *token) {
    ALOGV("%s", __FUNCTION__);
    //	android_CloseAudioDevice(stream);
    mic_array_device->stop_stream(mic_array_device);
}

int read_input(void *token, char *buff, int frame_cnt) {
    //	return android_AudioIn(stream, (short*)buff, frame_cnt/2);
    return mic_array_device->read_stream(mic_array_device, buff, frame_cnt);
}

int find_card(const char *snd) {
    return -1;
}

void on_err_input(void *token) {
    ALOGV("%s", __FUNCTION__);
}

void state_changed_callback(void *token, int state) {
    ALOGV("%s : %d", __FUNCTION__, state);
}

