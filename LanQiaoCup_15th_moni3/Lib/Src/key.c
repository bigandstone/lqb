#include "key.h"

Key key = {
    .state = 0,
    .last_state = 0,
    .flag  = 0,
    .timer = 0
};

void KEY_Updata(){
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET) key.state = 1;
    else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET) key.state = 2;
    else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET) key.state = 3;
    else if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) key.state = 4;
    else key.state = 0;
    
    if(key.last_state == 0 && key.state) {
        key.timer = 0;
    }
    
    if(key.last_state && key.state == 0) {
        if(key.timer > 10) key.flag = key.last_state;
    }
    
    key.last_state = key.state;
}
uint8_t KEY_GetFlag(){
    uint8_t key_flag = key.flag;
    key.flag = 0;
    return key_flag;
}