#pragma once
#include <mbed.h>
#include "parameters.hpp"


//-----足回り------
#if USING_R1

//ロータリエンコーダー
#define RE_1_A (PB_4)
#define RE_1_B (PB_5)
#define RE_2_A (PA_1)
#define RE_2_B (PA_0)
#define RE_3_A (PA_11)
#define RE_3_B (PB_12)
#define RE_4_A (PC_10)
#define RE_4_B (PC_12)

//DCモーター
#define PWM_1 (PB_1)
#define DIR_1 (PC_9)
#define PWM_2 (PB_15)
#define DIR_2 (PC_8)
#define PWM_3 (PB_14)
#define DIR_3 (PC_6)
#define PWM_4 (PB_13)
#define DIR_4 (PB_2)


//-----アーム-----
//DCモーター
#define PWM_5 (PB_8)
#define DIR_5 (PC_4)

//サーボ
#define SV_1_SIGNAL (PA_7)
#define SV_2_SIGNAL (PA_8)

//リミットスイッチ
#define LS_1 (PC_11)
#define LS_2 (PD_2) 

//-----レーザー-----
#define SDA (PC_7)
#define SCL (PB_6)

#define XSHUT_1 (PC_5)
#define XSHUT_2 (PA_12)
#define XSHUT_3 (PA_6)
#define XSHUT_4 (PC_1)

#else


#endif