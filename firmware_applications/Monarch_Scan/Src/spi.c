/**
  ******************************************************************************
  * @file		           : SPI.c
  * @brief 	  	       : This file provides code for the configuration
  *                      of the SPI instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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

#define HEADER_WRITE_MASK     0x00 /*!< Write mask for header byte*/
#define HEADER_READ_MASK      0x01 /*!< Read mask for header byte*/
#define HEADER_ADDRESS_MASK   0x00 /*!< Address mask for header byte*/
#define HEADER_COMMAND_MASK   0x80 /*!< Command mask for header byte*/

#define LINEAR_FIFO_ADDRESS 0xFF  /*!< Linear FIFO address*/

#define BUILT_HEADER(add_comm, w_r) (add_comm | w_r)  /*!< macro to build the header byte*/
#define WRITE_HEADER    BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_WRITE_MASK) /*!< macro to build the write header byte*/
#define READ_HEADER     BUILT_HEADER(HEADER_ADDRESS_MASK, HEADER_READ_MASK)  /*!< macro to build the read header byte*/
#define COMMAND_HEADER  BUILT_HEADER(HEADER_COMMAND_MASK, HEADER_WRITE_MASK) /*!< macro to build the command header byte*/

#define SPI_ENTER_CRITICAL()           __disable_irq()
#define SPI_EXIT_CRITICAL()            __enable_irq()

#define SdkEvalSPICSLow()        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define SdkEvalSPICSHigh()       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)

#define WAIT_FOR_SPI_TC()               {while(!__HAL_DMA_GET_FLAG(hspi1.hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(hspi1.hdmarx)));\
                                          do{HAL_DMA_IRQHandler(hspi1.hdmarx);\
                                          HAL_DMA_IRQHandler(hspi1.hdmatx);\
                                          }while(HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY);}

																					
static volatile uint8_t spi_in_use = 0;
static uint8_t waiting_irq = 0;
																					
static uint8_t tx_buff[128];
static uint8_t rx_buff[128];

/* USER CODE END 0 */

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
    GPIO_InitStruct.Pin = S2LP_SPI_MISO_PIN_Pin|S2LP_SPI_SCLK_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = S2LP_SPI_MOSI_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(S2LP_SPI_MOSI_PIN_GPIO_Port, &GPIO_InitStruct);

    /* SPI1 DMA Init */
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA1_Channel3;
    hdma_spi1_tx.Init.Request = DMA_REQUEST_1;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi1_tx);

    /* SPI1_RX Init */
    hdma_spi1_rx.Instance = DMA1_Channel2;
    hdma_spi1_rx.Init.Request = DMA_REQUEST_1;
    hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_rx.Init.Mode = DMA_NORMAL;
    hdma_spi1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(spiHandle,hdmarx,hdma_spi1_rx);

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
    HAL_GPIO_DeInit(GPIOB, S2LP_SPI_MISO_PIN_Pin|S2LP_SPI_SCLK_PIN_Pin);

    HAL_GPIO_DeInit(S2LP_SPI_MOSI_PIN_GPIO_Port, S2LP_SPI_MOSI_PIN_Pin);

    /* SPI1 DMA DeInit */
    HAL_DMA_DeInit(spiHandle->hdmatx);
    HAL_DMA_DeInit(spiHandle->hdmarx);
  /* USER CODE BEGIN SPI1_MspDeInit 1 */

  /* USER CODE END SPI1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

uint16_t S2LPSpiReadRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=READ_HEADER;
  tx_buff[1]=address;

  uint16_t status;
	
  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();

	HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

	
  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  for(uint32_t i=0;i<n_bytes;i++)
  {
    buffer[i]=rx_buff[i+2];
  }

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t S2LPSpiWriteRegisters(uint8_t address, uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=WRITE_HEADER;
  tx_buff[1]=address;

  uint16_t status = 0;

  for(uint32_t i=0;i<n_bytes;i++)
  {
    tx_buff[i+2]=buffer[i];
  }

  SPI_ENTER_CRITICAL();

  /* Puts the SPI chip select low to start the transaction */
  SdkEvalSPICSLow();

	HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2+n_bytes);
	WAIT_FOR_SPI_TC();
	
  /* Puts the SPI chip select high to end the transaction */
  SdkEvalSPICSHigh();

  SPI_EXIT_CRITICAL();

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];
	
  return status;

}

uint16_t S2LPSpiCommandStrobes(uint8_t command)
{
  tx_buff[0]=COMMAND_HEADER;
  tx_buff[1]=command;


  uint16_t status;

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();

  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2);
  WAIT_FOR_SPI_TC();

  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

uint16_t S2LPSpiReadFifo(uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=READ_HEADER;
  tx_buff[1]=LINEAR_FIFO_ADDRESS;

  uint16_t status;

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();

  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  for(uint32_t i=0;i<n_bytes;i++)
  {
    buffer[i]=rx_buff[i+2];
  }
  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];


  return status;
}

uint16_t S2LPSpiWriteFifo(uint8_t n_bytes, uint8_t* buffer)
{
  tx_buff[0]=WRITE_HEADER;
  tx_buff[1]=LINEAR_FIFO_ADDRESS;

  uint16_t status;

  for(uint32_t i=0;i<n_bytes;i++)
  {
    tx_buff[i+2]=buffer[i];
  }

  SPI_ENTER_CRITICAL();
  SdkEvalSPICSLow();

  HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buff, rx_buff, 2+n_bytes);
  WAIT_FOR_SPI_TC();

  SdkEvalSPICSHigh();
  SPI_EXIT_CRITICAL();

  ((uint8_t*)&status)[1]=rx_buff[0];
  ((uint8_t*)&status)[0]=rx_buff[1];

  return status;
}

__weak void SdkEvalSpiRawTC(void){}

void S2LPSpiRaw(uint8_t n_bytes, uint8_t* in_buffer, uint8_t* out_buffer, uint8_t can_return_bef_tx)
{
  uint8_t* out_buffer_=out_buffer;

  if(out_buffer==NULL)
      out_buffer_=rx_buff;

  spi_in_use = 1;

  if(can_return_bef_tx)
  {
    waiting_irq=1;
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 1); //pode estar errado DMA1_Channel2_3_IRQn?
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

  }
  else
  {
    waiting_irq=0;
    HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
    SPI_ENTER_CRITICAL();
  }

  SdkEvalSPICSLow();

  HAL_SPI_TransmitReceive_DMA(&hspi1, in_buffer, out_buffer_, n_bytes);

  if(!can_return_bef_tx)
  {
    WAIT_FOR_SPI_TC();

    SdkEvalSPICSHigh();
    SPI_EXIT_CRITICAL();
  }

  spi_in_use = 0;
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(waiting_irq)
  {
    SdkEvalSPICSHigh();
    waiting_irq=0;
    SdkEvalSpiRawTC();
  }
}

void ST_MCU_API_SpiRaw(uint8_t n_bytes, uint8_t* in_buffer, uint8_t* out_buffer, uint8_t can_return_bef_tx)
{
  /* in this implementation we are not interested in the value of the can_return_bef_tx flag.
  We always pass 0 to the S2LPSpiRaw so that the CPU will wait the DMA for the end of transfer. */
  S2LPSpiRaw(n_bytes,in_buffer,out_buffer,0);
}

void S2LPSetSpiInUse(uint8_t state)
{
  spi_in_use = state;
}

uint8_t S2LPGetSpiInUse(void)
{
  return spi_in_use;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
