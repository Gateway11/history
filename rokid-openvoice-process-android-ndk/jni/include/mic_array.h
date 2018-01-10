/*************************************************************************
	> File Name: mic_array.h
	> Author: 
	> Mail: 
	> Created Time: Mon May  4 14:19:38 2015
 ************************************************************************/

#ifndef ANDROID_MIC_ARRAY_H
#define ANDROID_MIC_ARRAY_H

#include <stdint.h>

__BEGIN_DECLS

#define MIC_ARRAY_HARDWARE_MODULE_ID "mic_array"

struct mic_array_device_t {

    struct pcm *pcm;
    unsigned int frame_cnt;
    unsigned int channels;
    unsigned int period_size;
    unsigned int period_count;
    unsigned int sample_rate;
    unsigned int bit;

    int (*get_stream_buff_size) (struct mic_array_device_t *dev);
    int (*start_stream) (struct mic_array_device_t *dev);
    int (*stop_stream) (struct mic_array_device_t *dev);
    int (*finish_stream) (struct mic_array_device_t * dev);
    int (*resume_stream) (struct mic_array_device_t *dev);
    int (*read_stream) (struct mic_array_device_t *dev, char *buff, unsigned int frame_cnt);
    int (*config_stream) (struct mic_array_device_t *dev, int cmd, char *cmd_buff);
    int (*find_card) (const char *snd);
};

int mic_array_device_open(struct mic_array_device_t **);

__END_DECLS
#endif
