#include "pot.h"

Pot pot = {
    .volt = 0,
    .init = 0
};

void Pot_Updata(){
    if(!pot.init){
        HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
        pot.init ^= 1;
    }
    HAL_ADC_Start(&hadc2);
    if(HAL_OK == HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY))
    pot.volt =  HAL_ADC_GetValue(&hadc2) * 3.3 / 4095;
}
