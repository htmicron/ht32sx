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

#include "HT_mcu_api.h"
#include "HT_P2P_s2lp_config.h"
#include "HT_data_repeater.h"
#include "S2LP_AUX_FEM.h"

uint8_t deepSleepModeFlag = 0;

extern HT_DR_States state;

void HT_McuApi_DeepSleepMode(void) {
	FEM_Operation(FEM_SHUTDOWN);
	S2LPEnterShutdown();

	EXTI->PR = 0xFFFFFFFF;

	HAL_PWR_EnterSTANDBYMode();
}

void HT_McuApi_EnableRTCWkp(uint32_t seconds) {

	/* Disable all Wakeup IT */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	HAL_NVIC_SetPriority(RTC_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(RTC_IRQn);

	/* Set wakeup IT */
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
