/**
 ******************************************************************************
 * File Name          : gpio.c
 * Description        : This file provides code for the configuration
 *                      of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
static volatile uint8_t low_power = 1;
static volatile uint8_t s2lp_irq_raised = 0;

GPIO_PinState csd_pin_inst, cps_pin_inst,ctx_pin_inst;
/* USER CODE END 1 */

/** Configure pins as 
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(S2LP_SPI_CS_PIN_GPIO_Port, S2LP_SPI_CS_PIN_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = S2LP_SPI_CS_PIN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(S2LP_SPI_CS_PIN_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : PB8 */
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB2 */
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

}

/* USER CODE BEGIN 2 */

#ifdef DEBUG_LOW_POWER

static void setGpioLowPower(void)
{
	/* For STM32, gpio power consumption is reduced when GPIOs are configured as
  no pull - analog, during this configuration we have to sure thath wake-up pins
  still remain as digital inputs*/

	/* Build the first part of the init structre for all GPIOs and ports*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode       = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull       = GPIO_NOPULL;
	GPIO_InitStructure.Speed      = GPIO_SPEED_HIGH;

	/* -------------------------------- PORT A ------------------------------------- */
	/* SDN , CS S2-LP, CS E2PROM, UART RX/TX, BUTTON1 */
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_8) & (~GPIO_PIN_15) &
			(~GPIO_PIN_9) & (~GPIO_PIN_2) & (~GPIO_PIN_3);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* -------------------------------- PORT B ------------------------------------- */
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_4);
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* -------------------------------- PORT C ------------------------------------- */
	/* IRQ Pin + TCXO Enable */
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_0) & (~GPIO_PIN_7) & (~GPIO_PIN_13);
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* -------------------------------- PORT D ------------------------------------- */
	GPIO_InitStructure.Pin= GPIO_PIN_All;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

}

static void setGpioRestore(void)
{
	/* For STM32, restore every gpio, previosly set as analog as digital */

	/* Restore all Gpio CLKs */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* SPI_MOSI/MISO */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode       = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull       = GPIO_PULLUP;
	GPIO_InitStructure.Speed      = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate  = GPIO_AF0_SPI1; //EEPROM
	GPIO_InitStructure.Pin        = GPIO_PIN_7 | GPIO_PIN_6; //EEPROM
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI_CLK*/
	GPIO_InitStructure.Mode       = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull       = GPIO_PULLUP;
	GPIO_InitStructure.Alternate  = GPIO_AF0_SPI1; //EEPROM
	GPIO_InitStructure.Pin        = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

}

#endif

void S2LPShutdownEnter(void)
{
	/* Puts high the GPIO connected to shutdown pin */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
}

void S2LPShutdownInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//S2LP_M2S_SDN_CLOCK();
	__GPIOB_CLK_ENABLE();

	/* Configures MCU GPIO */
	GPIO_InitStructure.Pin   = S2LP_M2S_SDN_PIN;
	GPIO_InitStructure.Mode  = S2LP_M2S_SDN_MODE;
	GPIO_InitStructure.Pull  = S2LP_M2S_SDN_PUPD;
	GPIO_InitStructure.Speed = S2LP_M2S_SDN_SPEED;

	HAL_GPIO_Init(S2LP_M2S_SDN_PORT, &GPIO_InitStructure);

	S2LPShutdownEnter();
}

void S2LPShutdownExit(void)
{
	/* Puts low the GPIO connected to shutdown pin */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

	/* Delay to allow the circuit POR, about 700 us */
	for(volatile uint32_t i=0;i<0x1E00;i++);
}
uint8_t S2LPShutdownCheck(void)
{
	/* Gets the GPIO level */
	return (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
}

void ST_MCU_API_Shutdown(sfx_u8 value)
{
	if(value)
	{
		S2LPShutdownEnter();
	}
	else
	{
		S2LPShutdownExit();
	}
}

void S2LPIRQInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure, EXTI_InitStructure;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configures MCU GPIO */
	GPIO_InitStructure.Pin   = GPIO_PIN_2;
	GPIO_InitStructure.Mode  = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configures MCU GPIO EXTI line */
	EXTI_InitStructure.Pin   = GPIO_PIN_2;
	EXTI_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
	EXTI_InitStructure.Pull  = GPIO_NOPULL;
	EXTI_InitStructure.Speed = GPIO_SPEED_HIGH;

	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	HAL_GPIO_Init(GPIOB, &EXTI_InitStructure);
}

void S2LPIRQEnable(uint8_t state, uint8_t edge_direction)
{
	uint8_t xNewState = state ? ENABLE:DISABLE;
	GPIO_InitTypeDef EXTI_InitStructure;

	/* Configures EXTI line */
	EXTI_InitStructure.Pin   = GPIO_PIN_2;
	EXTI_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
	EXTI_InitStructure.Pull  = GPIO_NOPULL;
	EXTI_InitStructure.Speed = GPIO_SPEED_HIGH;

	/* Clear any pending interrupt before enabling */
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	HAL_GPIO_Init(GPIOB, &EXTI_InitStructure);

	if(edge_direction)
		EXTI->RTSR |= (uint16_t)GPIO_PIN_2;
	else
		EXTI->RTSR &= ~(uint16_t)GPIO_PIN_2;

	/* Set the GPIO interrupt priority and enable/disable it */
	if (xNewState == ENABLE)
	{
		HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
		HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0x00, 0x00);
	}
	else
		HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);
}

void ST_MCU_API_GpioIRQ(sfx_u8 pin, sfx_u8 new_state, sfx_u8 trigger)
{
	/* configure the MCU IRQ connected to the specified S2-LP GPIO */
	/* trigger 1: rising, 0: falling (default) */
	S2LPIRQInit();
	S2LPIRQEnable(new_state, trigger);
}

void ST_MCU_API_LowPower(sfx_u8 low_power_flag)
{
	low_power = low_power_flag;
}

void setLowPowerFlag(uint8_t lowPower) {
	low_power = lowPower;
}

uint8_t getLowPowerFlag(void) {
	return low_power;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);

	if(GPIO_Pin == GPIO_PIN_2)
	{
		if (ST_RF_API_Get_Continuous_TX_or_MONARCH_Scan_Flag()==0) {
			//s2lp_irq_raised = 1;
			setS2lpIrqRaisedFlag(1);
		} else {
			ST_RF_API_S2LP_IRQ_CB(); //If the CBPSK is implemented trigger TX State Machine
		}
	} 

}

uint8_t readButton(void) {
	uint8_t state = 1;

	if(!(GPIOB->IDR & 1<<0))
		state = 0;

	return state;
}

void onButtonRelease(void) {
	while(readButton())
		HAL_Delay(1);
}

uint8_t button_pressed(void) {
	uint8_t state = 1;

	while(state) {
		state = readButton();
		onButtonRelease();
	}
	return 1;
}

void setS2lpIrqRaisedFlag(uint8_t s2lpIrqRaised) {
	s2lp_irq_raised = s2lpIrqRaised;
}

uint8_t getS2lpIrqRaisedFlag(void) {
	return s2lp_irq_raised;
}

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
