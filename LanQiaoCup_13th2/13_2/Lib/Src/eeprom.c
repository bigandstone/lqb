#include "eeprom.h"

void EEPROM_Write( uint8_t address,uint8_t data){
    I2CStart();
    I2CSendByte(ADDRESS_W);
    I2CWaitAck();
    
    I2CSendByte(address);
    I2CWaitAck();
    I2CSendByte(data);
    I2CWaitAck();
    
    I2CStop();
    HAL_Delay(1);
}

uint8_t EEPROM_Read(uint8_t address){
    I2CStart();
    I2CSendByte(ADDRESS_W);//write
    I2CWaitAck();
    I2CSendByte(address);//where
    I2CWaitAck();
    
    I2CStart();
    I2CSendByte(ADDRESS_R);//read
    I2CWaitAck();
    uint8_t data = I2CReceiveByte();
    I2CSendNotAck();
    I2CStop();
    return data;
}