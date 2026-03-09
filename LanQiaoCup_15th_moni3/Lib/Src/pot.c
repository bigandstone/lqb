#include "pot.h"

Pot pot = {
    .val_1 = 0,
    .val_2 = 0
};

void POT_Updata(){
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start(&hadc2);
    
    if(HAL_OK == HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY)) {
        pot.val_1 = HAL_ADC_GetValue(&hadc1) * 3.3 / (float)4095;
    }
    if(HAL_OK == HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY)) {
        pot.val_2 = HAL_ADC_GetValue(&hadc2) * 3.3 / (float)4095;
    }
}