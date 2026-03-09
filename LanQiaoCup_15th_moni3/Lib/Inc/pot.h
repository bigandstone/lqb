#ifndef __pot_h__
#define __pot_h__

#include "main.h"
#include "adc.h"

typedef struct {
    float val_1;
    float val_2;
}Pot;

extern Pot pot;

void POT_Updata();

#endif
