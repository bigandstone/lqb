#ifndef __pulse_h__
#define __pulse_h__

#include "main.h"
#include "tim.h"

typedef struct {
    uint8_t init;
    
    double freq;
    double v;
    uint8_t R;
    uint8_t K;
}Pulse;

extern Pulse pulse ;

void Pulse_Updata();

#endif
