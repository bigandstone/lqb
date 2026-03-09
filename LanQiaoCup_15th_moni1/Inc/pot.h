#ifndef __pot_h
#define __pot_h

#include "main.h"

typedef struct {
    uint8_t val;
}Pot;

extern Pot pot;

void pot_Scan();

#endif
