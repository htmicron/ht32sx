/**
  ******************************************************************************
  * @file		           : gpio.h
  * @brief		         : This file contains all the functions prototypes for 
  *                      the gpio  
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

/**
 * @brief  Init shutdown pin on the uC and set it to 1 (Shutdown active).
 * @param  None.
 * @retval None.
 */
extern void S2LPShutdownInit(void);

/**
 * @brief  Puts at logic 1 the SDN pin.
 * @param  None.
 * @retval None.
 */
extern void S2LPShutdownEnter(void);

/**
 * @brief  Put at logic 0 the SDN pin.
 * @param  None.
 * @retval None.
 */
extern void S2LPShutdownExit(void);

/**
* @brief  check the logic (0 or 1) at the SDN pin.
* @param  None.
* @retval FlagStatus.
*/
extern uint8_t S2LPShutdownCheck(void);

/**
 * @brief  Configures MCU GPIO and EXTI Line for GPIOs.
 * @param  None.
 * @retval None.
 */
extern void S2LPIRQInit(void);

/**
 * @brief  Enables or disables the interrupt on GPIO .
 * @param  state Specifies the State.
 *         This parameter can be one of following parameters:
 *         @arg 0: disable
 *         @arg >0: enable
 * @param  edge_direction Specifies the sensitive edge.
 *         This parameter can be one of following parameters:
 *         @arg 0: falling edge
 *         @arg 1: rising edge
 * @retval None.
 */
extern void S2LPIRQEnable(uint8_t state, uint8_t edge_direction);

/*!******************************************************************
 * \fn uint8_t getLowPowerFlag(void)
 * \brief Get low power flag to check the S2LP state.
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval low_power								1 if it's low power, else 0
 *******************************************************************/ 
extern uint8_t getLowPowerFlag(void);

/*!******************************************************************
 * \fn void setS2lpIrqRaisedFlag(uint8_t s2lpIrqRaised)
 * \brief Get low power flag to check the S2LP state.
 *
 * \param[in]  s2lpIrqRaised								1 if it's raised, else 0                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/ 
extern void setS2lpIrqRaisedFlag(uint8_t s2lpIrqRaised);

/*!******************************************************************
 * \fn uint8_t getS2lpIrqRaisedFlag(void)
 * \brief Get S2LP IRQ raised flag.
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval s2lp_irq_raised								1 if it's raised, else 0
 *******************************************************************/ 
extern uint8_t getS2lpIrqRaisedFlag(void);

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
