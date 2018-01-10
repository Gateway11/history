#include "VoiceCallback.h"
#include "json.h"

void VoiceCallback::voice_event(const int32_t id, const int32_t event, const double sl, const double energy){
    json_object *root = json_object_new_object();
    json_object_object_add(root, "id", json_object_new_int(id));
    json_object_object_add(root, "event", json_object_new_int(event));
    json_object_object_add(root, "sl", json_object_new_double(sl));
    json_object_object_add(root, "energy", json_object_new_double(energy));
    std::string strJson = json_object_to_json_string(root);
    json_object_put(root);
    send_message(MSG_VOICE_EVENT_ID, strJson);
}

void VoiceCallback::intermediate_result(const int32_t id, const int32_t type, const string& asr){
    json_object *root = json_object_new_object();
    json_object_object_add(root, "id", json_object_new_int(id));
    json_object_object_add(root, "type", json_object_new_int(type));
    json_object_object_add(root, "asr", json_object_new_string(asr.c_str()));
    std::string strJson = json_object_to_json_string(root);
    json_object_put(root);
    send_message(MSG_INTERMEDIATE_RESULT_ID, strJson);
}

void VoiceCallback::voice_command(const int32_t id, const string& asr, const string& nlp, const string& action){
    json_object *root = json_object_new_object();
    json_object_object_add(root, "id", json_object_new_int(id));
    json_object_object_add(root, "asr", json_object_new_string(asr.c_str()));
    json_object_object_add(root, "nlp", json_object_new_string(nlp.c_str()));
    json_object_object_add(root, "action", json_object_new_string(action.c_str()));
    std::string strJson = json_object_to_json_string(root);
    json_object_put(root);
    send_message(MSG_VOICE_COMMAND_ID, strJson);
}

void VoiceCallback::speech_error(const int32_t id, const int32_t errcode){
    json_object *root = json_object_new_object();
    json_object_object_add(root, "id", json_object_new_int(id));
    json_object_object_add(root, "errcode", json_object_new_int(errcode));
    std::string strJson = json_object_to_json_string(root);
    json_object_put(root);
    send_message(MSG_SPEECH_ERROR_ID, strJson);
}

