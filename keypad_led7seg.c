/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
char Key=0;
int i;
#define NUMROWS 4
#define NUMCOLS 3

//Khai bao cac du lieu cua keypad
typedef struct						
{
	uint32_t RowPins[NUMROWS];
	uint32_t ColPins[NUMCOLS];
	GPIO_TypeDef* RowPort[NUMROWS];
	GPIO_TypeDef* ColPort[NUMCOLS];
	char MAP[NUMROWS][NUMCOLS];
	char Value;
}KEYPAD_Name;

static void KEYPAD_Delay(uint16_t Time)
{
	HAL_Delay(Time);
}

//Ham khoi tao keypad
void KEYPAD3X4_Init(KEYPAD_Name* KEYPAD, char KEYMAP[NUMROWS][NUMCOLS],
										GPIO_TypeDef* COL1_PORT, uint32_t COL1_PIN, 
										GPIO_TypeDef* COL2_PORT, uint32_t COL2_PIN,
										GPIO_TypeDef* COL3_PORT, uint32_t COL3_PIN,
										GPIO_TypeDef* ROW1_PORT, uint32_t ROW1_PIN,
										GPIO_TypeDef* ROW2_PORT, uint32_t ROW2_PIN,
										GPIO_TypeDef* ROW3_PORT, uint32_t ROW3_PIN,
										GPIO_TypeDef* ROW4_PORT, uint32_t ROW4_PIN)
{
	KEYPAD->ColPort[0] = COL1_PORT; //Copy gia tri vao du lieu cua keypad
	KEYPAD->ColPort[1] = COL2_PORT;
	KEYPAD->ColPort[2] = COL3_PORT;
	KEYPAD->ColPins[0] = COL1_PIN;
	KEYPAD->ColPins[1] = COL2_PIN;
	KEYPAD->ColPins[2] = COL3_PIN;
	
	KEYPAD->RowPort[0] = ROW1_PORT;
	KEYPAD->RowPort[1] = ROW2_PORT;
	KEYPAD->RowPort[2] = ROW3_PORT;
	KEYPAD->RowPort[3] = ROW4_PORT;
	KEYPAD->RowPins[0] = ROW1_PIN;
	KEYPAD->RowPins[1] = ROW2_PIN;
	KEYPAD->RowPins[2] = ROW3_PIN;
	KEYPAD->RowPins[3] = ROW4_PIN;
	
	for(int colum = 0; colum < NUMCOLS; colum++)
	{
		for(int row = 0; row < NUMROWS; row++)
		{
			KEYPAD->MAP[row][colum] = KEYMAP[row][colum];
		}
	}
	
	HAL_GPIO_WritePin(KEYPAD->ColPort[0],KEYPAD->ColPins[0],GPIO_PIN_SET);
	HAL_GPIO_WritePin(KEYPAD->ColPort[1],KEYPAD->ColPins[1],GPIO_PIN_SET);
	HAL_GPIO_WritePin(KEYPAD->ColPort[2],KEYPAD->ColPins[2],GPIO_PIN_SET);
}
char KEYPAD3X4_Readkey(KEYPAD_Name* KEYPAD) // Scan Colums
{
	KEYPAD->Value = 0;
	for(int colum = 0; colum < NUMCOLS; colum++)
	{
		HAL_GPIO_WritePin(KEYPAD->ColPort[colum],KEYPAD->ColPins[colum],GPIO_PIN_RESET);
		for(int row = 0; row < NUMROWS; row++)
		{
			if(HAL_GPIO_ReadPin(KEYPAD->RowPort[row],KEYPAD->RowPins[row]) == 0)
			{
				KEYPAD_Delay(50);// debound
				while(HAL_GPIO_ReadPin(KEYPAD->RowPort[row],KEYPAD->RowPins[row])==0){}
				KEYPAD->Value = KEYPAD->MAP[row][colum];
					
				return KEYPAD->Value;
			}
		}
		HAL_GPIO_WritePin(KEYPAD->ColPort[colum],KEYPAD->ColPins[colum],GPIO_PIN_SET);
	}
	
	return 0;
}

void KEYPAD3x4_Config(KEYPAD_Name* KEYPAD, char KEYMAP_Config[NUMROWS][NUMCOLS])
{
	for(int colum = 0; colum < NUMCOLS; colum++)
	{
		for(int row = 0; row < NUMROWS; row++)
		{
			KEYPAD->MAP[row][colum] = KEYMAP_Config[row][colum];
		}
	}
}
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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char seg7[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};
KEYPAD_Name KeyPad;
char KEYMAP[NUMROWS][NUMCOLS]={
{1,2,3},
{4,5,6},
{7,8,9},
{10,'0',11}
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void delay(unsigned int ms)
{ unsigned int x, y;
for (x = 0; x <= ms; x++)
for (y = 0; y <= 120; y++);
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
//khai báo các chân Keypad->STM32
KEYPAD3X4_Init(&KeyPad, KEYMAP, GPIOB, GPIO_PIN_4, GPIOB, GPIO_PIN_5, GPIOB, GPIO_PIN_6, 
								GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_1,
								GPIOB, GPIO_PIN_2, GPIOB, GPIO_PIN_3);
GPIOA->ODR= seg7[0];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
	{
		Key = KEYPAD3X4_Readkey(&KeyPad);
		if(Key){
			if(Key == '0' ){GPIOA->ODR= seg7[0];}
			else{
			GPIOA->ODR= seg7[Key];}
		}
		HAL_Delay(50);
	}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

