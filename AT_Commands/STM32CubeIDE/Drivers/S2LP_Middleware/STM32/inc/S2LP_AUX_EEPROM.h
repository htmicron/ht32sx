/**
* @file    SPIRIT_SDK_EEPROM.h
* @author  LowPower RF BU - AMG
* @version 3.2.1
* @date    December, 2018
* @brief   SDK EVAL eeprom management
* @details This module exports API to manage the EEPROM of the eval boards.
*          Data stored in the EEPROM are mainly some manifacturing infos,
*          and informations that can be useful when developing applications
*          with the daughter board. Some of them are the RF band, the offset
*          of the carrier from the nominal frequency and the XTAL frequency.
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
#ifndef SPIRIT_SDK_EEPROM_H_
#define SPIRIT_SDK_EEPROM_H_

#include "cube_hal.h"

#ifdef __cplusplus
  "C" {
#endif


/**
* @brief  Read a page of the EEPROM.
*         A page size is 32 bytes.
*         The pages are 256.
*         Page 0 address: 0x0000
*         Page 1 address: 0x0020
*         ...
*         Page 255 address: 0x1FE0
* @param  None
* @retval None
*/
uint8_t EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif


#endif
 /******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/

