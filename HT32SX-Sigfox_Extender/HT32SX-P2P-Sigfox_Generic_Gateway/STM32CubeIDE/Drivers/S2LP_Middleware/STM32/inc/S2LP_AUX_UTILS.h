/**
* @file    S2LP_EVAL_Utils.h
* @author  VMA division - AMS
* @version 3.2.1
* @date    February 1, 2015
* @brief   Identification functions for S2LP DK. This is only useful to manage
* ST Eval kit and it is normally not needed in customer application
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
* <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
*/

#ifndef __S2LP_AUX_UTILS_H
#define __S2LP_AUX_UTILS_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/**
* @addtogroup S2LP_DK                   S2LP DK
* @{
*/


/**
* @defgroup SDK_S2LP_MANAGEMENT              SDK S2LP Management
* @{
*/

/**
* @defgroup SDK_S2LP_MANAGEMENT_FUNCTIONS    SDK S2LP Management Functions
* @{
*/

/**
* @defgroup IDENTIFICATION_FUNCTIONS      SDK S2LP Management Identification Functions
* @{
*/

uint32_t S2LPManagementComputeXtalFrequency(void);

uint32_t S2LPManagementGetXtalFrequency(void);
uint32_t S2LPGetFrequencyBand(void);

/**
* @}
*/



/**
* @}
*/


/**
* @}
*/

/**
* @}
*/

#endif /*__S2LP_AUX_UTILS_H */
/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
