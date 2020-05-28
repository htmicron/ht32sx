/**
* @file    SPIRIT_SDK_EMB_FLASH.h
* @author  AMG - RF Application team
* @version 3.2.1
* @date    March 26, 2018
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
* <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EMB_FLASH_API_H
#define __EMB_FLASH_API_H


/* Includes ------------------------------------------------------------------*/
#include "cube_hal.h"

#define DEBUG_FLASH 0

#define QUOTEME(a) #a

#if defined(__ICCARM__) || defined(__IAR_SYSTEMS_ASM__)
/**
  * @brief  SECTION
  *         Use the SECTION macro to assign data or code in a specific section.
  *         Usage:  SECTION(".my_section")
  */
#define SECTION(name)                  _Pragma(QUOTEME(location=name))

/**
  * @brief  NO_INIT_SECTION
  *         Use the NO_INIT_SECTION macro to declare a not initialized variable in RAM in
  *         in a specific section.
  *         Usage:  NO_INIT_SECTION(int my_no_init_var, "MySection")
  *         Usage:  NO_INIT_SECTION(uint16_t my_no_init_array[10], "MySection")
  */
#define NO_INIT_SECTION(var, sect)                  SECTION(sect) __no_init var

#else
//
//#ifdef __GNUC__
///**
//  * @brief  NO_INIT
//  *         Use the NO_INIT macro to declare a not initialized variable placed in RAM
//  *         Linker script has to make sure that section ".noinit" is not initialized
//  *         Usage:  NO_INIT(int my_no_init_var)
//  *         Usage:  NO_INIT(uint16_t my_no_init_array[10])
//  */
//#define NO_INIT(var)                    var  __attribute__((section(".noinit")))
//
///**
//  * @brief  NO_INIT_SECTION
//  *         Use the NO_INIT_SECTION macro to declare a not initialized variable.
//  *         In order to work properly this macro should be aligned with the linker file.
//  *         Usage:  NO_INIT_SECTION(int my_no_init_var, "MySection")
//  *         Usage:  NO_INIT_SECTION(uint16_t my_no_init_array[10], "MySection")
//  */
//#define NO_INIT_SECTION(var, sect)                   var __attribute__((section( sect ), zero_init))
//#else
//
//#error Neither ICCARM nor CC ARM detected. Define your macros.
//#endif
#endif

#ifdef __cplusplus
"C" {
#endif

  /**
  * @addtogroup SDK_EVAL_NUCLEO
  * @{
  */

  /**
  * @defgroup SDK_EMB_FLASH
  * @brief Management of Software Development Kit eval board FLASH.
  * @details See the file <i>@ref SDK_EMB_FLASH.h</i> for more details.
  * @{
  */

  /**
  * @brief nvm_data is the part of the FLASH where to store Sigfox recurrent data (like counters).
  * This memory is not direct handled by the user. It is up to the Sigfox libraries to handle this memory.
  * So, it strongly suggested to not operate in this area.
  */
#define FLASH_USER_START_ADDR   	nvm_data				/* Start @ of user Flash area */
#define FLASH_USER_END_ADDR		nvm_data+(sizeof(nvm_data))	/* End @ of user Flash area */

  /**
  * @brief board_data is the part of the FLASH where Sigfox board info are stored.
  * In this area are stored ID, PAC, RCZ, the Sigfox key and other informations.
  * The key could be encrypted or not.
  * Use the functions in the @nvm_api module in order to read/update these information.
  */
#define FLASH_BOARD_START_ADDR	board_data					/* Start @ of board data Flash area */
#define FLASH_BOARD_END_ADDR		board_data+(sizeof(board_data))	/* End @ of board data Flash area */

#ifdef ERASE_VALUE_IS_FF
#define FLASH_ERASE_VALUE 0xFF
#else
#define FLASH_ERASE_VALUE 0x00
#endif


  /*--STM32L053xx--*/
#if defined(STM32L053xx) || defined(STM32L052xx)
#define MAX_NO_OF_PAGES		32	/* Pages for sector */
#endif


  /*--STM32L152xE--*/
#if defined(STM32L152xE) || defined(STM32L151xBA)
#define MAX_NO_OF_PAGES		16	/* Pages for sector */
#endif


  /*--STM32F072xB--*/
#if defined(STM32F072xB)
#define MAX_NO_OF_PAGES		128	/* Pages for sector */
#endif


  /*--STM32F401xE--*/
#if defined(STM32F401xE)
#define MAX_NO_OF_PAGES		32	/* Pages for sector */
#define FLASH_PAGE_SIZE		(16*1024)

#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0,  16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1,  16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2,  16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3,  16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4,  64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#endif

/**
* @brief  FlashRead Status Enum
*/
typedef enum
{
  FLS_RW_OK		= 0x00,
  FLS_RW_ERROR	= 0x01,
  FLS_RW_OUT_OF_RANGE	= 0x02
} FLS_RW_StatusTypeDef;


extern uint8_t nvm_data[FLASH_PAGE_SIZE];
extern uint8_t board_data[FLASH_PAGE_SIZE];

/*
 * @brief  Reads cNbBytes from FLASH memory starting at nAddress and store a pointer to that in pcBuffer.
 *
 * @param nAddress starting address
 * @cNbBytes number of bytes to read
 * @pcBuffer the pointer to the data
 * @return The staus of the operation
*/
FLS_RW_StatusTypeDef FlashRead(uint32_t nAddress,  uint16_t cNbBytes,  uint8_t* pcBuffer);

/*
 * @brief  Writes cNbBytes pointed to pcBuffer to FLASH memory starting at nAddress.
 *
 * @param nAddress starting address
 * @cNbBytes number of bytes to write
 * @pcBuffer the pointer to the data
 * @eraseBeforeWrite if set to 1 erase the page before write bytes
 * @return The staus of the operation
*/
FLS_RW_StatusTypeDef FlashWrite(uint32_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer, uint8_t eraseBeforeWrite);

/*
 * @brief  Erase nPages from FLASH starting at nAddress
 *
 * @param nAddress starting address
 * @nPages number of pages to erase
 * @return The staus of the operation
*/
FLS_RW_StatusTypeDef FlashErase(uint32_t nAddress, uint32_t nPages);

/*
 * @brief  Check if nAddress is a valid address
 *
 * @param nAddress flash address
 * @return FLS_RW_OK if address is valid
*/
FLS_RW_StatusTypeDef CheckFlashAddress(uint32_t nAddress);

/*
 * @brief  Erase a page from Data Eeprom.
 *
 * @param nAddress flash address
 * @return FLS_RW_OK if address is valid
*/
FLS_RW_StatusTypeDef DataEepromErase(uint32_t nAddress);

/**
* @}
*/

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __EMB_FLASH_API_H */
/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/

