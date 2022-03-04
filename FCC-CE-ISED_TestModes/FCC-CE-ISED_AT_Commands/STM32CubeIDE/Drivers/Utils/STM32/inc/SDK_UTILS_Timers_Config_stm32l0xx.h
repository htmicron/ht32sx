/**
* @file    SDK_EVAL_Timers.c
* @author  AMG - RF Application team
* @version 3.2.1
* @date    26-September-2016
* @brief   SDK EVAL timers configuration.
* @details
*
* This module allows the user to easily configure the STM32L timers.
* The functions that are provided are limited to the generation of an
* IRQ every time the timer elapses.
*
* <b>Example:</b>
* @code
*
*   ...
*
*   SdkEvalTimersTimConfig_ms(TIM2,60.0);
*
*   ...
*
*   SdkEvalTimersState(TIM2, ENABLE);          // the timer starts counting here
*
*   ...
*
*   SdkEvalTimersState(TIM2, DISABLE);         // timer stopped
*
*   ...
*
* @endcode
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SDK_EVAL_TIMERS_H
#define __SDK_EVAL_TIMERS_H

/* Includes ------------------------------------------------------------------*/

#include "cube_hal.h"


#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @addtogroup SDK_EVAL_NUCLEO
 * @{
 */

/**
 * @defgroup SDK_EVAL_Timers            SDK EVAL Timers
 * @brief Management of STM32L timers.
 * @details See the file <i>@ref SDK_EVAL_Timers.h</i> for more details.
 * @{
 */
   
   
#define SDK_UTILS_TIMER1 0
#define SDK_UTILS_TIMER2 0
#define SDK_UTILS_TIMER3 0
#define SDK_UTILS_TIMER4 0

 

/**
 * @defgroup SDK_EVAL_Timers_Exported_Constants         SDK EVAL Timers Exported Constants
 * @{
 */

/**
 * @brief MCU XO frequency(in KHz) definition
 */
#define CLOCK_FREQUENCY         32000

/**
 *@}
 */


/**
 * @defgroup SDK_EVAL_Timers_Exported_Macros            SDK EVAL Timers Exported Macros
 * @{
 */

/**
 * @brief  Configures the specified TIMER to raise an interrupt every TIME ms.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  TIME: timer duration in ms.
 *          This parameter is a float.
 * @retval None
 */
#define SdkEvalTimersTimConfig_ms(TIM_HANDLER_P , TIME)      {\
                                                        uint32_t n = (uint32_t)(TIME*CLOCK_FREQUENCY);\
                                                        uint16_t a,b;\
                                                        SdkEvalTimersFindFactors(n,&a,&b);\
                                                        SdkEvalTimersTimConfig(TIM_HANDLER_P,a-1,b-1);\
                                                      }

/**
 * @brief  Enables or Disables a specific Timer with its IRQ.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  NEWSTATE: specifies if a timer has to be enabled or disabled.
 *          This parameter is a float.
 * @retval None
 */
#define SdkEvalTimersState(TIM_HANDLER_P , NEWSTATE)      {((uint8_t)NEWSTATE)?(HAL_TIM_Base_Start_IT(TIM_HANDLER_P)):(HAL_TIM_Base_Stop_IT(TIM_HANDLER_P));}


/**
 * @brief  Set the counter of the specified TIMER.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @param  VALUE: value to set in the counter.
 *          This parameter is an uint32_t
 * @retval None
 */
#define SdkEvalTimersSetCounter(TIM_HANDLER_P,VALUE)   {__HAL_TIM_SET_COUNTER(TIM_HANDLER_P,VALUE);}


/**
 * @brief  Get the counter of the specified TIMER.
 * @param  TIMER: timer to be set.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @retval None
 */
#define SdkEvalTimersGetCounter(TIM_HANDLER_P)   __HAL_TIM_GET_COUNTER(TIM_HANDLER_P)

/**
 * @brief  Resets the counter of a specific timer.
 * @param  TIMER: timer to be reset.
 *          This parameter can be a pointer to @ref TIM_TypeDef
 * @retval None
 */
#define SdkEvalTimersResetCounter(TIM_HANDLER_P)      {__HAL_TIM_SET_COUNTER(TIM_HANDLER_P,0);}


/**
 *@}
 */


/**
 * @defgroup SDK_EVAL_Timers_Exported_Functions         SDK EVAL Timers Exported Functions
 * @{
 */
   
#ifdef USE_SYSTICK_DELAY
void SysTick_Handler(void);
void SdkDelayMs(volatile uint32_t lTimeMs);
uint32_t SdkGetCurrentSysTick(void);
void SdkStartSysTick(void);
#endif

void SdkEvalTimersFindFactors(uint32_t n, uint16_t *pnPrescaler, uint16_t *pnCounter);
void SdkEvalTimersTimConfig(TIM_HandleTypeDef*  TIM_TimeBaseStructure, uint16_t nPrescaler, uint16_t nPeriod);


/**
 *@}
 */

/**
 *@}
 */

/**
 *@}
 */

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2016 STMicroelectronics *****END OF FILE****/
