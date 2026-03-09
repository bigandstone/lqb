#include "pulse.h"

Pulse pulse = {
    .K = 1,
    .R = 1,
};

static uint32_t tim2_cnt;
void pulse_init(){
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim2){
        tim2_cnt = TIM2->CNT;
        TIM2->CNT = 0;
    }
}

void pulse_process(){
    static uint8_t time  = 0;
    if(++time < 100) return;
    else time = 0;
    pulse.freq = 1000000 / tim2_cnt;
}