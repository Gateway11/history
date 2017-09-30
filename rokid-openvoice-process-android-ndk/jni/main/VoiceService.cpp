#include <stdio.h>
#include <sys/prctl.h>

#include "VoiceService.h"
#include "audio_recorder.h"

VoiceService::VoiceService() {
	pthread_mutex_init(&event_mutex, NULL);
	pthread_mutex_init(&speech_mutex, NULL);
	pthread_mutex_init(&siren_mutex, NULL);
    pthread_mutex_init(&session_mutex, NULL);
	pthread_cond_init(&event_cond, NULL);

    _voice_config = make_shared<VoiceConfig>();
	_callback = make_shared<VoiceCallback>();
	_speech = new_speech();
    clear();
}

bool VoiceService::init() {
	pthread_mutex_lock(&siren_mutex);
	if (mCurrentSirenState == SIREN_STATE_UNKNOWN) {
		if (!setup(this,
				[](void *token, voice_event_t *event) {((VoiceService*)token)->voice_event_callback(event);})) {
			LOGE("init siren failed.");
			pthread_mutex_unlock(&siren_mutex);
			return false;
		}
	} else {
		goto done;
	}
	mCurrentSirenState = SIREN_STATE_INITED;
	pthread_create(&event_thread, NULL,
			[](void* token)->void* {return ((VoiceService*)token)->onEvent();},
			this);
done: 
    pthread_mutex_unlock(&siren_mutex);
	return true;
}

void VoiceService::start_siren(const bool isopen) {
	LOGV("%s \t isopen : %d \t mCurrState : %d \t opensiren : %d",
			__FUNCTION__, isopen, mCurrentSirenState, openSiren);

	pthread_mutex_lock(&siren_mutex);
	if (isopen && (mCurrentSirenState == SIREN_STATE_INITED
				|| mCurrentSirenState == SIREN_STATE_STOPED)) {
		openSiren = true;
		_start_siren_process_stream();
		mCurrentSirenState = SIREN_STATE_STARTED;

	} else if (!isopen && mCurrentSirenState == SIREN_STATE_STARTED) {
		_stop_siren_process_stream();
		mCurrentSirenState = SIREN_STATE_STOPED;
	}
	if (!isopen && mCurrentSirenState != SIREN_STATE_UNKNOWN) openSiren = false;
	pthread_mutex_unlock(&siren_mutex);
}

void VoiceService::set_siren_state(const int32_t state) {
	set_siren_state_change(state);
	LOGV("current_status     >>   %d", state);
}

void VoiceService::network_state_change(const bool connected) {
	LOGV("network_state_change      isconnect  <<%d>>", connected);
	pthread_mutex_lock(&speech_mutex);
	if (connected && mCurrentSpeechState != SPEECH_STATE_PREPARED) {
        if(_voice_config->config(
                    [&](const char* key, const char* value){_speech->config(key, value);}) 
                    && _speech->prepare()) {
			mCurrentSpeechState = SPEECH_STATE_PREPARED;
			pthread_create(&response_thread, NULL,
					[](void* token)->void* {return ((VoiceService*)token)->onResponse();},
					this);
			pthread_detach(response_thread);

			pthread_mutex_lock(&siren_mutex);
			if (openSiren && (mCurrentSirenState == SIREN_STATE_INITED
							|| mCurrentSirenState == SIREN_STATE_STOPED)) {
				_start_siren_process_stream();
				mCurrentSirenState = SIREN_STATE_STARTED;
			}
			pthread_mutex_unlock(&siren_mutex);
		}
	} else if (!connected && mCurrentSpeechState == SPEECH_STATE_PREPARED) {
		pthread_mutex_lock(&siren_mutex);
		if (mCurrentSirenState == SIREN_STATE_STARTED) {
			_stop_siren_process_stream();
			mCurrentSirenState = SIREN_STATE_STOPED;
		}
		pthread_mutex_unlock(&siren_mutex);
		LOGV("==========================BEGIN============================");
		_speech->release();
		LOGV("===========================END==============================");
		mCurrentSpeechState = SPEECH_STATE_RELEASED;
	}
	pthread_mutex_unlock(&speech_mutex);
}

void VoiceService::update_stack(const string &appid) {
	this->appid = appid;
	LOGE("%s  %s", __FUNCTION__, this->appid.c_str());
}

void VoiceService::update_config(const string& device_id, const string& device_type_id,
                                const string& key, const string& secret) {
    if(!_voice_config->save_config(device_id, device_type_id, key, secret)){
    }
}

int32_t VoiceService::vad_start() {
	if (mCurrentSpeechState == SPEECH_STATE_PREPARED) {
		shared_ptr<Options> options = new_options();
		if (options.get() && has_vt) {
			options->set("voice_trigger", vt_data.c_str());
			char buf[64];
			snprintf(buf, sizeof(buf), "%d", vt_start);
			options->set("trigger_start", buf);
			snprintf(buf, sizeof(buf), "%d", vt_end - vt_start);
			options->set("trigger_length", buf);
			snprintf(buf, sizeof(buf), "%F", vt_energy);
			options->set("voice_power", buf);
			has_vt = false;
		}
		options->set("stack", appid.empty() ? "" : appid.c_str());
		string json;
		options->to_json_string(json);
		LOGV("%s \t %s", __FUNCTION__, json.c_str());
		return _speech->start_voice(options);
	}
	return -1;
}

void VoiceService::voice_print(const voice_event_t *voice_event) {
	if (voice_event && HAS_VT(voice_event->flag)) {
		vt_start = voice_event->vt.start;
		vt_end = voice_event->vt.end;
		vt_energy = voice_event->vt.energy;
		vt_data = (char*) voice_event->buff;
		has_vt = true;
	}
}

void VoiceService::voice_event_callback(voice_event_t *voice_event) {
	pthread_mutex_lock(&event_mutex);
    int32_t len =  0;
    char *buff = nullptr;
    if(voice_event->length > 0){
        len = voice_event->length;
    }
    char *temp = new char[sizeof(voice_event_t) + len];
    if(len) buff = temp + sizeof(voice_event_t);
    
    voice_event_t *_event_t = (voice_event_t*)temp;
    memcpy(_event_t, voice_event, sizeof(voice_event_t));
    
    if ((HAS_VOICE(_event_t->flag) || HAS_VT(_event_t->flag)) && len) {
    	memcpy(buff, voice_event->buff, len);
    	_event_t->buff = buff;
    }
    _events.push_back(_event_t);
    pthread_cond_signal(&event_cond);
    pthread_mutex_unlock(&event_mutex);
}

void* VoiceService::onEvent() {
	prctl(PR_SET_NAME, __FUNCTION__);
    while(true){
        pthread_mutex_lock(&event_mutex);
        while(_events.empty()) {
            pthread_cond_wait(&event_cond, &event_mutex);
        }
        voice_event_t *_event = _events.front();
        _events.pop_front();
        pthread_mutex_unlock(&event_mutex);

        LOGV("event : -------------------------%d----", _event->event);

        switch(_event->event) {
            case SIREN_EVENT_WAKE_PRE:
                _callback->voice_event(-1, VoiceEvent::VOICE_COMING, _event->sl);
                LOGV("VAD_COMING");
                break;
            case SIREN_EVENT_WAKE_CMD:
                _callback->voice_event(-1, VoiceEvent::VOICE_LOCAL_WAKE, _event->sl);
                LOGV("VAD_CMD");
                break;
            case SIREN_EVENT_VAD_START:
                if(session_id < 0) {
                    session_id = vad_start();
                    _callback->voice_event(session_id, VoiceEvent::VOICE_START);
                    LOGV("VAD_START\t\t ID  :  <<%d>>", session_id);
                }
                break;
            case SIREN_EVENT_VAD_DATA:
                if (session_id > 0 && HAS_VOICE(_event->flag))
                    _speech->put_voice(session_id, (uint8_t *)_event->buff, _event->length);
                break;
            case SIREN_EVENT_VAD_END:
                if(!_voice_config->cloud_vad_enable()){
                    LOGV("VAD_END\t\t ID  :   <<%d>> ", session_id);
                    if(session_id > 0) _speech->end_voice(session_id);
                    //_callback->voice_event(session_id, VoiceEvent::VOICE_END);
                    clear();
                }
                break;
            case SIREN_EVENT_VAD_CANCEL:
                LOGI("VAD_CANCEL\t\t ID  :   <<%d>> \t  %d", session_id, asr_finished);
                if(session_id > 0 && !asr_finished) _speech->cancel(session_id);
                asr_finished = false;
                if(!_voice_config->cloud_vad_enable()) clear();
                break;
            case SIREN_EVENT_VOICE_PRINT:
                voice_print(_event);
                break;
            case SIREN_EVENT_SLEEP:
                if(!_voice_config->cloud_vad_enable()){
                    _callback->voice_event(session_id, VoiceEvent::VOICE_SLEEP);
                    LOGV("SLEEP");
                }
                break;
        }
		delete[] (char *)_event;
    }
	_speech->release();
	_speech.reset();
	return NULL;
}

void* VoiceService::onResponse() {
	prctl(PR_SET_NAME, __FUNCTION__);
	auto arbitration = [](const string& activation)->bool {return ("fake" == activation || "reject" == activation);};
	SpeechResult sr;
    string activation, asr;
	json_object *nlp_obj, *activation_obj;
	while (true) {
		if (!_speech->poll(sr)) {
			break;
		}
        LOGV("result : id \t %d \t \t type \t %d \t err \t %d", sr.id, sr.type, sr.err);
        if(sr.type == SPEECH_RES_START) {
            asr_finished = false;
            activation.clear();
        } else if((sr.type == SPEECH_RES_INTER || sr.type == SPEECH_RES_END) && !sr.extra.empty()) {
            nlp_obj = json_tokener_parse(sr.extra.c_str());
            if(TRUE == json_object_object_get_ex(nlp_obj, "activation", &activation_obj)){
                activation = json_object_get_string(activation_obj);
                json_object_put(nlp_obj);
                LOGV("result : activ \t %s", activation.c_str());
                _callback->voice_event(sr.id, transform_string_to_event(activation));
                if(arbitration(activation)) {
                    set_siren_state(SIREN_STATE_SLEEP);
                }
            }
        }
        if(!arbitration(activation)) {
            if(sr.type == SPEECH_RES_INTER){// || sr.type == SPEECH_RES_ASR_FINISH){
                LOGV("result : asr\t%s", sr.asr.c_str());
                _callback->intermediate_result(sr.id, sr.type, sr.asr);
//                if(sr.type == SPEECH_RES_ASR_FINISH){
//                    if(session_id == sr.id || _voice_config->cloud_vad_enable()){
//                        set_siren_state(SIREN_STATE_SLEEP);
//                        asr_finished = true;
//                    }
//                    asr = sr.asr;
//                }
            }else if(sr.type == SPEECH_RES_END) {
                LOGV("result : nlp\t%s", sr.nlp.c_str());
                LOGV("result : action  %s", sr.action.c_str());
                _callback->voice_command(sr.id, asr, sr.nlp, sr.action);
            }else if(sr.type == SPEECH_RES_CANCELLED){
                _callback->voice_event(sr.id, VoiceEvent::VOICE_CANCEL);
            }else if(sr.type == SPEECH_RES_ERROR && (sr.err != SPEECH_SUCCESS)) {
                if(session_id == sr.id && _voice_config->cloud_vad_enable())
                    set_siren_state(SIREN_STATE_SLEEP);
                _callback->speech_error(sr.id, sr.err);
                asr_finished = false;
                activation.clear();
            }
        }
        if(sr.type >= SPEECH_RES_END) clear(sr.id);
	}
    clear();
	LOGV("exit !!");
	return NULL;
}

