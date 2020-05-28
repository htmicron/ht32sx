/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */

/**
 * @brief  SPI function to write registers of a slave device
 * @param  RegisterAddr: register address
 * @param  NumByteToRead: number of byte to write
 * @param  pBuffer: buffer contains data to write
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */

extern uint16_t S2LPSpiWriteRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer);

/**
 * @brief  SPI function to read registers from a slave device
 * @param  RegisterAddr: register address
 * @param  NumByteToRead: number of byte to read
 * @param  pBuffer: buffer to retrieve data from a slave
 * @retval ErrorStatus: error status @ref ErrorStatus
 *         This parameter can be: SUCCESS or ERROR.
 */
extern uint16_t S2LPSpiReadRegisters(uint8_t RegisterAddr, uint8_t NumByteToRead, uint8_t* pBuffer);
extern uint16_t S2LPSpiCommandStrobes(uint8_t command);
extern uint16_t S2LPSpiReadFifo(uint8_t n_bytes, uint8_t* buffer);
extern uint16_t S2LPSpiWriteFifo(uint8_t n_bytes, uint8_t* buffer);
extern void S2LPSetSpiInUse(uint8_t state);
extern uint8_t S2LPGetSpiInUse(void);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
