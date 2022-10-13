#ifndef HTU21_H_
#define HTU21_H_
//------------------------------------------------
#include "stm32l1xx_hal.h"
#include <string.h>
#include <math.h>
//------------------------------------------------
#define HTU21_ADDRESS 0x40 //HTU21 I2C ADDRESS (0x76<<1)
#define HTU21_REG_ID 0xD0 //HTU21 ID REGISTER
#define HTU21_ID 0xE5 //HTU21 I2C ID
//------------------------------------------------
#endif /* HTU21_H_ */
