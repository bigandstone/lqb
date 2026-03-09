#ifndef __key_h
#define __key_h

#include "main.h"

typedef struct {
    uint8_t state;
    uint8_t last_state;
    
    uint8_t flag;
    
    uint32_t timer;
}Key;

extern Key key;

void KEY_Scan();
uint8_t KEY_GetFlag();

#endif
