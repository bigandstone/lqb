#ifndef __eeprom_h__
#define __eeprom_h__

#include "main.h"
#include "i2c_hal.h"

#define ADDRESS_W 0xA0
#define ADDRESS_R 0xA1

void EEPROM_Write( uint8_t address, uint8_t data);
uint8_t EEPROM_Read(uint8_t address);

#endif
