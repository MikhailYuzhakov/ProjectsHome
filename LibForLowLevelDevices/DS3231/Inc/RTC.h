#ifdef RTC_H_
#define RTC_H_
//---------------------------------------
#include "stm32l1xx_hal.h"
//---------------------------------------
uint8_t aTxBuffer[8];

uint8_t RTC_ConvertFromDec(uint8_t c); //перевод двоично-десятичного числа в десятичное
uint8_t RTC_ConvertFromBinDec(uint8_t c); //перевод десятичного числа в двоично-десятичное
//---------------------------------------
uint8_t RTC_ConvertFromDec(uint8_t c)
{
	uint8_t ch = ((c>>4)*10+(0x0F&c));
	return ch;
}
//---------------------------------------
uint8_t RTC_ConvertFromBinDec(uint8_t c)
{
	uint8_t ch = ((c/10)<<4)|(c%10);
	return ch;
}
//---------------------------------------
#endif /* RTC_H_ */
