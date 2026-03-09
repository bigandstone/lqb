#ifndef __analog_h__
#define __analog_h__

#include "main.h"
#include "adc.h"

typedef struct{
    uint32_t val;
}Pot;

extern Pot pot;

void pot_init();

#endif
