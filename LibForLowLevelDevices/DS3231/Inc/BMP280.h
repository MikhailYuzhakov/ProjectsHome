#ifndef BMP280_H_
#define BMP280_H_
//------------------------------------------------
#include "stm32l1xx_hal.h"
#include "lcd.h"
#include <string.h>
#include <math.h>
//------------------------------------------------
#define BMP280_ADDRESS 0xEC //BMP280 I2C ADDRESS (0x76<<1)
#define BMP280_REG_ID 0xD0 //BMP280 ID REGISTER
#define BMP280_ID 0x58 //BMP280 I2C ID
#define BMP280_REG_SOFTRESET 0xE0 //BMP280 SOFT RESET REGISTER
#define BMP280_SOFTRESET_VALUE 0xB6 //BMP280 SOFT RESET VALUE
#define BMP280_REG_CTRL_HUM 0xF2 // Humidity measure control register
#define BMP280_REGISTER_STATUS 0XF3 //BMP280 STATUS REGISTER
#define BMP280_REG_CTRL_MEAS 0xF4 // Control register pressure and temperature measure
#define BMP280_STATUS_MEASURING 0X08 //Running conversion
#define BMP280_STATUS_IM_UPDATE 0X01 //NVM data copying
#define BMP280_REG_CONFIG 0xF5 // Configuration register
#define BMP280_REGISTER_PRESSUREDATA 0xF7
#define BMP280_REGISTER_TEMPDATA 0xFA
//------------------------------------------------
#define be16toword(a) ((((a)>>8)&0xff)|(((a)<<8)&0xff00))
#define be24toword(a) ((((a)>>16)&0x000000ff)|((a)&0x0000ff00)|(((a)<<16)&0x00ff0000))
//------------------------------------------------
typedef struct
{
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;
  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;
} BMP280_CalibData;
//------------------------------------------------
#define BMP280_REGISTER_DIG_T1 0x88
#define BMP280_REGISTER_DIG_T2 0x8A
#define BMP280_REGISTER_DIG_T3 0x8C
#define BMP280_REGISTER_DIG_P1 0x8E
#define BMP280_REGISTER_DIG_P2 0x90
#define BMP280_REGISTER_DIG_P3 0x92
#define BMP280_REGISTER_DIG_P4 0x94
#define BMP280_REGISTER_DIG_P5 0x96
#define BMP280_REGISTER_DIG_P6 0x98
#define BMP280_REGISTER_DIG_P7 0x9A
#define BMP280_REGISTER_DIG_P8 0x9C
#define BMP280_REGISTER_DIG_P9 0x9E
//------------------------------------------------
#define BMP280_STBY_MSK 0xE0
#define BMP280_STBY_0_5 0x00
#define BMP280_STBY_62_5 0x20
#define BMP280_STBY_125 0x40
#define BMP280_STBY_250 0x60
#define BMP280_STBY_500 0x80
#define BMP280_STBY_1000 0xA0
#define BMP280_STBY_2000 0xC0
#define BMP280_STBY_4000 0xE0
//------------------------------------------------
#define BMP280_FILTER_MSK 0x1C
#define BMP280_FILTER_OFF 0x00
#define BMP280_FILTER_2 0x04
#define BMP280_FILTER_4 0x08
#define BMP280_FILTER_8 0x0C
#define BMP280_FILTER_16 0x10
//------------------------------------------------
#define BMP280_OSRS_T_MSK 0xE0
#define BMP280_OSRS_T_SKIP 0x00
#define BMP280_OSRS_T_x1 0x20
#define BMP280_OSRS_T_x2 0x40
#define BMP280_OSRS_T_x4 0x60
#define BMP280_OSRS_T_x8 0x80
#define BMP280_OSRS_T_x16 0xA0
#define BMP280_OSRS_P_MSK 0x1C
#define BMP280_OSRS_P_SKIP 0x00
#define BMP280_OSRS_P_x1 0x04
#define BMP280_OSRS_P_x2 0x08
#define BMP280_OSRS_P_x4 0x0C
#define BMP280_OSRS_P_x8 0x10
#define BMP280_OSRS_P_x16 0x14
//------------------------------------------------
#define BMP280_MODE_MSK 0x03
#define BMP280_MODE_SLEEP 0x00
#define BMP280_MODE_FORCED 0x01
#define BMP280_MODE_NORMAL 0x03
//------------------------------------------------
#endif /* BMP280_H_ */
