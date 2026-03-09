#include "pulse.h"

Pulse pulse = {
    .K = 1,
    .R = 1,
};

void Pulse_Updata(){
    if(!pulse.init){
        HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
        pulse.init = 1;
    }
    if(htim3.Instance->CCR1){
        if((HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1) % 2) == 0)
            pulse.freq = (double)1000000 /( HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1)+1);
    }
    pulse.v = 2 * 3.14 * pulse.freq * pulse.R /(float)( 100 * pulse.K);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
    if(htim == &htim3 ){
        htim3.Instance->CNT = 0;
    }
}
