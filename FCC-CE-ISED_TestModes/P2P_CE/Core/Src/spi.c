/**
  ******************************************************************************
  * File Name          : SPI.c
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

/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspInit 0 */

  /* USER CODE END SPI1_MspInit 0 */
    /* SPI1 clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PB4     ------> SPI1_MISO
    PB3     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = RADIO_SPI_MISO_PIN_Pin|RADIO_SPI_SCK_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RADIO_SPI_MOSI_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(RADIO_SPI_MOSI_PIN_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_MspInit 1 */

  /* USER CODE END SPI1_MspInit 1 */
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

  if(spiHandle->Instance==SPI1)
  {
  /* USER CODE BEGIN SPI1_MspDeInit 0 */

  /* USER CODE END SPI1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PB4     ------> SPI1_MISO
    PB3     ------> SPI1_SCK
    PA7     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOB, RADIO_SPI_MISO_PIN_Pin|RADIO_SPI_SCK_PIN_Pin);

    HAL_GPIO_DeInit(RADIO_SPI_MOSI_PIN_GPIO_Port, RADIO_SPI_MOSI_PIN_Pin);

  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

S2LPStatus S2LPSpiWriteRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer) {
	uint8_t tx_buff[2]={WRITE_HEADER,cRegAddress};
	uint8_t rx_buff[255];
	S2LPStatus status;

	SPI_ENTER_CRITICAL();

	/* Puts the SPI chip select low to start the transaction */
	RadioSpiCSLow();

	HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
	HAL_SPI_TransmitReceive(&hspi1, pcBuffer, &rx_buff[2], cNbBytes, 1000);

	/* Puts the SPI chip select high to end the transaction */
	RadioSpiCSHigh();

	SPI_EXIT_CRITICAL();

	((uint8_t*)&status)[1]=rx_buff[0];
	((uint8_t*)&status)[0]=rx_buff[1];

	return status;
}

S2LPStatus S2LPSpiCommandStrobes(uint8_t cCommandCode) {
	uint8_t tx_buff[2]={COMMAND_HEADER,cCommandCode};
	uint8_t rx_buff[2];
	S2LPStatus status;

	SPI_ENTER_CRITICAL();
	RadioSpiCSLow();
	HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
	RadioSpiCSHigh();
	SPI_EXIT_CRITICAL();

	((uint8_t*)&status)[1]=rx_buff[0];
	((uint8_t*)&status)[0]=rx_buff[1];

	return status;
}

S2LPStatus S2LPSpiWriteFifo(uint8_t cNbBytes, uint8_t* pcBuffer) {
	uint8_t tx_buff[2]={WRITE_HEADER,LINEAR_FIFO_ADDRESS};
	uint8_t rx_buff[130];
	S2LPStatus status;

	SPI_ENTER_CRITICAL();
	RadioSpiCSLow();
	HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
	HAL_SPI_TransmitReceive(&hspi1, pcBuffer, &rx_buff[2], cNbBytes, 1000);
	RadioSpiCSHigh();
	SPI_EXIT_CRITICAL();

	((uint8_t*)&status)[1]=rx_buff[0];
	((uint8_t*)&status)[0]=rx_buff[1];

	return status;
}

S2LPStatus S2LPSpiReadRegisters(uint8_t cRegAddress, uint8_t cNbBytes, uint8_t* pcBuffer) {
	uint8_t tx_buff[255]={READ_HEADER,cRegAddress};
	uint8_t rx_buff[2];
	S2LPStatus status;

	SPI_ENTER_CRITICAL();
	RadioSpiCSLow();
	HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
	HAL_SPI_TransmitReceive(&hspi1, tx_buff, pcBuffer, cNbBytes, 1000);
	RadioSpiCSHigh();
	SPI_EXIT_CRITICAL();

	((uint8_t*)&status)[1]=rx_buff[0];
	((uint8_t*)&status)[0]=rx_buff[1];

	return status;
}

S2LPStatus S2LPSpiReadFifo(uint8_t cNbBytes, uint8_t* pcBuffer) {
	uint8_t tx_buff[130]={READ_HEADER,LINEAR_FIFO_ADDRESS};
	uint8_t rx_buff[2];
	S2LPStatus status;

	SPI_ENTER_CRITICAL();
	RadioSpiCSLow();
	HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 2, 1000);
	HAL_SPI_TransmitReceive(&hspi1, tx_buff, pcBuffer, cNbBytes, 1000);
	RadioSpiCSHigh();
	SPI_EXIT_CRITICAL();

	((uint8_t*)&status)[1]=rx_buff[0];
	((uint8_t*)&status)[0]=rx_buff[1];


	return status;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
