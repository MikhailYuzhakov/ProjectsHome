/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint16_t Timer1=0;
uint8_t sect[512];
extern char str1[60];
uint32_t byteswritten,bytesread;
uint8_t result;
char USER_Path[4]; /* logical drive path */
FATFS SDFatFs;
FATFS *fs;
FIL MyFile;
//char buffer1[512] ="Hello, my name is Mikhail and I can write code for SD card on MCU STM32L152RE!"; //Буфер данных для записи/чтения
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FRESULT ReadLongFile(void)
{
  uint16_t i=0, i1=0;
  uint32_t ind=0;
  uint32_t f_size = MyFile.obj.objsize;
  sprintf(str1,"fsize: %lu\n",(unsigned long)f_size);
  HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
  ind=0;
  do
  {
    if(f_size<512)
    {
      i1=f_size;
    }
    else
    {
      i1=512;
    }
    f_size-=i1;
    f_lseek(&MyFile,ind);
    f_read(&MyFile,sect,i1,(UINT *)&bytesread);
    for(i=0;i<bytesread;i++)
    {
      HAL_UART_Transmit(&huart2,sect+i,1,0x1000);
    }
    ind+=i1;
  }
  while(f_size>0);
  HAL_UART_Transmit(&huart2,(uint8_t*)"\n",2,0x1000);
  return FR_OK;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint16_t i;
	char str2[] = "fileone.csv";
	FRESULT res; //результат выполнения
	uint8_t wtext[]=" ";
	FILINFO fileInfo;
	char *fn;
	DIR dir;
	DWORD fre_clust, fre_sect, tot_sect;
  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2); //запустим таймер 2
  //SD_PowerOn();
  //sd_ini();
  //SD_Write_Block((uint8_t*)buffer1,0x0400); //Запишем блок в буфер
  //SD_Read_Block(sect,0x0400); //Считаем блок из буфера
  //for(i=0;i<512;i++) HAL_UART_Transmit(&huart2,sect+i,1,0x1000);
  //HAL_UART_Transmit(&huart2,(uint8_t*)"\n",2,0x1000);
  disk_initialize(SDFatFs.drv);
  //read
  /*if(f_mount(&SDFatFs,(TCHAR const*)USER_Path,0) != FR_OK)
  {
    Error_Handler();
  }
  else
  {
    if(f_open(&MyFile,"123.txt",FA_READ)!=FR_OK)
    {
      Error_Handler();
    }
    else
    {
		ReadLongFile();
		f_close(&MyFile);
    }
  }*/
  //sprintf(str2, "fileone.csv");
  //read dir
  if(f_mount(&SDFatFs,(TCHAR const*)USER_Path,0)!=FR_OK)
  {
    Error_Handler();
  }
  else
  {
    fileInfo.fname[13] = (char*)sect;
    fileInfo.fsize = sizeof(sect);
    result = f_opendir(&dir, "/");
    if (result == FR_OK)
    {
    	  while(1)
    	  {
    	    result = f_readdir(&dir, &fileInfo);
    	    if (result==FR_OK && fileInfo.fname[0])
    	    {
    	    	  fn = fileInfo.fname;
    	    	  if(strlen(fn)) HAL_UART_Transmit(&huart2,(uint8_t*)fn,strlen(fn),0x1000);
    	    	  else HAL_UART_Transmit(&huart2,(uint8_t*)fileInfo.fname,strlen((char*)fileInfo.fname),0x1000);
    	    	  if(fileInfo.fattrib&AM_DIR)
    	    	  {
    	    	    HAL_UART_Transmit(&huart2,(uint8_t*)" [DIR]",7,0x1000);
    	    	  }
    	    }
    	    else break;
    	    HAL_UART_Transmit(&huart2,(uint8_t*)"\n",2,0x1000);
    	    f_getfree("/", &fre_clust, &fs);
    	    sprintf(str1,"fre_clust: %lu\n",fre_clust);
    	    HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
    	    sprintf(str1,"n_fatent: %lu\n",fs->n_fatent);
    	    HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
    	    sprintf(str1,"fs_csize: %d\n",fs->csize);
    	    HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
    	    tot_sect = (fs->n_fatent - 2) * fs->csize;
    	    sprintf(str1,"tot_sect: %lu\n",tot_sect);
    	    HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
    	    fre_sect = fre_clust * fs->csize;
    	    sprintf(str1,"fre_sect: %lu\n",fre_sect);
    	    HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
    	    sprintf(str1, "%lu KB total drive space.\n%lu KB available.\n",
    	    fre_sect/2, tot_sect/2);
    	    HAL_UART_Transmit(&huart2,(uint8_t*)str1,strlen(str1),0x1000);
    	  }
      f_closedir(&dir);
    }
  }
  uint8_t j=0;
  //FATFS_UnLinkDriver(USER_Path);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  sprintf(wtext, "%d;", j);
	  char buf[20];
	  //write
	    if(f_mount(&SDFatFs,(TCHAR const*)USER_Path,0)!=FR_OK)
	    {
	      Error_Handler();
	    }
	    else
	    {
	      if(f_open(&MyFile,str2,FA_CREATE_ALWAYS|FA_WRITE)!=FR_OK)
	      {
	        Error_Handler();
	      }
	      else
	      {
	        res=f_write(&MyFile,wtext,sizeof(wtext),(void*)&byteswritten);
	        if((byteswritten==0)||(res!=FR_OK))
	        {
	          Error_Handler();
	        }
	        f_close(&MyFile);
	      }
	    }
	    FATFS_UnLinkDriver(USER_Path);
	    j++;
      	//HAL_UART_Transmit(&huart2,"Success\n",8,0x1000);
      	HAL_UART_Transmit(&huart2,wtext,sizeof(wtext),0x1000);
      	HAL_Delay(2000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 39999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 230400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //обработчик прерывания по переполнению таймера
{
	if(htim==&htim2)
	{
		Timer1++;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
