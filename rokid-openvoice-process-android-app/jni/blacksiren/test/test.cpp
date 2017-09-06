#include <thread>
#include <iostream>
#include <vector>
#include <fstream>

#include <fcntl.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <curl/curl.h>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "siren.h"
#include "sutils.h"
#include "lfqueue.h"
#include "siren_channel.h"

#include "mic_array.h"

int magic = 233;
std::ofstream recordingDebugStream;
std::ifstream test_recording_stream;
siren_t siren;
int init_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "init input stream");
    test_recording_stream.open(("/data/debug0.pcm"), std::ios::in | std::ios::binary);
    recordingDebugStream.open(("/data/test.pcm"), std::ios::out | std::ios::binary);
    return 0;
}

void release_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "release input stream");
}

int start_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "start input stream");
    return 0;
}

void stop_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "stop input stream");
}

bool stopRecording = false;
int read_input_stream(void *token,  char *buff, int len) {
    //siren_printf(BlackSiren::SIREN_INFO, "read input stream");
    test_recording_stream.read(buff, len);
    if (!test_recording_stream) {
        siren_printf(BlackSiren::SIREN_INFO, "read end of file");
        if (siren != 0) {
            std::this_thread::sleep_for(std::chrono::seconds(100));
            stop_siren_process_stream(siren);
            return 0;
        }
    }

    return 0;
}

void on_err_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "on err input stream");
}

const char *eventToString(siren_event_t event) {
    switch(event) {
    case SIREN_EVENT_VAD_START:
        return "vad_start";
    case SIREN_EVENT_VAD_DATA:
        return "vad_data";
    case SIREN_EVENT_VAD_END:
        return "vad_end";
    case SIREN_EVENT_VAD_CANCEL:
        return "vad_cancel";
    case SIREN_EVENT_WAKE_VAD_START:
        return "wake_vad_start";
    case SIREN_EVENT_WAKE_VAD_DATA:
        return "wake_vad_data";
    case SIREN_EVENT_WAKE_VAD_END:
        return "wake_vad_end";
    case SIREN_EVENT_WAKE_PRE:
        return "wake_pre";
    case SIREN_EVENT_WAKE_NOCMD:
        return "wake_nocmd";
    case SIREN_EVENT_WAKE_CMD:
        return "wake_cmd";
    case SIREN_EVENT_SLEEP:
        return "sleep";
    case SIREN_EVENT_HOTWORD:
        return "hotword";
    case SIREN_EVENT_SR:
        return "sr";
    case SIREN_EVENT_VOICE_PRINT:
        return "voice_print";
    case SIREN_EVENT_DIRTY:
        return "dirty";
    default: {
        siren_printf(BlackSiren::SIREN_WARNING, "unknown event %d", (int)event);
        return "unknown event";
    }
    }
}

static int dump_id = 0;
static int len_start = 0;
static int len_end = 0;
static bool vt_flag = false;
void debug_voice_event(voice_event_t *event) {
    if (HAS_VT(event->flag)) {
        siren_printf(BlackSiren::SIREN_INFO,
                "{%d} vt with %s [%d,%d]@%f", event->event, event->buff, 
                event->vt.start, event->vt.end, event->vt.energy);
        vt_flag = true;
        len_start = event->vt.start;
        len_end = event->vt.end;
    } else if (HAS_SL(event->flag)) {
        siren_printf(BlackSiren::SIREN_INFO,
                "sl with %f", event->sl); 
    } else if (HAS_VOICE(event->flag)) {
        if (vt_flag && event->event == SIREN_EVENT_VAD_DATA) {
            vt_flag = false;
            std::fstream output;
            char path[256];
            sprintf(path, ("/data/debug/dump%d.pcm"), dump_id);
            dump_id++;
            output.open(path, std::ios::out|std::ios::binary);
            char *p = (char *)event->buff;
            p += len_start * 4;
            int len = len_end - len_start;
            len *= 4;
            siren_printf(BlackSiren::SIREN_INFO, "dump offset %d and len %d", len_start, len);
            output.write(p, len);
            output.flush();
            output.close();

            len_start = 0;
            len_end = 0;
        }
    }
}

void on_voice_event(void *token, voice_event_t *event) {
    if (event != nullptr) {
        debug_voice_event(event);
    } else {
        siren_printf(BlackSiren::SIREN_ERROR, "on voice event with nullptr");
        return;
    }
}

void test_init() {
    siren_input_if_t input_callback;
    input_callback.init_input = init_input_stream;
    input_callback.release_input = release_input_stream;
    input_callback.start_input = start_input_stream;
    input_callback.stop_input = stop_input_stream;
    input_callback.on_err_input = on_err_input_stream;
    input_callback.read_input = read_input_stream;

    siren = init_siren(nullptr, "blacksiren.json", &input_callback);
    if (siren == 0) {
        siren_printf(BlackSiren::SIREN_INFO, "init siren failed");
        return;
    }

    destroy_siren(siren);
}


void test_recording() {
    siren_input_if_t input_callback;
    siren_proc_callback_t proc_callback;

    input_callback.init_input = init_input_stream;
    input_callback.release_input = release_input_stream;
    input_callback.start_input = start_input_stream;
    input_callback.stop_input = stop_input_stream;
    input_callback.on_err_input = on_err_input_stream;
    input_callback.read_input = read_input_stream;

    proc_callback.voice_event_callback = on_voice_event;
    siren = init_siren(nullptr, nullptr, &input_callback);
    if (siren == 0) {
        siren_printf(BlackSiren::SIREN_INFO, "init siren failed");
        return;
    }

    siren_printf(BlackSiren::SIREN_INFO, "start recording test");
    start_siren_process_stream(siren, &proc_callback);
    siren_printf(BlackSiren::SIREN_INFO, "go test");

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

//mic_array_module_t *module;
bool mic_open = false;
mic_array_device_t *mic_array_device;

int init_xmos_input_stream(void *token) {
    if (mic_array_device_open(&mic_array_device) != 0) {
        siren_printf(BlackSiren::SIREN_ERROR, "open mic failed");
        return -1;
    } else {
        mic_open = true;
    }
    
    //recordingDebugStream.open(PATH_ME("test.pcm"), std::ios::out | std::ios::binary);
    return 0;
}

void release_xmos_input_stream(void *token) {
}


static bool test_twice_start = false;
int start_xmos_input_stream(void *token) {
    //if (!test_twice_start) {
    //    test_twice_start = true;
    //    return -1;
    //}
    siren_printf(BlackSiren::SIREN_INFO, "start input stream");
    mic_array_device->start_stream(mic_array_device);
    siren_printf(BlackSiren::SIREN_INFO, "end of start input stream");
    return 0;
}

void stop_xmos_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "stop input stream");
    mic_array_device->stop_stream(mic_array_device);
}

void on_net_event(void *token, char *data, int len) {
    data[len - 1] = '\0';
    std::string t(data);
    siren_printf(BlackSiren::SIREN_INFO, "recv net event %s", t.c_str());
}

int read_xmos_input_stream(void *token,  char *buff, int len) {
    //mic_array_device->read_stream(mic_array_device, buff, (unsigned long long *)&len);
    //memset(buff, 0, len);
    unsigned long long size = 0;
    //buff[len/2] = 'a';
    //buff[len/2 + 1] = 'b';
    //buff[len/2 + 2] = 'c';
    //buff[len/2 + 3] = 'd';
    //siren_printf(BlackSiren::SIREN_INFO, "before read stream");
    mic_array_device->read_stream(mic_array_device, buff, len);
    recordingDebugStream.write(buff, len);
    //buff[len/2 + 4] = '\0';
    //std::cout<<buff + len/2<<std::endl;
    return 0;
}

void on_err_xmos_input_stream(void *token) {
    siren_printf(BlackSiren::SIREN_INFO, "on err input stream");
}

void on_siren_state_changed_fn(void *token, int current) {
    int *pm = (int *)token;
    siren_printf(BlackSiren::SIREN_INFO, "magic is %d", *pm);
    siren_printf(BlackSiren::SIREN_INFO, "set state to %d", current);
}

void test_xmos() {

    siren_input_if_t input_callback;
    siren_proc_callback_t proc_callback;
    siren_state_changed_callback_t state_changed_callback;
    siren_net_callback_t net_callback;
    state_changed_callback.state_changed_callback = on_siren_state_changed_fn;
    input_callback.init_input = init_xmos_input_stream;
    input_callback.release_input = release_xmos_input_stream;
    input_callback.start_input = start_xmos_input_stream;
    input_callback.stop_input = stop_xmos_input_stream;
    input_callback.on_err_input = on_err_xmos_input_stream;
    input_callback.read_input = read_xmos_input_stream;

    net_callback.net_event_callback = on_net_event;
    proc_callback.voice_event_callback = on_voice_event;
    siren = init_siren((void *)&magic, nullptr, &input_callback);
    if (siren == 0) {
        siren_printf(BlackSiren::SIREN_INFO, "init siren failed");
        return;
    }
    siren_printf(BlackSiren::SIREN_INFO, "start recording test");

    //start_siren_monitor(siren, &net_callback);
    siren_printf(BlackSiren::SIREN_INFO, "1111");
    start_siren_process_stream(siren, &proc_callback);
    siren_printf(BlackSiren::SIREN_INFO, "go test");

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    //set_siren_state(siren, SIREN_STATE_AWAKE, nullptr);
#if 0
    std::thread t([&] {
        //std::this_thread::sleep_for(std::chrono::seconds(10));
        //siren_printf(BlackSiren::SIREN_INFO, "test set state awake sync");
        //set_siren_state(siren, SIREN_STATE_AWAKE, nullptr);
        //siren_printf(BlackSiren::SIREN_INFO, "now state is in wake");
        //std::this_thread::sleep_for(std::chrono::seconds(10));
        //siren_printf(BlackSiren::SIREN_INFO, "stop");
        //stop_siren_process_stream(siren);
        //set_siren_state(siren, SIREN_STATE_SLEEP, &state_changed_callback);
        //std::this_thread::sleep_for(std::chrono::seconds(100000));
        siren_printf(BlackSiren::SIREN_INFO, "test twice start");
        start_siren_process_stream(siren, &proc_callback);
    });

    t.join();
#endif
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

}

void test_mic() {
    if (mic_array_device_open(&mic_array_device) != 0) {
        siren_printf(BlackSiren::SIREN_ERROR, "cannot open mic array");
        return;
    }

    //recordingDebugStream.open(PATH_ME("test.pcm"), std::ios::out | std::ios::binary);
    if (0 != mic_array_device->start_stream(mic_array_device)) {
        std::cout << "mic_array start stream failed" << std::endl;
    }
    int frameSize = 6 * 4 * 480;//mic_array_device->get_stream_buff_size(mic_array_device);
    siren_printf(BlackSiren::SIREN_INFO, "use frame cnt %d", frameSize);
    char *buff = new char[frameSize];
    for (;;) {
        int size = 0;
        mic_array_device->read_stream(mic_array_device, buff, frameSize);
        recordingDebugStream.write(buff, frameSize);
    }
}

std::string getAddressByHostname(const char *hostname) {
    std::string t;
    if (hostname == nullptr) {
        siren_printf(BlackSiren::SIREN_ERROR, "host name is null");
        return t;
    }

    hostent *record = gethostbyname(hostname);
    if (record == NULL) {
        siren_printf(BlackSiren::SIREN_ERROR, "cannot find hostname %s", hostname);
        return t;
    }

    in_addr *addr = (in_addr *)record->h_addr;
    t = inet_ntoa(* addr);

    return t;
}

void test_download() {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    FILE *f = fopen("/data/blacksiren/blacksiren.json", "w");
    curl = curl_easy_init();
    if (curl) {
        std::string ip = getAddressByHostname("config.open.rokid.com");
        if (ip.empty()) {
            siren_printf(BlackSiren::SIREN_ERROR, "cannot find address for host name config.open.rokid.com");
            return;
        } else {
            siren_printf(BlackSiren::SIREN_INFO, "address is %s", ip.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_URL, "https://config.open.rokid.com/openconfig/blacksiren.json");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)f);
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            siren_printf(BlackSiren::SIREN_ERROR, "perfrom curl failed with %s", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    } else {
        siren_printf(BlackSiren::SIREN_ERROR, "init curl failed");
        return;
    }

    curl_global_cleanup();
}

void test_send() {
    siren_input_if_t input_callback;
    siren_proc_callback_t proc_callback;
    siren_state_changed_callback_t state_changed_callback;
    state_changed_callback.state_changed_callback = on_siren_state_changed_fn;
    input_callback.init_input = init_xmos_input_stream;
    input_callback.release_input = release_xmos_input_stream;
    input_callback.start_input = start_xmos_input_stream;
    input_callback.stop_input = stop_xmos_input_stream;
    input_callback.on_err_input = on_err_xmos_input_stream;
    input_callback.read_input = read_xmos_input_stream;

    proc_callback.voice_event_callback = on_voice_event;
    siren = init_siren((void *)&magic, nullptr, &input_callback);
    if (siren == 0) {
        siren_printf(BlackSiren::SIREN_INFO, "init siren failed");
        return;
    }
    siren_printf(BlackSiren::SIREN_INFO, "start recording test");
    start_siren_process_stream(siren, &proc_callback);

    for (;;) {
        siren_printf(BlackSiren::SIREN_INFO, "send broadcast");
        if (SIREN_STATUS_OK != broadcast_siren_event(siren, "hello world", 13)) {
            siren_printf(BlackSiren::SIREN_ERROR, "send failed");
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void dump_vt_word(siren_vt_word &vt_word) {
    siren_printf(BlackSiren::SIREN_INFO, "word=%s,phone=%s", vt_word.vt_word.c_str(), vt_word.vt_phone.c_str());
}

void test_vt() {
    siren_input_if_t input_callback;
    input_callback.init_input = init_input_stream;
    input_callback.release_input = release_input_stream;
    input_callback.start_input = start_input_stream;
    input_callback.stop_input = stop_input_stream;
    input_callback.on_err_input = on_err_input_stream;
    input_callback.read_input = read_input_stream;

    siren_proc_callback_t proc_callback;
    proc_callback.voice_event_callback = on_voice_event;
    siren = init_siren(nullptr, "blacksiren.json", &input_callback);
    if (siren == 0) {
        siren_printf(BlackSiren::SIREN_INFO, "init siren failed");
        return;
    }

    start_siren_process_stream(siren, &proc_callback);
    
    siren_vt_word vt1, vt2, vt3;
    vt1.vt_phone = "ni2hao3";
    vt1.vt_type = VT_TYPE_AWAKE;
    vt1.use_default_config = true;
    vt1.vt_word = "你好";

    vt2.vt_phone = "sha3bi1";
    vt2.vt_type = VT_TYPE_HOTWORD;
    vt2.use_default_config = false;
    vt2.vt_word = "傻逼";
    vt2.alg_config.vt_block_avg_score = 1.0f;
    vt2.alg_config.vt_classify_shield = 1.1f;
    vt2.alg_config.vt_block_min_score = 2.0f;
    vt2.alg_config.vt_left_sil_det = false;
    vt2.alg_config.vt_right_sil_det = false;
    vt2.alg_config.vt_remote_check_with_aec = false;
    vt2.alg_config.vt_remote_check_without_aec = false;
    vt2.alg_config.vt_local_classify_check = false;
    vt2.alg_config.nnet_path = "/system/etc/hello";

    vt3.vt_phone = "hao3ren2";
    vt3.vt_type = VT_TYPE_SLEEP;
    vt3.use_default_config = true;
    vt3.vt_word = "好人";

    siren_vt_t result = add_vt_word(siren, &vt1, true);
    siren_printf(BlackSiren::SIREN_INFO, "add %s with result %d", vt1.vt_word.c_str(), result);
    std::this_thread::sleep_for(std::chrono::seconds(10));

    result = add_vt_word(siren, &vt2, true);
    siren_printf(BlackSiren::SIREN_INFO, "add %s with result %d", vt2.vt_word.c_str(), result);
    std::this_thread::sleep_for(std::chrono::seconds(10));
#if 0
    //test get
    siren_vt_word *words = nullptr;
    int num = get_vt_word(siren, &words);
    if (num > 0) {
        for (int i = 0; i < num; i++) {
            dump_vt_word(words[i]);
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(3));
    siren_printf(BlackSiren::SIREN_INFO, "after add...");
#endif
    result = add_vt_word(siren, &vt3, true);
    siren_printf(BlackSiren::SIREN_INFO, "add %s with result %d", vt3.vt_word.c_str(), result);
    std::this_thread::sleep_for(std::chrono::seconds(10));
#if 0
    num = get_vt_word(siren, &words);
    if (num > 0) {
        for (int i = 0; i < num; i++) {
            dump_vt_word(words[i]);
        }
    }
#endif
    siren_printf(BlackSiren::SIREN_INFO, "after remove...");
    result = remove_vt_word(siren, vt3.vt_word.c_str());
    std::this_thread::sleep_for(std::chrono::seconds(10));
#if 1
    siren_vt_word *words = nullptr;
    int num = get_vt_word(siren, &words);
    if (num > 0) {
        for (int i = 0; i < num; i++) {
            dump_vt_word(words[i]);
        }
    }
#endif
    //destroy_siren(siren);
    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(1000));
    }
}


int main(void) {
    //test_common();
    //test_channel();
    //test_thread_start();
    //test_thread_hardware_concurrency();
    //test_fork_socketpair();

    //test_init();
    //test_recording();
    test_xmos();
    //test_mic();
    //test_download();

    //test_send();

    //test_vt();
}
