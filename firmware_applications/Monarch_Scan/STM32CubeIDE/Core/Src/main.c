/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "dma.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "monarch_app.h"
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

/* USER CODE BEGIN PV */
TIM_HandleTypeDef  Tim6_Handler={.Instance=TIM6};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_TIM6_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	mcuConfig();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		if(getScanStatus()) {
			monarchScan(ALL_REGIONS, 6, SFX_TIME_M);
			setScanStatus(0);
		}

		configRegion();

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
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void mcuConfig(void) {
	ST_SFX_ERR stSfxRetErr;

	ST_Init();

	NVM_BoardDataType sfxConfiguration;
	stSfxRetErr = ST_Sigfox_Init(&sfxConfiguration, 0);

	if(stSfxRetErr != ST_SFX_ERR_NONE)
	{
		/* If an error occured reading Sigfox credentials (for example the board has never been registered)
		 * automatically set the test mode credentials. */
		if(stSfxRetErr == ST_SFX_ERR_CREDENTIALS)
		{
			sfxConfiguration.id = 0;
			memset(sfxConfiguration.pac, 0x00, 8);
			sfxConfiguration.rcz = 0;

		} else 
			printf("Error!\n");

	}

	/* Calibrate RTC in case of STM32*/
#if  !(defined(BLUENRG2_DEVICE) || defined(BLUENRG1_DEVICE))
	/* The low level driver uses the internal RTC as a timer while the STM32 is in low power.
  This function calibrates the RTC using an auxiliary general purpose timer in order to
  increase its precision. */
	ST_MCU_API_TimerCalibration(500);
#endif

	printf("Sigfox Monarch iMCP HT32SX\n");
	printf("ID: %.8X - PAC: ", (unsigned int)sfxConfiguration.id);

	for(uint16_t i = 0; i < sizeof(sfxConfiguration.pac); i++)
		printf("%.2X", sfxConfiguration.pac[i]);

	printf("\n");

	ST_RF_API_set_xtal_freq(50000000); 
	ST_RF_API_set_freq_offset(sfxConfiguration.freqOffset);
	ST_RF_API_set_rssi_offset(sfxConfiguration.rssiOffset);
	ST_RF_API_set_lbt_thr_offset(sfxConfiguration.lbtOffset);

}

void ST_Init(void)
{
	/* Put the radio off */
	S2LPShutdownInit();
	HAL_Delay(10);
	S2LPShutdownExit();

	/* Init TIM6 which will trigger TX */
	SdkEvalTimersState(&Tim6_Handler, ENABLE);

	/* Auto detect settings, if EEPROM is available */
#if EEPROM_PRESENT == EEPROM_YES
	/* Identify the S2-LP RF board reading some production data */
	S2LPManagementIdentificationRFBoard();
#elif EEPROM_PRESENT==EEPROM_NO
	/* Set XTAL frequency with offset */
	S2LPRadioSetXtalFrequency(XTAL_FREQUENCY+XTAL_FREQUENCY_OFFSET);

	/* Set the frequency base */
	S2LPManagementSetBand(BOARD_FREQUENCY_BAND);

	/* Configure PA availability */
#if S2LP_FEM_PRESENT == S2LP_FEM_NO
	S2LPManagementSetRangeExtender(0);
#else
	S2LPManagementSetRangeExtender(1);
#endif
#endif

	/* uC IRQ config and enable */

	S2LPIRQInit();
	S2LPIRQEnable(ENABLE, M2S_GPIO_EDGE_EVENT);

	/* FEM Initialization */
	FEM_Init();

	/* TCXO Initialization */
	//TCXO_Init();

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
