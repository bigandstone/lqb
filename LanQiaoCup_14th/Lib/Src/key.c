#include "key.h"
Key key1,key2,key3,key4;

void key_init(){
    key_init_struct(&key1, GPIOB, GPIO_PIN_0);
    key_init_struct(&key2, GPIOB, GPIO_PIN_1);
    key_init_struct(&key3, GPIOB, GPIO_PIN_2);
    key_init_struct(&key4, GPIOA, GPIO_PIN_0);
}

void key_process(){
    key_process_single(&key1);
    key_process_single(&key2);
    key_process_single(&key3);
    key_process_single(&key4);
}

void key_process_single(Key* key){
    switch (key->state)
    {
    	case KEY_ON:
            if(HAL_GPIO_ReadPin(key->port, key->pin) == GPIO_PIN_RESET){
                key->time_ms = 0;
                key->state = KEY_DOWN;
            }
    		break;
    	case KEY_DOWN:
            if(HAL_GPIO_ReadPin(key->port, key->pin) == GPIO_PIN_RESET){
                key->time_ms ++;
                break;
            }
            key->state = KEY_ON;
            if(key->time_ms < 10){
                key->flag = NONE;
                break;
            }
            if(key->time_ms > 2000){
                key->flag = LONG;
                break;
            }
            key->flag = SHORT;
    		break;
    }
}
KeyFlag key_get_flag(Key* key){
    KeyFlag flag = key->flag;
    key->flag = NONE;
    return flag;
}

void key_init_struct(Key* key, GPIO_TypeDef* port,uint16_t pin){
    key->port = port;
    key->pin = pin;
    key->flag = NONE;
    key->state = KEY_ON;
    key->time_ms = 0;
}