#ifndef __key_h__
#define __key_h__

#include "main.h"

typedef struct {
    uint8_t now;
    uint8_t last;
    uint8_t* flag;//0 == NONE, 1 == SHORT, 2 == LONG
    
    uint16_t timer;
}Key;

extern Key key;

void Key_Scan();
uint8_t Key_GetFlag(uint8_t keyx);

#endif
