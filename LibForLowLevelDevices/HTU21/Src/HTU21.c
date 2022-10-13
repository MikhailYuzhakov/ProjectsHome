#include "HTU21.h"
//------------------------------------------------
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;
extern char str1[100];
//------------------------------------------------
static void I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Write(&hi2c1, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 0x10000);
}
//------------------------------------------------
static uint8_t I2Cx_ReadData(uint16_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  status = HAL_I2C_Mem_Read(&hi2c1, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x10000);
  return value;
}
//------------------------------------------------
uint8_t HTU21_ReadReg(uint8_t Reg)
{
  uint8_t res = I2Cx_ReadData(HTU21_ADDRESS,Reg);
  return res;
}
//------------------------------------------------
void HTU21_Init(void)
{
  uint8_t value = 0;
  uint32_t value32 = 0;
  value = HTU21_ReadReg(HTU21_ID);
  sprintf(str1, "ID: 0x%02X\n", value);
  HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
}
