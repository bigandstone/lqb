#ifndef __key_h__
#define __key_h__

#include "main.h"
#include "gpio.h"

typedef enum{
    KEY_ON,
    KEY_DOWN,
}KeyState;

typedef enum{
    NONE,
    SHORT,
    LONG
}KeyFlag;

typedef struct {
    KeyState state;
    
    GPIO_TypeDef* port;
    uint16_t pin;
    
    uint32_t time_ms;
    KeyFlag flag;
}Key;

extern Key key1,key2,key3,key4;

void key_init();
void key_process();//1ms
KeyFlag key_get_flag(Key* key);


void key_process_single(Key* key);
void key_init_struct(Key* key, GPIO_TypeDef* port,uint16_t pin);

#endif

