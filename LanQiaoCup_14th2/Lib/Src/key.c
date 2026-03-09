#include "key.h"

static uint8_t key_flag[4] = {0,0,0,0};

Key key = {
    .now = 0,
    .last = 0,
    .flag = key_flag,
    .timer = 0
};

void Key_Scan(){
    key.now = 0;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) key.now = 1;
    else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET) key.now = 2;
    else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET) key.now = 3;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) key.now = 4;

    uint8_t down = key.now & (key.now ^ key.last);
    uint8_t up = ~key.now & (key.now ^ key.last);
    
    if(down) key.timer = 0;
    else if(up) {
        if(key.timer > 2000) {
            key.flag[key.last - 1] = 2;
        }else if(key.timer > 10) {
            key.flag[key.last - 1] = 1;
        }
    }
    key.last = key.now;
}

uint8_t Key_GetFlag(uint8_t keyx) {
    uint8_t flag = key.flag[keyx - 1];
    key.flag[keyx - 1] = 0;
    return flag;
}
