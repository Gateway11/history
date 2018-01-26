//
//  audio_format.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_audio_format__
#define __vsys_audio_format__

#include "typedefs.h"

enum {
    AUDIO_FORMAT_PCM_8_BIT           = 0x1,
    AUDIO_FORMAT_PCM_16_BIT          = 0x2,
    AUDIO_FORMAT_PCM_24_BIT          = 0x3,
    AUDIO_FORMAT_PCM_32_BIT          = 0x4,
    AUDIO_FORMAT_PCM_32_10_BIT       = 0x5,
    AUDIO_FORMAT_PCM_32_FLOAT        = 0x6,
};

enum {
    AUDIO_SAMPLT_RATE_16000          = 0x1,
    AUDIO_SAMPLT_RATE_48000          = 0x2,
    AUDIO_SAMPLT_RATE_96000          = 0x3,
};

typedef struct {
    vsys_uint_t sampleRateHz;
    vsys_uint_t sampleSizeInBits;
    vsys_uint_t numChannels;
}audio_format_t;

static inline vsys_bool_t audio_is_valid_sample_rate(vsys_uint_t sample_rate){
    switch (sample_rate) {
        case AUDIO_SAMPLT_RATE_16000:
        case AUDIO_SAMPLT_RATE_48000:
        case AUDIO_SAMPLT_RATE_96000:
            return VSYS_TRUE;
    }
    return VSYS_FALSE;
}

static inline vsys_bool_t audio_is_valid_format(vsys_uint_t format){
    switch (format) {
        case AUDIO_FORMAT_PCM_8_BIT:
        case AUDIO_FORMAT_PCM_16_BIT:
        case AUDIO_FORMAT_PCM_24_BIT:
        case AUDIO_FORMAT_PCM_32_BIT:
        case AUDIO_FORMAT_PCM_32_10_BIT:
        case AUDIO_FORMAT_PCM_32_FLOAT:
            return VSYS_TRUE;
    }
    return VSYS_FALSE;
}

static inline vsys_uint_t audio_format_to_bits(vsys_uint_t format){
    switch (format) {
        case AUDIO_FORMAT_PCM_8_BIT:
            return 8;
        case AUDIO_FORMAT_PCM_16_BIT:
            return 16;
        case AUDIO_FORMAT_PCM_24_BIT:
            return 24;
        default:
            return 32;
    }
}

static inline vsys_uint_t audio_bytes_per_sample(vsys_uint_t format){
    switch (format) {
        case AUDIO_FORMAT_PCM_8_BIT:
            return 1;
        case AUDIO_FORMAT_PCM_16_BIT:
            return 2;
        case AUDIO_FORMAT_PCM_24_BIT:
            return 3;
        default:
            return 4;
    }
}
#endif /* __vsys_audio_format__ */
