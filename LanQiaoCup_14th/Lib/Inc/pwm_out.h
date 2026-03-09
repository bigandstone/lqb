#ifndef __pwm_out_h__
#define __pwm_out_h__

#include "main.h"
#include "tim.h"

#define HIGH_FREQ   8000
#define LOW_FREQ    4000


typedef enum {
    PWM_LOW,
    PWM_HIGH,
    PWM_TO_LOW,
    PWM_TO_HIGH,
}PwmMode;

typedef struct{
    PwmMode mode ;
    
    uint16_t freq;
    uint16_t pulse;//0~100
    
    uint16_t mode_cnt;//mode changed cnt
    uint8_t change_flag;
}Pwm;

extern Pwm pwm1;

void pwm_init();
void pwm_process();
void pwm_change_mode();

void pwm_updata();
void pwm_updata_pulse();
void pwm_updata_freq();

#endif
