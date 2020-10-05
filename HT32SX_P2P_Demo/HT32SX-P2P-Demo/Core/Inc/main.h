/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RADIO_SPI_CS_PIN_Pin GPIO_PIN_15
#define RADIO_SPI_CS_PIN_GPIO_Port GPIOA
#define RADIO_SPI_MISO_PIN_Pin GPIO_PIN_4
#define RADIO_SPI_MISO_PIN_GPIO_Port GPIOB
#define RADIO_SPI_SCK_PIN_Pin GPIO_PIN_3
#define RADIO_SPI_SCK_PIN_GPIO_Port GPIOB
#define RADIO_GPIO_SDN_PIN_Pin GPIO_PIN_8
#define RADIO_GPIO_SDN_PIN_GPIO_Port GPIOB
#define USER_BUTTON_PIN_Pin GPIO_PIN_0
#define USER_BUTTON_PIN_GPIO_Port GPIOB
#define USER_BUTTON_PIN_EXTI_IRQn EXTI0_1_IRQn
#define RADIO_SPI_MOSI_PIN_Pin GPIO_PIN_7
#define RADIO_SPI_MOSI_PIN_GPIO_Port GPIOA
#define USER_LED_PIN_Pin GPIO_PIN_5
#define USER_LED_PIN_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
