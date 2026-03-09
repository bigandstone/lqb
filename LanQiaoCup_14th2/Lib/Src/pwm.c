#include "pwm.h"

Pwm pwm = { .freq = 4000 };

void Pwm_Updata(){
    if(!pwm.init) {
        pwm.init = 1;
        HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    }

    htim2.Instance->ARR = 1000000 / pwm.freq - 1;
    htim2.Instance->CCR2 = htim2.Instance->ARR * pwm.duty / 100;
}