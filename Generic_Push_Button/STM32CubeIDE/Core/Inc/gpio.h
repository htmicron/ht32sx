/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "usart.h"
#include "st_rf_api.h"
#include "st_mcu_api.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */
extern void S2LPShutdownEnter(void);
extern void S2LPShutdownExit(void);
extern uint8_t S2LPShutdownCheck(void);
extern void S2LPIRQInit(void);
extern void S2LPIRQEnable(uint8_t state, uint8_t edge_direction);
extern void S2LPShutdownInit(void);

extern void setLowPowerFlag(uint8_t lowPower);
extern uint8_t getLowPowerFlag(void);
extern void setS2lpIrqRaisedFlag(uint8_t s2lpIrqRaised);
extern uint8_t getS2lpIrqRaisedFlag(void);

/*!******************************************************************
 * \fn void HT_GPIO_UserButtonHandler(void)
 * \brief Button handler (PA6). Send a frame to the Sigfox Network.
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/
void HT_GPIO_UserButtonHandler(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
