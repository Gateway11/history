//
//  main.cpp
//  vsys
//
//  Created by 薯条 on 2017/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//
#define FRAME_SIZE 160

#include <iostream>
#include <fstream>

#include "aec.h"
#include "bf.h"
#include "vad.h"
#include "zvbvapi.h"
#include "rdc.h"

vsys_float_t mic_pos[] = {
    0.0425000000,0.0000000000,0.0,
    0.0300520382,0.0300520382,0.0,
    0.0000000000,0.0425000000,0.0,
    0.0300520382,0.0300520382,0.0,
    0.0425000000,0.0000000000,0.0,
    0.0300520382,0.0300520382,0.0,
    0.0000000000,0.0425000000,0.0,
    0.0300520382,0.0300520382,0.0
};
audio_format_t audio_format8 = {16000, 32, 8};
audio_format_t audio_format1 = {16000, 32, 1};
audio_format_t audio_format_aec = {16000, 32, 6};
vsys_uint_t mic_id1[] = {0};
vsys_uint_t mic_id8[] = {0,1,2,3,4,5,6,7};
vsys_uint_t mic_id_aec[] = {0,1,2,3,4,5};
vsys_uint_t mic_id_aec_ref[] = {6,7};
vsys_float_t mic_delay[] = {0.000000, 0.0000000,0.0,0.0,0.0,0.0,0.0,0.0};

vsys_uint_t l1, l2;
vsys_char_t buffer1[FRAME_SIZE * sizeof(vsys_float_t)];
vsys_char_t buffer8[FRAME_SIZE * 8 * sizeof(vsys_float_t)];
vsys_float_t* p;
vsys_float_t** p6;
vsys_float_t* p6_1 = new vsys_float_t[FRAME_SIZE * 6];
vsys_float_t ** p1 = new vsys_float_t*[1];
vsys_float_t ** p8 = new vsys_float_t*[8];
std::ifstream pcm_in(
                     "/Users/daixiang/Desktop/vsys/data/sounds/baomao_M_0020.wav.f32.pcm",    //vad/bf
//                     "/Users/daixiang/Desktop/vsys/debug1.pcm",
                     std::ios::in | std::ios::binary);
std::ofstream pcm_out(
//                      "/Users/daixiang/Desktop/vsys/data/sounds/pcm_out_vsys.pcm",            //vad
                      "/Users/daixiang/Desktop/vsys/pcm_out.pcm",
                      std::ios::out | std::ios::binary);

__vsys__::Rdc *__rdc = new __vsys__::Rdc(audio_format1, mic_id1, VSYS_NULL, 16000);
__vsys__::Aec *__aec = new __vsys__::Aec(audio_format_aec, mic_id_aec, 2, mic_id_aec_ref, 64.0f);
__vsys__::Bf *__bf = new __vsys__::Bf(audio_format8, mic_id1, mic_pos, mic_delay);
__vsys__::Vad *__vad = new __vsys__::Vad(audio_format1, VSYS_TRUE);

void test_vad(){
    for (int i = 0; i < 8; i++) {
        p8[i] = new vsys_float_t[FRAME_SIZE];
    }
    vsys_bool_t vad_flag = false;
    while(pcm_in.good()){
        pcm_in.read((char *)buffer8, FRAME_SIZE * sizeof(vsys_float_t) * 8);
        for(int i = 0; i < 8; i++){
            for (int j = 0; j < FRAME_SIZE; j++) {
                p8[i][j] = ((float *)buffer8)[j * 8 + i] * 32768.0f;
            }
        }
        __bf->process((const vsys_float_t **)p8, FRAME_SIZE, p, l2);
        int ret = __vad->process(p, FRAME_SIZE, false, p, l2);
        std::cout << ret;
        if(VAD_ON(ret))vad_flag = true;
        if(vad_flag){
            for (vsys_uint_t i = 0; i < l2; i++) {
                p[i] /= 32768;
            }
            pcm_out.write((const char *)p, l2 * sizeof(vsys_float_t));
        }
    }
}

void test_vbv(){
    WordInfo * pWordLst = new WordInfo[1] ;
    pWordLst[0].iWordType = WORD_AWAKE ;
    strcpy(pWordLst[0].pWordContent_UTF8, "若琪") ;
    strcpy(pWordLst[0].pWordContent_PHONE, "r|l|r_B|l_B|# w o4|o4_E|## q|q_B|# i2|i2_E|##");
    pWordLst[0].fBlockAvgScore = 4.2 ;
    pWordLst[0].fBlockMinScore = 2.7 ;
    pWordLst[0].bLeftSilDet = false ;
    pWordLst[0].bRightSilDet = false ;
    pWordLst[0].bRemoteAsrCheckWithAec = false ;
    pWordLst[0].bRemoteAsrCheckWithNoAec = false ;
    pWordLst[0].bLocalClassifyCheck = true ;
    pWordLst[0].fClassifyShield = -0.3f ;
    strcpy(pWordLst[0].pLocalClassifyNnetPath, "/Users/daixiang/external/thirdlib/workdir_cn/final.ruoqi.mod");
    
    r2_vbv_htask vbv_handler = r2_vbv_create(8, mic_pos, mic_delay,
                  "/Users/daixiang/external/thirdlib/workdir_cn/final.svd.mod",
                  "/Users/daixiang/external/thirdlib/workdir_cn/phonetable");
    r2_vbv_setwords(vbv_handler, pWordLst, 1);
    for (int i = 0; i < 8; i++) {
        p8[i] = new vsys_float_t[FRAME_SIZE];
    }
    while (pcm_in.good()) {
        pcm_in.read((char *)buffer8, FRAME_SIZE * 8 * 4);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < FRAME_SIZE; j++) {
                p8[i][j] = ((vsys_float_t *)buffer8)[i + j * 8] * 32768.0f;
            }
        }
        int ret = r2_vbv_process(vbv_handler, (const vsys_float_t **)p8, FRAME_SIZE, 1, true);
    }
}

void test_rdc(){
    while (pcm_in.good()) {
        l1 = FRAME_SIZE;
        pcm_in.read((char *)buffer1, FRAME_SIZE * sizeof(vsys_float_t));
        p1[0] = (vsys_float_t *)buffer1;
        __rdc->process(p1, l1);
        for (vsys_uint_t i = 0; i < l1; i++) {
            p1[0][i] /= 32768;
        }
        pcm_out.write((char *)p1[0], l1 * sizeof(float));
    }
}

void test_aec(){
    for (int i = 0 ; i < 8; i++) {
        p8[i] = new vsys_float_t[FRAME_SIZE];
    }
    while (pcm_in.good()) {
        pcm_in.read((char *)buffer8, FRAME_SIZE * 8 * 4);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < FRAME_SIZE; j++) {
                p8[i][j] = ((vsys_float_t *)buffer8)[i + j * 8] * 32768.0f;
            }
        }
        __aec->process((const vsys_float_t **)p8, FRAME_SIZE, p6, l1);
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < FRAME_SIZE; j++) {
                p6_1[i + j * 6] = p6[i][j] / 2048;
            }
        }
        pcm_out.write((const char *)p6_1, FRAME_SIZE * 6 * sizeof(float));
    }
    printf("\n");
}

int main(int argc, const char * argv[]) {
//    test_rdc();
    test_aec();
//    test_vbv();
//    test_vad();
    return 0;
}
