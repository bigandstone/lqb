#include "led.h"

Led led = {
    .state = 0xFF
};

void led_init(){
    led_show();
}

void led_set(uint8_t LD, uint8_t status){
    if(status == 0) led.state  &= ~(0x01 << (LD-1));
    else led.state  |= (0x01 << (LD-1));
    
    led_show();
}

void led_show(){
    led_unlatch();

    for(uint8_t i = 0; i < 8; i++ ){
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8 << i, (led.state & (0x01 << i)) == 0? GPIO_PIN_RESET:GPIO_PIN_SET);
    }
    
    led_latch();
}

void led_latch(){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}
void led_unlatch(){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
}