#ifndef __led_h__
#define __led_h__

#include "main.h"
#include "gpio.h"

typedef struct{
    uint8_t state;
}Led;

extern Led led;

void led_init();
void led_set(uint8_t LD, uint8_t status);
void led_show();

void led_latch();
void led_unlatch();

#endif
