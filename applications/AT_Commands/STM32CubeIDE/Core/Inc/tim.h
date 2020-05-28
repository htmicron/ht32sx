/**
  ******************************************************************************
  * File Name          : TIM.h
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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
#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "gpio.h"
#include "rtc.h"
#include "usart.h" 
#include "st_rf_api.h"
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN Private defines */

/**
 * @brief  Enables or Disables a specific Timer with its IRQ.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  NEWSTATE: specifies if a timer has to be enabled or disabled.
 *          This parameter is a float.
 * @retval None
 */
#define SdkEvalTimersState(TIM_HANDLER_P , NEWSTATE)      {((uint8_t)NEWSTATE)?(HAL_TIM_Base_Start_IT(TIM_HANDLER_P)):(HAL_TIM_Base_Stop_IT(TIM_HANDLER_P));}

/* USER CODE END Private defines */

void MX_TIM6_Init(void);

/* USER CODE BEGIN Prototypes */

/**
* @brief  Configures the specified timer to raise an interrupt every time the counter
*         reaches the nPeriod value counting with a prescaler of nPrescaler.
* @note   The specified timer is configured but not enabled.
* @param  TIM_TimeBaseStructure Timer Handler of the timer to be set.
*          This parameter can be a pointer to @ref TIM_HandleTypeDef .
* @param  nPrescaler prescaler factor.
* @param  nPeriod period factor.
* @retval None.
*/
extern void SdkEvalTimersTimConfig(TIM_HandleTypeDef* TIM_TimeBaseStructure, uint16_t nPrescaler, uint16_t nPeriod);

/**
* @brief  Computes two integer value prescaler and period such that Cycles = prescaler * period.
* @param  lCycles the specified cycles for the desired timer value.
* @param  pnPrescaler prescaler factor.
* @param  pnCounter period factor.
* @retval None.
*/
extern void SdkEvalTimersFindFactors(uint32_t lCycles, uint16_t *pnPrescaler, uint16_t *pnCounter);

/*!******************************************************************
 * \fn void setNotifyEndFlag(uint8_t notifyEnd)
 * \brief Set notify end flag.
 *
 * \param[in]  notifyEnd																		1 or 0
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setNotifyEndFlag(uint8_t notifyEnd);

/*!******************************************************************
 * \fn void setNIntermediateTimIrqFlag(uint8_t nIntermediateTimeIrq)
 * \brief Set N intermediate TIM IRQ flag.
 *
 * \param[in]  nIntermediateTimeIrq													1 or 0
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setNIntermediateTimIrqFlag(uint8_t nIntermediateTimeIrq);

/*!******************************************************************
 * \fn uint8_t getNotifyEndFlag(void)
 * \brief Get notify end flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval notify_end																				1 or 0
 *******************************************************************/ 
extern uint8_t getNotifyEndFlag(void);

/*!******************************************************************
 * \fn uint32_t getNIntermediateTimIrqFlag(void);
 * \brief Get N intermediate TIM IRQ flag.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval n_intermediate_tim_irq														 1 or 0
 *******************************************************************/ 
extern uint32_t getNIntermediateTimIrqFlag(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
