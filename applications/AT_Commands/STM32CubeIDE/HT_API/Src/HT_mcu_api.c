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
#include "spi.h"
#include "main.h"
#include "gpio.h"
#include "dma.h"
#include "tim.h"
#include "rtc.h"
#include "HT_mcu_api.h"

uint8_t deepSleepModeFlag = 0;

void HT_McuApi_configPeripherals(void) {

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	HAL_UART_MspInit(getUsartHandle());
	__HAL_UART_DISABLE(getUsartHandle());
	__HAL_UART_ENABLE(getUsartHandle());

	/* Wake Up based on RXNE flag successful */
	HAL_UARTEx_DisableStopMode(getUsartHandle());
}

void HT_McuApi_enableUsartWkp(void) {
	UART_WakeUpTypeDef WakeUpSelection;

	/* make sure that no UART transfer is on-going */
	while(__HAL_UART_GET_FLAG(&huart1, USART_ISR_BUSY) == SET);
	/* make sure that UART is ready to receive
	 * (test carried out again later in HAL_UARTEx_StopModeWakeUpSourceConfig) */
	while(__HAL_UART_GET_FLAG(&huart1, USART_ISR_REACK) == RESET);

	/* set the wake-up event:
	 * specify wake-up on RXNE flag */
	WakeUpSelection.WakeUpEvent = UART_WAKEUP_ON_READDATA_NONEMPTY;
	if (HAL_UARTEx_StopModeWakeUpSourceConfig(&huart1, WakeUpSelection)!= HAL_OK)
	{
		Error_Handler();
	}

	/* Enable the UART Wake UP from stop mode Interrupt */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_WUF);

	/* enable MCU wake-up by UART */
	HAL_UARTEx_EnableStopMode(&huart1);

}

void HT_McuApi_enterGpioLowPower(void) {
	/* Set all the GPIOs in low power mode (input analog) */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;

	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_15) & (~GPIO_PIN_10) & (~GPIO_PIN_9) & (~GPIO_PIN_7);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_8) & (~GPIO_PIN_3) & (~GPIO_PIN_4);
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	//HAL_SPI_MspDeInit(getSpiHandle());

	/* keep the SDN driven */
	S2LPShutdownInit();

	/* Be sure that it is driving the device to be in shutdown*/
	S2LPShutdownEnter();
}

void HT_McuApi_enterStopMode(void) {

	deepSleepModeFlag = 1;

	EXTI->PR = 0xFFFFFFFF;

	HT_McuApi_enableUsartWkp();

	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	HAL_ResumeTick();

}

void HT_McuApi_enterDeepSleepMode(void) {

	deepSleepModeFlag = 1;

	FEM_Operation(FEM_TX);
	HAL_Delay(500);
	FEM_Operation(FEM_SHUTDOWN);
	S2LPShutdownEnter();

	HAL_Delay(500);
	EXTI->PR = 0xFFFFFFFF;

	HT_McuApi_enableUsartWkp();

	HT_McuApi_enterGpioLowPower();

	HT_McuApi_enterStopMode();
}

void HT_McuApi_softwareReset(void) {
	NVIC_SystemReset();
}

void HT_McuApi_setDeepSleepModeFlag(uint8_t flag) {
	deepSleepModeFlag = flag;
}

uint8_t HT_McuApi_getDeepSleepModeFlag(void) {
	return deepSleepModeFlag;
}

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
