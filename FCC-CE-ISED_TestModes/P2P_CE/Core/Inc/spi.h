/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "HT_P2P_s2lp_types.h"
#include "HT_P2P_s2lp_gpio.h"
/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN Private defines */

#define SPI_ENTER_CRITICAL()          RadioGpioInterruptCmd(RADIO_GPIO_IRQ,0x04,0x04,DISABLE);
#define SPI_EXIT_CRITICAL()           RadioGpioInterruptCmd(RADIO_GPIO_IRQ,0x04,0x04,ENABLE);

#define RadioSpiCSLow()        HAL_GPIO_WritePin(RADIO_SPI_CS_PIN_GPIO_Port, RADIO_SPI_CS_PIN_Pin, GPIO_PIN_RESET)
#define RadioSpiCSHigh()       HAL_GPIO_WritePin(RADIO_SPI_CS_PIN_GPIO_Port, RADIO_SPI_CS_PIN_Pin, GPIO_PIN_SET)

#define HEADER_WRITE_MASK     0x00 /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01 /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00 /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80 /*!< Command mask for header byte*/

#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/
#define LINEAR_FIFO_ADDRESS 0xFF  /*!< Linear FIFO address*/

/* USER CODE END Private defines */

void MX_SPI1_Init(void);

/* USER CODE BEGIN Prototypes */


/*!******************************************************************
 * \fn S2LPStatus S2LPSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
 * \brief Write single or multiple registers.
 *
 * \param[in] uint8_t cRegAddress					Base register's address to be write
 * \param[in] uint8_t cNbBytes						Number of registers and bytes to be written
 * \param[in] uint8_t* pcBuffer					Pointer to the buffer which is going to be written.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
S2LPStatus S2LPSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);

/*!******************************************************************
 * \fn S2LPStatus S2LPSpiCommandStrobes(uint8_t cCommandCode)
 * \brief Send a command to S2LP.
 *
 * \param[in] uint8_t cCommandCode					Command to be sent.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
S2LPStatus S2LPSpiCommandStrobes(uint8_t cCommandCode);

/*!******************************************************************
 * \fn S2LPStatus S2LPSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer)
 * \brief Write data in S2LP FIFO.
 *
 * \param[in] uint8_t cNbBytes						Number of registers and bytes to be read
 * \param[oin] uint8_t* pcBuffer					Pointer to the buffer to be written in S2LP FIFO.
 *
 * \retval none
 *******************************************************************/
S2LPStatus S2LPSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer);

/*!******************************************************************
 * \fn S2LPStatus S2LPSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
 * \brief Read single or multiple registers.
 *
 * \param[in] uint8_t cRegAddress					Base register's address to be read
 * \param[in] uint8_t cNbBytes						Number of registers and bytes to be read
 * \param[out] uint8_t* pcBuffer						Pointer to the buffer of registers' values read
 *
 * \retval none
 *******************************************************************/
S2LPStatus S2LPSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer);


/*!******************************************************************
 * \fn S2LPStatus S2LPSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer)er)
 * \brief Read data in S2LP FIFO.
 *
 * \param[in] uint8_t cNbBytes						Number of registers and bytes to be read
 * \param[out] uint8_t* pcBuffer					Pointer to the buffer read.
 *
 * \retval none
 *******************************************************************/
S2LPStatus S2LPSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer);

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
