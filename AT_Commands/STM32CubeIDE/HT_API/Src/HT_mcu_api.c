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
#include "adc.h"
#include "HT_mcu_api.h"
#include "SDK_UTILS_Flash.h"

uint8_t deepSleepModeFlag = 0;

void HT_McuApi_configPeripherals(void) {

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();
	HAL_SPI_MspInit(&hspi1);

	MX_USART1_UART_Init();
	__HAL_UART_DISABLE(&huart1);
	__HAL_UART_ENABLE(&huart1);
}

void HT_McuApi_enterGpioLowPower(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;

	GPIO_InitStructure.Pin = GPIO_PIN_All;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

#if STANDBY_MODE == 1
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_15);
#else
	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_15) & (~GPIO_PIN_6) & (~GPIO_PIN_9) & (~GPIO_PIN_10);
#endif

	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_All & (~GPIO_PIN_8);
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* keep the SDN driven */
	S2LPShutdownInit();

	/* Be sure that it is driving the device to be in shutdown*/
	S2LPShutdownEnter();
}

void HT_McuApi_enterStopMode(void) {

	deepSleepModeFlag = 1;

	HAL_SuspendTick();
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
	HAL_ResumeTick();
}

void HT_McuApi_DisableADC(void) {
	HAL_ADC_DeInit(&hadc);
	__HAL_RCC_ADC1_FORCE_RESET();
	__HAL_RCC_ADC1_RELEASE_RESET();
}

void HT_McuApi_enterDeepSleepMode(void) {

	FEM_Operation(FEM_SHUTDOWN);
	S2LPShutdownEnter();

#if STANDBY_MODE == 1
	/*Disable all used wakeup sources: Pin1(PA.0)*/
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

	/*Clear all related wakeup flags*/
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	/*Re-enable all used wakeup sources: Pin1(PA.0)*/
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	HAL_PWR_EnterSTANDBYMode();
#else

	EXTI->PR = 0xFFFFFFFF;
	HAL_Delay(50);

	HT_McuApi_DisableADC();

	HT_McuApi_enterGpioLowPower();
	HT_McuApi_enterStopMode();

#endif

}

uint8_t HT_McuApi_setLbtOffset(int32_t offset) {
	uint8_t err;

	err = ST_RF_API_set_lbt_thr_offset(offset);

	NVM_UpdateOffset(NVM_LBT_OFFSET, offset);

	return err;
}

uint8_t HT_McuApi_setRssiOffset(int32_t offset) {
	uint8_t err;

	err = ST_RF_API_set_rssi_offset(offset);

	NVM_UpdateOffset(NVM_RSSI_OFFSET, offset);

	return err;
}

uint8_t HT_McuApi_setFreqOffset(int32_t offset) {
	uint8_t err;

	err = ST_RF_API_set_freq_offset(offset);  /* Override RF_API Xtal value */

	if(!err)
		err = NVM_UpdateOffset(NVM_FREQ_OFFSET, offset);

	return err;
}

uint8_t HT_McuApi_reduceOutputPower(int16_t reduce_value) {
	return ST_RF_API_reduce_output_power(reduce_value);
}

uint8_t HT_McuApi_switchBoost(uint8_t state) {
	if(state)
		ST_RF_API_smps(7);
	else
		ST_RF_API_smps(2);

	return SFX_ERR_NONE;
}

uint8_t HT_McuApi_switchPa(uint8_t state) {
	return ST_RF_API_set_pa(state);
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

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
