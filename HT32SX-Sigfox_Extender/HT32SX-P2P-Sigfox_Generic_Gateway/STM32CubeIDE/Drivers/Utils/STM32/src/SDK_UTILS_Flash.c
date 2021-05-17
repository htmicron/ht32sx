/**
* @file    SDK_UTILS_Flash.c
* @author  AMS RF application team
* @version 1.0.0
* @date    December, 2018
* @brief   SDK EVAL flash management
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
//#include "SDK_EVAL_Config.h"
#include "SDK_UTILS_Flash.h"
#include "stm32l0xx_hal_flash_ex.h"
#include "usart.h"

uint8_t board_data[FLASH_PAGE_SIZE] __attribute__((section(".noinit.sigfox_board_data")));
uint8_t nvm_data[FLASH_PAGE_SIZE] __attribute__((section(".noinit.sigfox_nvm_data")));

/**
* @addtogroup SDK_EVAL_NUCLEO
* @{
*/


/**
* @defgroup SDK_FLASH              SDK FLASH Management
* @{
*/

static FLASH_EraseInitTypeDef EraseInitStruct; /* Variable used for Erase procedure */
uint32_t  PAGEError = 0;

/**
* @defgroup FLASH Functions
* @{
*/

#if defined(STM32F401xE)
static uint32_t GetSector(uint32_t address)
{
  uint32_t sector = 0;

  if((address < ADDR_FLASH_SECTOR_1) && (address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;
  }
  else if((address < ADDR_FLASH_SECTOR_2) && (address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;
  }
  else if((address < ADDR_FLASH_SECTOR_3) && (address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;
  }
  else if((address < ADDR_FLASH_SECTOR_4) && (address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;
  }
  else if((address < ADDR_FLASH_SECTOR_5) && (address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;
  }
  else if((address < ADDR_FLASH_SECTOR_6) && (address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;
  }
  else if((address < ADDR_FLASH_SECTOR_7) && (address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;
  }
  else if(address == ADDR_FLASH_SECTOR_7)
  {
    sector = FLASH_SECTOR_7;
  }

  return sector;
}
#endif

static uint32_t GetNumberOfPagesByBytes(uint32_t nBytesCount)
{
  uint32_t nRet, nTmp;

  nTmp = nBytesCount % FLASH_PAGE_SIZE;
  nRet = (!nTmp)?(nBytesCount/FLASH_PAGE_SIZE):(nBytesCount/FLASH_PAGE_SIZE)+1;

  return nRet;
}

FLS_RW_StatusTypeDef FlashRead(uint32_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer)
{
  FLS_RW_StatusTypeDef frRetStatus = FLS_RW_OK;
  volatile uint64_t tmp;
  uint8_t i, count4;
  uint16_t pageIdx;

#if DEBUG_FLASH
  nAddress = FLASH_USER_START_ADDR;
#endif
	
  //	if(cNbBytes > FLASH_PAGE_SIZE*MAX_NO_OF_PAGES)
  //		frRetStatus = FLS_RW_OUT_OF_RANGE;

  if(pcBuffer == NULL)
    frRetStatus = FLS_RW_ERROR;

  if(frRetStatus == FLS_RW_OK)
  {
    for(i=0; i<cNbBytes; i++)
    {
      count4  = (i-((i/4)*4));	/* Counts 0...3 and restarts */
      pageIdx = (i/4)*4;		/* Every 4 bytes moves ahead */
	
	if(CheckFlashAddress(nAddress+pageIdx) == FLS_RW_OK)
	{
	  tmp = *((__IO uint32_t *)(nAddress+pageIdx));
		pcBuffer[i] = (tmp&(0xFF000000>>(count4*8)))>>(24-(count4*8));
	}
	else
	{
	  frRetStatus = FLS_RW_ERROR;
	  break;
	}
    }
  }
	
  return frRetStatus;
}

FLS_RW_StatusTypeDef FlashWrite(uint32_t nAddress, uint16_t cNbBytes, uint8_t* pcBuffer, uint8_t eraseBeforeWrite)
{
  uint8_t i, count4;
  uint16_t pageIdx;
  uint32_t temp_word;
  uint32_t __attribute__ ((unused)) error_code;

  FLS_RW_StatusTypeDef frRetStatus = FLS_RW_OK;
	
  //	if(cNbBytes > FLASH_PAGE_SIZE*MAX_NO_OF_PAGES)
  //		frRetStatus = FLS_RW_OUT_OF_RANGE;

  if(GetNumberOfPagesByBytes(cNbBytes) > MAX_NO_OF_PAGES)
    frRetStatus = FLS_RW_OUT_OF_RANGE;

  if(pcBuffer == NULL)
    frRetStatus = FLS_RW_ERROR;

#if DEBUG_FLASH
  nAddress = FLASH_USER_START_ADDR;
#endif
	
  if (eraseBeforeWrite) {
		frRetStatus = FlashErase(nAddress, GetNumberOfPagesByBytes(cNbBytes));
	}
		
  if(frRetStatus == FLS_RW_OK)
  {
    temp_word = 0;

    /* Unlock the Flash to enable the flash control register access */
    HAL_FLASHEx_DATAEEPROM_Unlock();
    
		for(i=0; i<cNbBytes; i++)
    {
      count4  = i-((i/4)*4);	/* Counts 0...3 and restarts */
      pageIdx = (i/4)*4; 	/* Every 4 bytes writes page */

      temp_word |= ((uint32_t)pcBuffer[i])<<(24-(8*count4));

      if((i == cNbBytes-1) || count4 == 3 )/* Write every 4 bytes or if bytes in args are less than 4 */
      {
	  if (HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAM_WORD, nAddress+pageIdx, temp_word) == HAL_OK)
	  {
	    temp_word = 0;
	  }
	  else
	  {
	    error_code = HAL_FLASH_GetError();
	    frRetStatus = FLS_RW_ERROR;

	    break;
	  }
      }
    }
		
    HAL_FLASHEx_DATAEEPROM_Lock();
  }

  return frRetStatus;
}

FLS_RW_StatusTypeDef DataEepromErase(uint32_t nAddress) {
  uint32_t __attribute__ ((unused)) error_code;
  FLS_RW_StatusTypeDef frRetStatus = FLS_RW_ERROR;
	
  /* Unlock the Flash to enable the flash control register access */
  if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK)
  {
    if(HAL_FLASHEx_DATAEEPROM_Erase(nAddress) == HAL_OK)
    {
      if(HAL_FLASHEx_DATAEEPROM_Lock() == HAL_OK)
				frRetStatus = FLS_RW_OK;
    }
  }

  if(frRetStatus == FLS_RW_ERROR)
  {
    error_code = HAL_FLASH_GetError();
  }
	
  return FLS_RW_OK;
}

FLS_RW_StatusTypeDef FlashErase(uint32_t nAddress, uint32_t nPages)
{
  uint32_t __attribute__ ((unused)) error_code;
  FLS_RW_StatusTypeDef frRetStatus = FLS_RW_ERROR;
	
  /* Unlock the Flash to enable the flash control register access */
  if(HAL_FLASHEx_DATAEEPROM_Unlock() == HAL_OK)
  {
#if defined(STM32F401xE)
    EraseInitStruct.TypeErase		= FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector		= GetSector(nAddress);
    EraseInitStruct.NbSectors		= nPages;
    EraseInitStruct.VoltageRange	= FLASH_VOLTAGE_RANGE_3;
#else
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = nAddress;
    EraseInitStruct.NbPages     = nPages;
#endif

    if(HAL_FLASHEx_DATAEEPROM_Erase(nAddress) == HAL_OK)
    {
      if(HAL_FLASHEx_DATAEEPROM_Lock() == HAL_OK)
				frRetStatus = FLS_RW_OK;
    }
  }

  if(frRetStatus == FLS_RW_ERROR)
  {
    error_code = HAL_FLASH_GetError();
  }

  return FLS_RW_OK;
}

FLS_RW_StatusTypeDef CheckFlashAddress(uint32_t nAddress)
{
#if defined(STM32F401xE)
  if(IS_FLASH_ADDRESS(nAddress))
#else
//  if(IS_FLASH_PROGRAM_ADDRESS(nAddress))

#endif
    return FLS_RW_OK;
  //else
    //return FLS_RW_ERROR;
}

/**
* @}
*/


/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE*****/

