/**
  ******************************************************************************
  * File Name          : RTC.h
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
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

/*!******************************************************************
 * \fn RTC_HandleTypeDef * getRtcHandler(void)
 * \brief Get the currently RTC handler.
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval &hrtc						RTC handler address
 *******************************************************************/ 
extern RTC_HandleTypeDef * getRtcHandler(void);

/*!******************************************************************
 * \fn void Configure_RTC_Clock(void)
 * \brief Configure RTC Clock used at RF API.
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void Configure_RTC_Clock(void);

/*!******************************************************************
 * \fn void setRtcInUseFlag(uint8_t rtcInUse)
 * \brief Set a logic value corresponding to the RTC state. 1 if it's being used. 
 *
 * \param[in]  rtcInUse											1 it's being used, else 0                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setRtcInUseFlag(uint8_t rtcInUse);

/*!******************************************************************
 * \fn void setRtcIrqFlag(uint8_t rtcIrq)
 * \brief Set RTC IRQ flag.
 *
 * \param[in]  rtcIrq													1 or 0                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setRtcIrqFlag(uint8_t rtcIrq);

/*!******************************************************************
 * \fn void setRtcInUseForCsFlag(uint8_t rtcInUseForCs)
 * \brief Set RTC in use for CS flag. 
 *
 * \param[in]  rtcInUseForCs							1 it's being used, else 0                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setRtcInUseForCsFlag(uint8_t rtcInUseForCs);

/*!******************************************************************
 * \fn void setNextRtcWakeUpFlag(uint32_t nextRtcWakeUp)
 * \brief Set Next RTC wake up flag. 
 *
 * \param[in]  nextRtcWakeUp							
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setNextRtcWakeUpFlag(uint32_t nextRtcWakeUp);

/*!******************************************************************
 * \fn void setRtcPrescFlag(uint16_t rtcPresc)
 * \brief Set RTC prescale flag.
 *
 * \param[in]  rtcPresc
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setRtcPrescFlag(uint16_t rtcPresc);

/*!******************************************************************
 * \fn uint32_t getNextRtcWakeUpFlag(void)
 * \brief Get RTC next wake up flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval next_rtc_wakeup
 *******************************************************************/ 
extern uint32_t getNextRtcWakeUpFlag(void);

/*!******************************************************************
 * \fn uint8_t getRtcInUseForCsFlag(void)
 * \brief Get RTC in use for CS flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval rtc_in_use_for_cs
 *******************************************************************/ 
extern uint8_t getRtcInUseForCsFlag(void);

/*!******************************************************************
 * \fn uint8_t getRtcIrqFlag(void)
 * \brief Get RTC IRQ flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval rtc_irq														1 or 0
 *******************************************************************/ 
extern uint8_t getRtcIrqFlag(void);

/*!******************************************************************
 * \fn uint8_t getRtcInUseFlag(void)
 * \brief Get RTC in use flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval rtc_in_use														1 it's being used, else 0
 *******************************************************************/ 
extern uint8_t getRtcInUseFlag(void);

/*!******************************************************************
 * \fn uint16_t getRtcPrescFlag(void)
 * \brief Get RTC prescale flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval rtc_presc														
 *******************************************************************/ 
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
