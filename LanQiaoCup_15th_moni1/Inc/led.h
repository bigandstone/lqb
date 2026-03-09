#ifndef __led_h
#define __led_h

#include "main.h"

typedef struct {
    uint8_t state;
}Led;

extern Led led;

void Led_Scan();

#endif
