#ifndef __pot_h__
#define __pot_h__

#include "main.h"
#include "adc.h"

typedef struct{
    float volt;
    uint8_t init;
}Pot;

extern Pot pot;

void Pot_Updata();

#endif
