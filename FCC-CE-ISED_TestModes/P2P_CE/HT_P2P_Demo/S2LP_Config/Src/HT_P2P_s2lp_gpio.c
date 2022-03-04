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

#include "HT_P2P_s2lp_gpio.h"
#include "HT_P2P_app.h"

static const uint8_t aRADIO_GPIO_IRQn[RADIO_GPIO_NUMBER-1] = {
		RADIO_GPIO_0_EXTI_IRQN,
		RADIO_GPIO_1_EXTI_IRQN,
		RADIO_GPIO_2_EXTI_IRQN,
		RADIO_GPIO_3_EXTI_IRQN
};

void S2LPGpioInit(SGpioInit* pxGpioInitStruct) {
	uint8_t tmp;

	s_assert_param(IS_S2LP_GPIO(pxGpioInitStruct->xS2LPGpioPin));
	s_assert_param(IS_S2LP_GPIO_MODE(pxGpioInitStruct->xS2LPGpioMode));
	s_assert_param(IS_S2LP_GPIO_IO(pxGpioInitStruct->xS2LPGpioIO));

	tmp = ((uint8_t)(pxGpioInitStruct->xS2LPGpioMode) | (uint8_t)(pxGpioInitStruct->xS2LPGpioIO));
	g_xStatus = S2LPSpiWriteRegisters(pxGpioInitStruct->xS2LPGpioPin, 1, &tmp);

}

void S2LPGpioIrqConfig(IrqList xIrq, SFunctionalState xNewState) {
	uint8_t tmpBuffer[4];
	uint32_t tempValue = 0;

	s_assert_param(IS_S2LP_IRQ_LIST(xIrq));
	s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

	S2LPSpiReadRegisters(IRQ_MASK3_ADDR, 4, tmpBuffer);

	/* Build the IRQ mask word */
	for(int i=0; i<4; i++) {
		tempValue += ((uint32_t)tmpBuffer[i])<<(8*(3-i));
	}

	/* Rebuild the new mask according to user request */
	if(xNewState == S_DISABLE) {
		tempValue &= (~xIrq);
	}
	else {
		tempValue |= (xIrq);
	}

	/* Build the array of bytes to write in the IRQ_MASK registers */
	for(int j=0; j<4; j++) {
		tmpBuffer[j] = (uint8_t)(tempValue>>(8*(3-j)));
	}

	g_xStatus = S2LPSpiWriteRegisters(IRQ_MASK3_ADDR, 4, tmpBuffer);

}

void S2LPGpioIrqDeInit(S2LPIrqs* pxIrqInit) {
	uint8_t tmp[4] = {0x00,0x00,0x00,0x00};

	if(pxIrqInit != NULL)
		memset(pxIrqInit, 0, sizeof(S2LPIrqs));

	g_xStatus = S2LPSpiWriteRegisters(IRQ_MASK3_ADDR, 4, tmp);
}

void RadioGpioInterruptCmd(RadioGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState) {
	if (xNewState == ENABLE) {
		HAL_NVIC_EnableIRQ((IRQn_Type)aRADIO_GPIO_IRQn[xGpio]);
		HAL_NVIC_SetPriority((IRQn_Type)aRADIO_GPIO_IRQn[xGpio], nPreemption, nSubpriority);

	} else {
		HAL_NVIC_DisableIRQ((IRQn_Type)aRADIO_GPIO_IRQn[xGpio]);
	}
}

void S2LPGpioIrqClearStatus(void) {
	uint8_t tmp[4] = {0x00, 0x00, 0x00, 0x00};
	g_xStatus = S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);

}

void S2LPGpioIrqGetStatus(S2LPIrqs* pxIrqStatus) {
	uint8_t tmp[4];
	uint8_t* pIrqPointer = (uint8_t*)pxIrqStatus;

	g_xStatus = S2LPSpiReadRegisters(IRQ_STATUS3_ADDR, 4, tmp);

	/* Build the IRQ Status word */
	for(uint8_t i=0; i<4; i++) {
		*pIrqPointer = tmp[3-i];
		pIrqPointer++;
	}
}

void HT_P2P_softReset(void) {

	printf("Reseting...\n");
	HAL_Delay(500);

	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(SOFT_RESET_Pin);

	NVIC_SystemReset();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == GPIO_PIN_2) {
		P2PInterruptHandler();
	} else if(GPIO_Pin == GPIO_PIN_0) {
		Set_KeyStatus(SET);
	} else if(GPIO_Pin == SOFT_RESET_Pin) {
		HT_P2P_softReset();
	}
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
