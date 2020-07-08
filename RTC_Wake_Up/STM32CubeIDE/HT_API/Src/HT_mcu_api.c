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
	MX_SPI1_Init();
	HAL_SPI_MspInit(getSpiHandle());
	MX_USART1_UART_Init();
	HAL_UART_MspInit(getUsartHandle());
	__HAL_UART_DISABLE(getUsartHandle());
	__HAL_UART_ENABLE(getUsartHandle());
}

void HT_McuApi_enterGpioLowPower(void) {
	/* Set all the GPIOs in low power mode (input analog) */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;

	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_15);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_8) & (~GPIO_PIN_0) & (~GPIO_PIN_2);
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

	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	HAL_ResumeTick();

}

void HT_McuApi_EnableRTCWkp(uint32_t seconds) {
	uint32_t counter = 0;
	float base = 0;

	/* Disable Wakeup Counter */
	HAL_RTCEx_DeactivateWakeUpTimer(getRtcHandler());

	base = 0.410/1000;
	counter = seconds/base; //truncate

	HAL_RTCEx_SetWakeUpTimer_IT(getRtcHandler(), counter, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {

	if(deepSleepModeFlag) {
		/* Clear Wake Up Flag */
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		HT_McuApi_configPeripherals();

		printf("Waking up...\n");

		deepSleepModeFlag = 0;
	}
}

void HT_McuApi_enterDeepSleepMode(void) {

	deepSleepModeFlag = 1;

	FEM_Operation(FEM_TX);
	HAL_Delay(500);
	FEM_Operation(FEM_SHUTDOWN);
	S2LPShutdownEnter();

	HAL_Delay(500);
	EXTI->PR = 0xFFFFFFFF;

	HT_McuApi_enterGpioLowPower();

	HT_McuApi_enterStopMode();
}

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
