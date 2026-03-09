#ifndef __key_h__
#define __key_h__

#include "main.h"

typedef struct {
    uint8_t state;
    uint8_t last_state;
    
    uint8_t flag[4];
    
    uint16_t timer;
}Key;

extern Key key;

void KEY_Scan();
void KEY_GetFlag(uint8_t* arr);

#endif
