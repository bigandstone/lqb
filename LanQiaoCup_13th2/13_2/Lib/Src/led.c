#include "led.h"

Led led = {0};

void LED_Updata(){
    GPIOC->ODR = ~(led.state << 8);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
}