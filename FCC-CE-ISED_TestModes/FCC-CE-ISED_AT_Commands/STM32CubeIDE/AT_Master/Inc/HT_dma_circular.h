/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

*/

/*!
 * \file HT_dma_circular.h
 * \brief DMA Circular HT32SX iMCP SiP Sigfox
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date July 17, 2020
 *
 * This file defines a DMA Circular for the AT Commands application.
 */

#ifndef HT_DMA_CIRCULAR_H
#define HT_DMA_CIRCULAR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"

/* Defines  ------------------------------------------------------------------*/
#define DMA_RX_BUFFER_SIZE          36
#define UART_BUFFER_SIZE            36

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn extern void USART_IrqHandler (UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma)
 * \brief Function that will handle all the USART IT.
 *
 * \param[in]  UART_HandleTypeDef *huart		Huart handle.
 * \param[in]  DMA_HandleTypeDef *hdma			DMA handle.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void USART_IrqHandler (UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma);

/*!******************************************************************
 * \fn extern void DMA_IrqHandler (DMA_HandleTypeDef *hdma)
 * \brief Function that will handle all the DMA IT.
 *
 * \param[in]  DMA_HandleTypeDef *hdma			DMA handle.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void DMA_IrqHandler (DMA_HandleTypeDef *hdma);

/*!******************************************************************
 * \fn extern void AT_DMA_clearUartRxBuffer(void)
 * \brief Reset the USART RX buffer.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_DMA_clearUartRxBuffer(void);

#endif

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/

