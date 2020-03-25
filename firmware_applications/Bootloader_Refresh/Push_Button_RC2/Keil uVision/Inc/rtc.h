/**
  ******************************************************************************
  * File Name          : RTC.h
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rtc_H
#define __rtc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */
extern RTC_HandleTypeDef * getRtcHandler(void);

extern void Configure_RTC_Clock(void);

/* Set flags */
extern void setRtcInUseFlag(uint8_t rtcInUse);
extern void setRtcIrqFlag(uint8_t rtcIrq);
extern void setRtcInUseForCsFlag(uint8_t rtcInUseForCs);
extern void setNextRtcWakeUpFlag(uint32_t nextRtcWakeUp);
extern void setRtcPrescFlag(uint16_t rtcPresc);


/* Get flags */ 
extern uint32_t getNextRtcWakeUpFlag(void);
extern uint8_t getRtcInUseForCsFlag(void);
extern uint8_t getRtcIrqFlag(void);
extern uint8_t getRtcInUseFlag(void);
extern uint16_t getRtcPrescFlag(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
