#include "BMP280.h"
//------------------------------------------------
extern I2C_HandleTypeDef hi2c2;
extern char str[100];
BMP280_CalibData CalibData;
int32_t temper_int;
//------------------------------------------------
static void I2Cx_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Write(&hi2c2, Addr, (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT, &Value, 1, 0x10000);
}
//------------------------------------------------
static uint8_t I2Cx_ReadData(uint16_t Addr, uint8_t Reg)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t value = 0;
  status = HAL_I2C_Mem_Read(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, &value, 1, 0x10000);
  return value;
}
//------------------------------------------------
static void I2Cx_ReadData16(uint16_t Addr, uint8_t Reg, uint16_t *Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Read(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)Value, 2, 0x10000);
}
//------------------------------------------------
static void I2Cx_ReadData24(uint16_t Addr, uint8_t Reg, uint32_t *Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  status = HAL_I2C_Mem_Read(&hi2c2, Addr, Reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)Value, 3, 0x10000);
}
//------------------------------------------------
void BMP280_WriteReg(uint8_t Reg, uint8_t Value)
{
  I2Cx_WriteData(BMP280_ADDRESS, Reg, Value);
}
//------------------------------------------------
uint8_t BMP280_ReadReg(uint8_t Reg)
{
  uint8_t res = I2Cx_ReadData(BMP280_ADDRESS,Reg);
  return res;
}
//------------------------------------------------
void BMP280_ReadReg_U16(uint8_t Reg, uint16_t *Value)
{
  I2Cx_ReadData16(BMP280_ADDRESS,Reg,Value);
}
//------------------------------------------------
void BMP280_ReadReg_S16(uint8_t Reg, int16_t *Value)
{
  I2Cx_ReadData16(BMP280_ADDRESS,Reg, (uint16_t*) Value);
}
//------------------------------------------------
void BMP280_ReadReg_BE_U24(uint8_t Reg, uint32_t *Value)
{
  I2Cx_ReadData24(BMP280_ADDRESS,Reg,Value);
  *(uint32_t *) Value = be24toword(*(uint32_t *) Value) & 0x00FFFFFF;
}
//------------------------------------------------
uint8_t BMP280_ReadStatus(void)
{
  uint8_t res = BMP280_ReadReg(BMP280_REGISTER_STATUS)&0x09;
  return res;
}
//------------------------------------------------
void BMP280_ReadCoefficients(void)
{
  BMP280_ReadReg_U16(BMP280_REGISTER_DIG_T1,&CalibData.dig_T1);
  sprintf(str, "DIG_T1: %u\n", CalibData.dig_T1);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_T2,&CalibData.dig_T2);
  sprintf(str, "DIG_T2: %d\n", CalibData.dig_T2);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_T3,&CalibData.dig_T3);
  sprintf(str, "DIG_T3: %d\n", CalibData.dig_T3);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_U16(BMP280_REGISTER_DIG_P1,&CalibData.dig_P1);
  sprintf(str, "DIG_P1: %u\n", CalibData.dig_P1);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P2,&CalibData.dig_P2);
  sprintf(str, "DIG_P2: %d\n", CalibData.dig_P2);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P3,&CalibData.dig_P3);
  sprintf(str, "DIG_P3: %d\n", CalibData.dig_P3);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P4,&CalibData.dig_P4);
  sprintf(str, "DIG_P4: %d\n", CalibData.dig_P4);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P5,&CalibData.dig_P5);
  sprintf(str, "DIG_P5: %d\n", CalibData.dig_P5);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P6,&CalibData.dig_P6);
  sprintf(str, "DIG_P6: %d\n", CalibData.dig_P6);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P7,&CalibData.dig_P7);
  sprintf(str, "DIG_P7: %d\n", CalibData.dig_P7);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P8,&CalibData.dig_P8);
  sprintf(str, "DIG_P8: %d\n", CalibData.dig_P8);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_ReadReg_S16(BMP280_REGISTER_DIG_P9,&CalibData.dig_P9);
  sprintf(str, "DIG_P9: %d\n", CalibData.dig_P9);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
}
//------------------------------------------------
void BMP280_SetStandby(uint8_t tsb)
{
  uint8_t reg;
  reg = BMP280_ReadReg(BMP280_REG_CONFIG) & ~BMP280_STBY_MSK;
  reg |= tsb & BMP280_STBY_MSK;
  BMP280_WriteReg(BMP280_REG_CONFIG,reg);
}
//------------------------------------------------
void BMP280_SetFilter(uint8_t filter)
{
 uint8_t reg;
 reg = BMP280_ReadReg(BMP280_REG_CONFIG) & ~BMP280_FILTER_MSK;
 reg |= filter & BMP280_FILTER_MSK;
 BMP280_WriteReg(BMP280_REG_CONFIG,reg);
}
//------------------------------------------------
void BMP280_SetOversamplingTemper(uint8_t osrs)
{
  uint8_t reg;
  reg = BMP280_ReadReg(BMP280_REG_CTRL_MEAS) & ~BMP280_OSRS_T_MSK;
  reg |= osrs & BMP280_OSRS_T_MSK;
  BMP280_WriteReg(BMP280_REG_CTRL_MEAS,reg);
}
//------------------------------------------------
void BMP280_SetOversamplingPressure(uint8_t osrs)
{
  uint8_t reg;
  reg = BMP280_ReadReg(BMP280_REG_CTRL_MEAS) & ~BMP280_OSRS_P_MSK;
  reg |= osrs & BMP280_OSRS_P_MSK;
  BMP280_WriteReg(BMP280_REG_CTRL_MEAS,reg);
}
//------------------------------------------------
void BMP280_SetMode(uint8_t mode)
{
  uint8_t reg;
  reg = BMP280_ReadReg(BMP280_REG_CTRL_MEAS) & ~BMP280_MODE_MSK;
  reg |= mode & BMP280_MODE_MSK;
  BMP280_WriteReg(BMP280_REG_CTRL_MEAS,reg);
}
//------------------------------------------------
float BMP280_ReadTemperature(void)
{
  float temper_float = 0.0f;
  uint32_t temper_raw;
  int32_t val1;
  float val2 = 0.0f;
  BMP280_ReadReg_BE_U24(BMP280_REGISTER_TEMPDATA,&temper_raw);
  temper_raw >>= 4;
  val1 = (((double)temper_raw/16384.0 - ((double)CalibData.dig_T1)/1024.0)*((double)CalibData.dig_T2));
  val2 = ((((float)(temper_raw)))/131072.0 - ((float)CalibData.dig_T1)/8192.0)*(((float)temper_raw)/131072.0 - (((float)CalibData.dig_T1)/8192.0))*((float)CalibData.dig_T3);
  temper_float = ((float)val1+(float)val2)/5120.0;
  sprintf(str, "%.3f", temper_float);
  LCD_SetPos(9,0);
  LCD_String(str);
  return temper_float;
}
//------------------------------------------------
float BMP280_ReadPressure(void)
{
  float press_float = 0.0f;
  uint32_t press_raw, pres_int;
  float val1 = 0.0f;
  float val2 = 0.0f;
  BMP280_ReadTemperature(); // must be done first to get t_fine
  BMP280_ReadReg_BE_U24(BMP280_REGISTER_PRESSUREDATA,&press_raw);
  press_raw >>= 4;
  val1 = ((double) temper_int/2.0) - 64000.0;
  val2 = val1 * val1 * ((double)CalibData.dig_P6)/32768.0;
  val2 = val2 + val1*((double)CalibData.dig_P5)*2.0;
  val2 = (val2/4.0)+(((double)CalibData.dig_P4)*65536.0);
  val1 = (((double)CalibData.dig_P3)*val1*val1/524288.0+((double)CalibData.dig_P2)*val1)/524288.0;
  val1 = (1.0+val1/32768.0)*((double)CalibData.dig_P1);
  press_float = 1048576.0 - (double)press_raw;
  press_float = (press_float-(val2/4096.0))*6250.0/val1;
  val1 = ((double)CalibData.dig_P9)*press_float*press_float/2147483648.0;
  val2 = press_float*((double)CalibData.dig_P8)/32768.0;
  press_float = press_float+((val1+val2+((double)CalibData.dig_P7)))/16.0;
  press_float = press_float*0.00750062;
  sprintf(str, "%.2f", press_float);
  LCD_SetPos(9,1);
  LCD_String(str);
  return press_float;
}
//------------------------------------------------
void BMP280_Init(void)
{
  uint8_t value = 0;
  uint32_t value32 = 0;
  value = BMP280_ReadReg(BMP280_REG_ID);
  sprintf(str, "ID: 0x%02X\n", value);
  //////HAL_UART_Transmit(&huart2,(uint8_t*)str,strlen(str),0x1000);
  BMP280_WriteReg(BMP280_REG_SOFTRESET,BMP280_SOFTRESET_VALUE);
  while (BMP280_ReadStatus() & BMP280_STATUS_IM_UPDATE);
  BMP280_ReadCoefficients();
  BMP280_SetStandby(BMP280_STBY_1000);
  BMP280_SetFilter(BMP280_FILTER_4);
  BMP280_SetOversamplingTemper(BMP280_OSRS_T_x4);
  BMP280_SetOversamplingPressure(BMP280_OSRS_P_x2);
  value32 = BMP280_ReadReg(BMP280_REG_CTRL_MEAS);
  sprintf(str, "Temperature: %s\nPressure: %s\n",
    (value32 & BMP280_OSRS_T_MSK) ? "ON" : "OFF",
    (value32 & BMP280_OSRS_P_MSK) ? "ON" : "OFF");
  BMP280_SetMode(BMP280_MODE_NORMAL);
}
