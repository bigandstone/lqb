#include "analog_in.h"
Pot pot = {
    .val = 0,
};

void pot_init(){
    HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
    HAL_ADC_Start_IT(&hadc2);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
    if(hadc == &hadc2){
        pot.val = HAL_ADC_GetValue(&hadc2);
    }
}