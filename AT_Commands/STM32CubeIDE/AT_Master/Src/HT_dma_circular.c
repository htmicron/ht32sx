/**
  *
  * Copyright (c) 2020 HT Micron Semicondutors S.A.
	* Licensed under the Apache License, Version 2.0 (the "License");
	* you may not use this file except in compliance with the License.
	* You may obtain a copy of the License at
	* http://www.apache.org/licenses/LICENSE-2.0
	* Unless required by applicable law or agreed to in writing, software
	* distributed under the License is distributed on an "AS IS" BASIS,
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
  *
*/

#include "main.h"
#include "HT_dma_circular.h"
#include <string.h>

extern uint8_t DMA_RX_Buffer[DMA_RX_BUFFER_SIZE];
uint8_t UART_Buffer[UART_BUFFER_SIZE];

size_t Write;
size_t len, tocopy;
uint8_t *ptr;

uint8_t cont = 0;
uint32_t previous_len = 0;

uint8_t bufferAux[DMA_RX_BUFFER_SIZE];

void USART_IrqHandler (UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma) {

	volatile uint32_t tmp;                  /* Must be volatile to prevent optimizations */

	if ((huart->Instance->ISR & UART_FLAG_IDLE) && !(huart->Instance->ISR & UART_FLAG_RXNE) && (huart->Instance->RDR != 0x0)) /* if Idle flag is set */
	{
		//volatile uint32_t tmp;                  /* Must be volatile to prevent optimizations */
		hdma->Instance->CCR &= ~DMA_CCR_EN;       /* Disabling DMA will force transfer complete interrupt if enabled */
		DMA_IrqHandler(hdma);
		tmp = huart->Instance->ISR;                       /* Read status register */
		tmp = huart->Instance->RDR;                       /* Read data register */
		huart->Instance->RQR = 1 << 3;
	} else {
		tmp = huart->Instance->ISR;                       /* Read status register */
		tmp = huart->Instance->RDR;
	}
	tmp = tmp; /* to avoid warning */
}

void DMA_IrqHandler (DMA_HandleTypeDef *hdma) {

	typedef struct
	{
		__IO uint32_t ISR;   /*!< DMA interrupt status register */
		__IO uint32_t Reserved0;
		__IO uint32_t IFCR;  /*!< DMA interrupt flag clear register */
	} DMA_Base_Registers;

	DMA_Base_Registers *regs = (DMA_Base_Registers *)hdma->DmaBaseAddress;

	if(__HAL_DMA_GET_IT_SOURCE(hdma, DMA_IT_TC) != RESET)   // if the source is TC
	{
		/* Clear the transfer complete flag */
		regs->IFCR = DMA_FLAG_TC5 << hdma->ChannelIndex;

		/* Get the length of the data */
		len = DMA_RX_BUFFER_SIZE - hdma->Instance->CNDTR;

		/* Get number of bytes we can copy to the end of buffer */
		tocopy = UART_BUFFER_SIZE - Write;

		/* Check how many bytes to copy */
		if (tocopy > len) {
			tocopy = len;
		}

		/* Write received data for UART main buffer for manipulation later */

		ptr = DMA_RX_Buffer;
		memcpy(UART_Buffer + strlen((char *)UART_Buffer), ptr, tocopy);

		/* Prepare DMA for next transfer */
		/* Important! DMA stream won't start if all flags are not cleared first */

		regs->IFCR = 0x3FU << hdma->ChannelIndex; // clear all interrupts
		hdma->Instance->CMAR = (uint32_t)DMA_RX_Buffer;   /* Set memory address for DMA again */
		hdma->Instance->CNDTR = DMA_RX_BUFFER_SIZE;    /* Set number of bytes to receive */
		hdma->Instance->CCR |= DMA_CCR_EN;            /* Start DMA transfer */

		if(UART_Buffer[strlen((char *)UART_Buffer)-1] == ';')
			AT_setCmdStr(UART_Buffer);

	}
}

void AT_DMA_clearUartRxBuffer(void) {
	memset(UART_Buffer, 0, UART_BUFFER_SIZE);
	memset(DMA_RX_Buffer, 0, DMA_RX_BUFFER_SIZE);
	tocopy = 0;
	Write = 0;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
