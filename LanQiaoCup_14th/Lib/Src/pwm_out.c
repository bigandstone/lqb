#include "pwm_out.h"

Pwm pwm1 = {
    .mode = PWM_LOW,
    
    .freq = LOW_FREQ,
    .pulse = 0,
    
    .mode_cnt = 0,
    .change_flag = 0,
};

void pwm_init(){
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    pwm_updata();
}

void pwm_process(){
    static uint8_t time = 0;
    if(++time < 10) return;
    time = 0;
    switch (pwm1.mode)
    {
    	case PWM_LOW:
            pwm1.freq = LOW_FREQ;
            if(pwm1.change_flag){
                pwm1.mode = PWM_TO_HIGH;
            }
    		break;
    	case PWM_HIGH:
            pwm1.freq = HIGH_FREQ;
            if(pwm1.change_flag){
                pwm1.mode = PWM_TO_LOW;
            }
    		break;
    	case PWM_TO_LOW:
            //10ms one change, all is 4000hz to change in 5s,so every change is 8hz
            pwm1.freq -= 8;
            if(pwm1.freq <= LOW_FREQ){
                pwm1.mode = PWM_LOW;
                pwm1.mode_cnt ++;
                pwm1.change_flag = 0;
            }
            break;
        case PWM_TO_HIGH:
            pwm1.freq += 8;
            if(pwm1.freq >= HIGH_FREQ){
                pwm1.mode = PWM_HIGH;
                pwm1.mode_cnt ++;
                pwm1.change_flag = 0;
            }
            break;
    }
    pwm_updata();
}

void pwm_change_mode(){
    if(!pwm1.change_flag)
    pwm1.change_flag = 1;
}

void pwm_updata(){
    pwm_updata_pulse();
    pwm_updata_freq();
}

void pwm_updata_pulse(){
    TIM3->CCR2 = TIM3->ARR * pwm1.pulse / 100;
}

void pwm_updata_freq(){
    TIM3->ARR = 1000000 / pwm1.freq;
}