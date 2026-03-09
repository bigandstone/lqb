#ifndef __led_h__
#define __led_h__

#include "main.h"

typedef struct {
    uint8_t state;
}Led;

extern Led led;

void Led_Set();

#endif
