#include <stdio.h>
#include <sys/prctl.h>
#include <functional>

#include "VoiceService.h"
#include "audio_recorder.h"
#include "json.h"

VoiceService::VoiceService() {
	pthread_mutex_init(&event_mutex, NULL);
	pthread_mutex_init(&speech_mutex, NULL);
	pthread_mutex_init(&siren_mutex, NULL);
	pthread_cond_init(&event_cond, NULL);
}

bool VoiceService::setup() {
	pthread_mutex_lock(&siren_mutex);
	if (mCurrentSirenState == SIREN_STATE_UNKNOWN) {
		if (!init(this,
				[](void *token, voice_event_t *event) {((VoiceService*)token)->voice_event_callback(event);})) {
			LOGE("init siren failed.");
			pthread_mutex_unlock(&siren_mutex);
			return false;
		}
	} else {
		goto done;
	}
	mCurrentSirenState = SIREN_STATE_INITED;
	if (!_speech.get()) _speech = new_speech();
	pthread_create(&event_thread, NULL,
			[](void* token)->void* {return ((VoiceService*)token)->onEvent();},
			this);

done: 
    pthread_mutex_unlock(&siren_mutex);
	return true;
}

void VoiceService::start_siren(bool isopen) {
	LOGV("%s \t flag : %d \t mCurrState : %d \t opensiren : %d",
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
	if (!isopen && mCurrentSirenState != SIREN_STATE_UNKNOWN)
		openSiren = false;
	pthread_mutex_unlock(&siren_mutex);
}

void VoiceService::set_siren_state(const int state) {
	set_siren_state_change(state);
	LOGV("current_status     >>   %d", state);
}

void VoiceService::network_state_change(bool connected) {
	LOGV("network_state_change      isconnect  <<%d>>", connected);
	pthread_mutex_lock(&speech_mutex);
	if (!_speech.get())
		_speech = new_speech();
	if (connected && mCurrentSpeechState != SPEECH_STATE_PREPARED) {
		this->config();
		if (_speech->prepare()) {
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
	LOGE("appid  %s", this->appid.c_str());
}

void VoiceService::regist_callback(const void* callback) {
	this->callback = make_shared < VoiceCallback > (callback);
}

int VoiceService::vad_start() {
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
	//add to siren_queue
	voice_event_t *voice_message(new voice_event_t);
	memcpy(voice_message, voice_event, sizeof(voice_event_t));
	void *buff = NULL;
	if (HAS_VOICE(voice_message->flag) || HAS_VT(voice_message->flag)) {
		buff = new char(voice_event->length);
		memcpy(buff, voice_event->buff, voice_event->length);
		voice_message->buff = buff;
	}
	message_queue.push_back(voice_message);
	pthread_cond_signal(&event_cond);
	pthread_mutex_unlock(&event_mutex);
}

void VoiceService::config() {
	json_object *json_obj = json_object_from_file(OPENVOICE_PREFILE);

	if (json_obj == NULL) {
		LOGE("%s cannot find", OPENVOICE_PREFILE);
		return;
	}
	json_object *host, *port, *branch, *ssl_roots_pem, *auth_key, *device_type,
			*device_id, *secret, *api_version;

	if (TRUE == json_object_object_get_ex(json_obj, "host", &host)) {
		_speech->config("host", json_object_get_string(host));
		LOGE("%s", json_object_get_string(host));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "port", &port)) {
		_speech->config("port", json_object_get_string(port));
		LOGE("%s", json_object_get_string(port));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "branch", &branch)) {
		_speech->config("branch", json_object_get_string(branch));
		LOGE("%s", json_object_get_string(branch));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "ssl_roots_pem",&ssl_roots_pem)) {
		_speech->config("ssl_roots_pem", json_object_get_string(ssl_roots_pem));
		LOGE("%s", json_object_get_string(ssl_roots_pem));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "key", &auth_key)) {
		_speech->config("key", json_object_get_string(auth_key));
		LOGE("%s", json_object_get_string(auth_key));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "device_type_id", &device_type)) {
		_speech->config("device_type_id", json_object_get_string(device_type));
		LOGE("%s", json_object_get_string(device_type));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "device_id", &device_id)) {
		_speech->config("device_id", json_object_get_string(device_id));
		LOGE("%s", json_object_get_string(device_id));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "api_version", &api_version)) {
		_speech->config("api_version", json_object_get_string(api_version));
		LOGE("%s", json_object_get_string(api_version));
	}
	if (TRUE == json_object_object_get_ex(json_obj, "secret", &secret)) {
		_speech->config("secret", json_object_get_string(secret));
		LOGE("%s", json_object_get_string(secret));
	}
	_speech->config("codec", "opu");
	json_object_put(json_obj);
}

void* VoiceService::onEvent() {
	prctl(PR_SET_NAME, __FUNCTION__);
	int id = -1;
	while (true) {
		pthread_mutex_lock(&event_mutex);
		while (message_queue.empty()) {
			pthread_cond_wait(&event_cond, &event_mutex);
		}
		voice_event_t *message = message_queue.front();
		message_queue.pop_front();
		pthread_mutex_unlock(&event_mutex);

		LOGV("event : -------------------------%d----", message->event);

		if (callback.get()) {
			callback->voice_event(message->event, HAS_SL(message->flag),
					message->sl, message->background_energy,
					message->background_threshold);
		}

		switch (message->event) {
		case SIREN_EVENT_WAKE_CMD:
			LOGV("WAKE_CMD");
			break;
		case SIREN_EVENT_SLEEP:
			LOGV("SLEEP");
			break;
		case SIREN_EVENT_VAD_START:
			id = vad_start();
			LOGV("VAD_START\t\t ID  :  <<%d>>", id);
			break;
		case SIREN_EVENT_VAD_DATA:
			if (id > 0 && HAS_VOICE(message->flag)) {
				_speech->put_voice(id, (uint8_t *) message->buff, message->length);
			}
			break;
		case SIREN_EVENT_VAD_END:
			LOGV("VAD_END\t\t ID  <<%d>> ", id);
			if (id > 0) {
				_speech->end_voice(id);
				id = -1;
			}
			break;
		case SIREN_EVENT_VAD_CANCEL:
			if (id > 0) {
				_speech->cancel(id);
				LOGI("VAD_CANCEL\t\t ID   <<%d>>", id);
				id = -1;
			}
			break;
		case SIREN_EVENT_VOICE_PRINT:
			voice_print(message);
			LOGI("VOICE_PRINT");
			break;
		}
		delete[] (char*) message->buff;
		delete message;
	}
	_speech->release();
	_speech.reset();
	return NULL;
}

void* VoiceService::onResponse() {
	prctl(PR_SET_NAME, __FUNCTION__);
	auto arbitration = [](const string& activation)->bool {return ("fake" == activation || "reject" == activation);};
	SpeechResult sr;
	string activation;
	json_object *nlp_obj, *activation_obj;
	while (1) {
		if (!_speech->poll(sr)) {
			break;
		}
		LOGV("result : type \t %d \t err \t %d \t id \t %d", sr.type, sr.err, sr.id);
		if (sr.type == SPEECH_RES_START) {
			activation.clear();
		} else if ((sr.type == SPEECH_RES_INTER || sr.type == SPEECH_RES_END) && !sr.extra.empty()) {
			nlp_obj = json_tokener_parse(sr.extra.c_str());
			if (TRUE == json_object_object_get_ex(nlp_obj, "activation", &activation_obj)) {
				activation = json_object_get_string(activation_obj);
				json_object_put(nlp_obj);
				LOGV("result : activation %s", activation.c_str());
				if (callback.get()) {
					callback->arbitration(activation);
				}
				if (arbitration(activation)) {
					set_siren_state_change(SIREN_STATE_SLEEP);
					continue;
				}
			}
		}
		if (!arbitration(activation)) {
			if (sr.type == SPEECH_RES_END) {
				LOGV("result : asr\t%s", sr.asr.c_str());
				LOGV("result : nlp\t%s", sr.nlp.c_str());
				LOGV("result : action  %s", sr.action.c_str());
				if (callback.get()) {
					callback->voice_command(sr.asr, sr.nlp, sr.action);
				} else {
					LOGI("Java service is null , Waiting for it to initialize");
				}
			} else if (sr.type == SPEECH_RES_ERROR
							&& (sr.err == SPEECH_TIMEOUT
							|| sr.err == SPEECH_SERVER_INTERNAL)) {
				if (callback.get()) {
					callback->speech_error(sr.err);
				}
			}
		}
	}
	LOGV("exit !!");
	return NULL;
}

