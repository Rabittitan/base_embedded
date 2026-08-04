#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
//#include "wiring_digital.cpp"
#include "Arduino.h"


// void i2c_init();

void i2c_start();

void i2c_stop();

uint8_t readByte(bool ack);
bool writeByte(uint8_t data); 
#endif