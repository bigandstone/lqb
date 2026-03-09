#ifndef __pwm_h__
#define __pwm_h__

#include "main.h"
#include "tim.h"

typedef struct {
    uint8_t init;
    
    uint16_t freq;
    uint8_t duty;
}Pwm;

extern Pwm pwm;

void Pwm_Updata();

#endif
