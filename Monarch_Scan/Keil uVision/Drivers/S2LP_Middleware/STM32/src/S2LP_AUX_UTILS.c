/**
 * @file    S2LP_EVAL_Utils.c
 * @author  LowPower RF BU - AMG
 * @version 3.3.0
 * @date    Oct 1, 2018
 * @brief   Identification functions for S2LP DK.
 * @details
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
 * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_AUX_UTILS.h"

/**
* @addtogroup S2LP_DK                   S2LP DK
* @{
*/


/**
* @brief This flag is used to synchronize the TIM3 ISR with the XtalMeasurement routine.
*/
static volatile uint8_t s_RfModuleBand = 0;
static volatile uint32_t s_RfXtalFrequency=50000000;
static volatile S2LPDaughterBoard _daughterBoard = FKI_SERIES;

/**
* @defgroup SDK_S2LP_MANAGEMENT_FUNCTIONS    SDK S2LP Management Functions
* @{
*/

/**
* @defgroup IDENTIFICATION_FUNCTIONS      SDK S2LP Management Identification Functions
* @{
*/

uint32_t S2LPManagementComputeXtalFrequency(void)
{
  s_RfXtalFrequency=50000000;
  return s_RfXtalFrequency;
}

uint32_t S2LPManagementGetXtalFrequency(void)
{
  return s_RfXtalFrequency;
}

/**
* @}
*/



/**
* @}
*/


/**
* @}
*/

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
