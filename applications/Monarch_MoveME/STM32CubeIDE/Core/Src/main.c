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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define READ_ACC_VAL 0 // Enables acceleration handler to read 3-axis values
												

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

TIM_HandleTypeDef  Tim6_Handler={.Instance=TIM6};

uint32_t AccInstance = IKS01A2_LSM6DSL_0;		// Accelerometer handler instance
uint32_t timer_latter;											// Timer register 
int counter_exti = 0;												// ACC GPIO EXTI counter
int FSM_state = 0; 													// ACC GPIO EXTI handler state machine
int led_status = 0;													// Controls USER LED status
int msg_send = 0; 													// Used to allow SigFox MSG publish

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void ST_Init(void);
void MCU_Config(void);
void configRegion(void);

static void enable_accelerometer(void);			// Enables accelerometer sensor
static void Initialize_accelerometer(void); // Initalizes accelerometer sensor

#if READ_ACC_VAL
extern void Accelero_Sensor_Handler(uint32_t Instance); // Retrieves accelerometer's 3-D axis value 
#endif
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
	uint8_t state = 1;			// Controls if device is in monarch mode or move-me mode
	
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
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  MX_TIM22_Init();
  /* USER CODE BEGIN 2 */
	
	// ------------------------
	// 		Initialize system
	// ------------------------
	
	MCU_Config();
	
	Initialize_accelerometer();
	
  enable_accelerometer();
	
	(void)IKS01A2_MOTION_SENSOR_Enable_Wake_Up_Detection(AccInstance, IKS01A2_MOTION_SENSOR_INT1_PIN); // Enables accelerometer motion detection interruption 
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
	
	led_status = 1;
	
	msg_send = 0;
	
	HAL_TIM_Base_MspInit(&htim22);
	
	for (int i = 0; i < 20; i++)
	{
		printf("\n\r");
	}
	printf(" .d88b.  d8888b.     .88b  d88.  .d88b.  d8888b. d88888b  \r");
	printf(".8P  Y8. 88  `8D     88'YbdP`88 .8P  Y8. 88  `8D 88'     \r");
	printf("88    88 88oodD'     88  88  88 88    88 88   88 88ooooo  \r");
	printf("88    88 88~~~       88  88  88 88    88 88   88 88~~~~~  \r");
	printf("`8b  d8' 88          88  88  88 `8b  d8' 88  .8D 88.      \r");
	printf(" `Y88P'  88          YP  YP  YP  `Y88P'  Y8888D' Y88888P \r");
	
	for (int i = 0; i < 2; i++)
	{
		printf("\n\r");
	}

	printf(".88b  d88.  .d88b.  db    db d88888b        .88b  d88. d88888b \r");
	printf("88'YbdP`88 .8P  Y8. 88    88 88'            88'YbdP`88 88'     \r");
	printf("88  88  88 88    88 Y8    8P 88ooooo        88  88  88 88ooooo \r");
	printf("88  88  88 88    88 `8b  d8' 88~~~~~ C8888D 88  88  88 88~~~~~ \r");
	printf("88  88  88 `8b  d8'  `8bd8'  88.            88  88  88 88.     \r");
	printf("YP  YP  YP  `Y88P'     YP    Y88888P        YP  YP  YP Y88888P \r");
	
	for (int i = 0; i < 10; i++)
	{
		printf("\n\r");
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		
		if(!readButton()) { // Pressing USER button changes device mode 
	
			state = !led_status ? state : (state ^ 1);
			
			for (int i = 0; i < 20; i++)
			{
				printf("\n\r");
			}
			
			printf(" .d88b.  d8888b.     .88b  d88.  .d88b.  d8888b. d88888b  \r");
			printf(".8P  Y8. 88  `8D     88'YbdP`88 .8P  Y8. 88  `8D 88'     \r");
			printf("88    88 88oodD'     88  88  88 88    88 88   88 88ooooo  \r");
			printf("88    88 88~~~       88  88  88 88    88 88   88 88~~~~~  \r");
			printf("`8b  d8' 88          88  88  88 `8b  d8' 88  .8D 88.      \r");
			printf(" `Y88P'  88          YP  YP  YP  `Y88P'  Y8888D' Y88888P \r");
			
			for (int i = 0; i < 2; i++)
			{
				printf("\n\r");
			}
			
			if(!state) {
				
				(void)IKS01A2_MOTION_SENSOR_Disable_Wake_Up_Detection(AccInstance); // Temporarly disables wake-up detection interrupt
				
				printf(".88b  d88.  .d88b.  d8b   db  .d8b.  d8888b.  .o88b. db   db  \r");
				printf("88'YbdP`88 .8P  Y8. 888o  88 d8' `8b 88  `8D d8P  Y8 88   88  \r");
				printf("88  88  88 88    88 88V8o 88 88ooo88 88oobY' 8P      88ooo88  \r");
				printf("88  88  88 88    88 88 V8o88 88~~~88 88`8b   8b      88~~~88  \r");
				printf("88  88  88 `8b  d8' 88  V888 88   88 88 `88. Y8b  d8 88   88  \r");
				printf("YP  YP  YP  `Y88P'  VP   V8P YP   YP 88   YD  `Y88P' YP   YP \r");
				
				
			} else {
				
				printf(".88b  d88.  .d88b.  db    db d88888b        .88b  d88. d88888b \r");
				printf("88'YbdP`88 .8P  Y8. 88    88 88'            88'YbdP`88 88'     \r");
				printf("88  88  88 88    88 Y8    8P 88ooooo        88  88  88 88ooooo \r");
				printf("88  88  88 88    88 `8b  d8' 88~~~~~ C8888D 88  88  88 88~~~~~ \r");
				printf("88  88  88 `8b  d8'  `8bd8'  88.            88  88  88 88.     \r");
				printf("YP  YP  YP  `Y88P'     YP    Y88888P        YP  YP  YP Y88888P \r");
				
				setScanStatus(SCAN_IDLE);
				
				(void)IKS01A2_MOTION_SENSOR_Enable_Wake_Up_Detection(AccInstance, IKS01A2_MOTION_SENSOR_INT1_PIN); // Re-enables wake-up detection interrupt
			}
			
			for (int i = 0; i < 10; i++)
			{
				printf("\n\r");
			}
			
			HAL_Delay(500);
		}
		
		if(state) { // Move-me mode, device will detect motion and send a SigFox frame when enough motion is applied to device.
			
			if (led_status == 1) // Change USER LED status (ON, OFF)
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			}
			
			if (FSM_state == 2) // Device detected enough motion and will send a SigFox message
			{
				
				if(msg_send == 1)
				{
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // Enables USER LED indicating that the system is in IDLE state
					
					for (int i = 0; i < 20; i++)
					{
						printf("\n\r");
					}
					printf(".d8888. d88888b d8b   db d8888b. d888888b d8b   db  d888b       .88b  d88. .d8888.  d888b  \r");
					printf("88'  YP 88'     888o  88 88  `8D   `88'   888o  88 88' Y8b      88'YbdP`88 88'  YP 88' Y8b \r");
					printf("`8bo.   88ooooo 88V8o 88 88   88    88    88V8o 88 88           88  88  88 `8bo.   88      \r");
					printf("  `Y8b. 88~~~~~ 88 V8o88 88   88    88    88 V8o88 88  ooo      88  88  88   `Y8b. 88  ooo \r");
					printf("db   8D 88.     88  V888 88  .8D   .88.   88  V888 88. ~8~      88  88  88 db   8D 88. ~8~ \r");
					printf("`8888Y' Y88888P VP   V8P Y8888D' Y888888P VP   V8P  Y888P       YP  YP  YP `8888Y'  Y888P  \r");
								
					for (int i = 0; i < 10; i++)
					{
						printf("\n\r");
					}
					__HAL_TIM_SET_COUNTER(&htim22, 0);
					
					HAL_Delay(500);	
					
					configRegion(); //Configure and send SigFox frame
			
					msg_send = 0;
					
					HAL_Delay(100);
				}
				
				timer_latter = __HAL_TIM_GET_COUNTER(&htim22); // Fetches current timer value
				
				if (timer_latter > 1000) // Wait 1 s before allowing new publish
				{
					
					for (int i = 0; i < 20; i++)
					{
						printf("\n\r");
					}

					printf(".88b  d88.  .d88b.  db    db d88888b        .88b  d88. d88888b      db  \r");
					printf("88'YbdP`88 .8P  Y8. 88    88 88'            88'YbdP`88 88'          88  \r");
					printf("88  88  88 88    88 Y8    8P 88ooooo        88  88  88 88ooooo      YP  \r");
					printf("88  88  88 88    88 `8b  d8' 88~~~~~ C8888D 88  88  88 88~~~~~          \r");
					printf("88  88  88 `8b  d8'  `8bd8'  88.            88  88  88 88.          db  \r");
					printf("YP  YP  YP  `Y88P'     YP    Y88888P        YP  YP  YP Y88888P      YP  \r");
					
					for (int i = 0; i < 20; i++)
					{
						printf("\n\r");
					}
					
					__HAL_TIM_SET_COUNTER(&htim22, 0); // Resets timer counter register
					
					(void)IKS01A2_MOTION_SENSOR_Enable_Wake_Up_Detection(AccInstance, IKS01A2_MOTION_SENSOR_INT1_PIN); // Re-enables wake-up detection interrupt
					
					timer_latter = 0;
					FSM_state = 0;
					led_status = 1;
					
				}
			}
		} else { // Monarch mode
			
				if(getScanStatus() == SCAN_IDLE) 
				{
					MonarchScan(ALL_REGIONS, 10, SFX_TIME_M);
					
					setScanStatus(SCAN_DONE);
				}
				
		}
		
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/*!******************************************************************
 * \fn void MCU_Config(void)
 * \brief Starts the MCU peripherals configuration and get credentials from Flash memory
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/

void MCU_Config(void) {
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
  printf("ID: %.8X - PAC: ", (unsigned int )sfxConfiguration.id);
	
	for(uint16_t i = 0; i < sizeof(sfxConfiguration.pac); i++)
		printf("%.2X", sfxConfiguration.pac[i]);
	
	printf("\n");
	
	ST_RF_API_set_xtal_freq(50000000); 
  ST_RF_API_set_freq_offset(sfxConfiguration.freqOffset); 
	ST_RF_API_set_rssi_offset(sfxConfiguration.rssiOffset);
	ST_RF_API_set_lbt_thr_offset(sfxConfiguration.lbtOffset);
	
}

/*!******************************************************************
 * \fn void ST_Init(void)
 * \brief Configure the MCU peripherals needed for the transceiver operation.
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/

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
  //HBG
	S2LPIRQInit();
  S2LPIRQEnable(ENABLE, M2S_GPIO_EDGE_EVENT);

    /* FEM Initialization */
  FEM_Init();
	
}

/*!******************************************************************
 * \fn void handle_ACC_exti(void)
 * \brief Handles the ACCELEROMETER motion detection interrupts
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/

void handle_ACC_exti(void)
{ 	
			
		switch (FSM_state)
		{
			case 0: // First accelerometer interrupt
			{
				__HAL_TIM_SET_COUNTER(&htim22, 0);
				counter_exti = 0;
				timer_latter = 0;
				FSM_state = 1;
			}
			case 1: // Latter accelerometer interrupt
			{
				timer_latter = __HAL_TIM_GET_COUNTER(&htim22);
				counter_exti++;
				if (timer_latter > 500) 
				{
					if (counter_exti > 4)
					{
						(void)IKS01A2_MOTION_SENSOR_Disable_Wake_Up_Detection(AccInstance); // Temporarly disables ACC INT
						led_status = 0;
						counter_exti = 0;
						timer_latter = 0;
						__HAL_TIM_SET_COUNTER(&htim22, 0);
						FSM_state = 2;
						msg_send = 1;
					}
					else // Not enough motion detected to trigger publish
					{
						FSM_state = 0;
					}
				}
			}
		
		}
			
}

#if READ_ACC_VAL

/*!******************************************************************
 * \fn void Accelero_Sensor_Handler(uint32_t Instance)
 * \brief Handles the ACCELERO axes data getting/sending
 *
 * \param[in]  Instance the device instance
 * \param[out] none
 *******************************************************************/

static void Accelero_Sensor_Handler(uint32_t Instance)
{
  int32_t data[6];
  IKS01A2_MOTION_SENSOR_Axes_t acceleration;
  uint8_t status = 0;

  if (IKS01A2_MOTION_SENSOR_Get_DRDY_Status(Instance, MOTION_ACCELERO, &status) == BSP_ERROR_NONE && status == 1U)
  {

    (void)IKS01A2_MOTION_SENSOR_GetAxes(Instance, MOTION_ACCELERO, &acceleration);

    
    data[0] = acceleration.x;
    data[1] = acceleration.y;
    data[2] = acceleration.z;
	
		printf("[ACC RD SCCSS]\r\n");
		
		printf("ACC_X: %d\r\n", (int)data[0]);
		printf("ACC_Y: %d\r\n", (int)data[1]);
		printf("ACC_Z: %d\r\n", (int)data[2]);
		
  }
  else
  {
		printf("[ACC RD FAIL]\r\n");
  }
}

#endif

/*!******************************************************************
 * \fn void Initialize_accelerometer(void)
 * \brief Initalizes accelerometer 
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/
 
static void Initialize_accelerometer(void)
{
	int32_t ret_val = 0;
	
  ret_val = IKS01A2_MOTION_SENSOR_Init(AccInstance, MOTION_ACCELERO);
	
	if (ret_val == BSP_ERROR_NONE)
	{
		printf("\r\n[ACC INIT] SUCESS!\r\n");
	}
	else 
	{
		printf("\r\n[ACC INIT] FAIL!\r\n");
	}
	
}

/*!******************************************************************
 * \fn void enable_accelerometer(void)
 * \brief Enables accelerometer
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/

static void enable_accelerometer(void)
{
	int32_t ret_val = 0;

	ret_val = IKS01A2_MOTION_SENSOR_Enable(AccInstance, MOTION_ACCELERO);
	
	if (ret_val == BSP_ERROR_NONE)
	{
	  printf("\r\n[ACC ENABLE] SUCESS!\r\n");
		
	}
	else 
	{
	  printf("\r\n[ACC ENABLE] FAIL!\r\n");
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
