#ifndef __pulse_h__
#define __pulse_h__

#include "main.h"
#include "tim.h"

typedef struct{
    uint16_t freq;
    uint8_t R;
    uint8_t K;
}Pulse;

extern Pulse pulse;

void pulse_init();
void pulse_process();

#endif
