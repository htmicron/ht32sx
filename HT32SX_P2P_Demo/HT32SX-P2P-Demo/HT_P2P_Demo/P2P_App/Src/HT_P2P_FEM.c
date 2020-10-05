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

#include "HT_P2P_FEM.h"

void FEM_Init(void) {

	/* Configuration of S2LP GPIO to control external PA signal CSD, CPS, CTX */
	uint8_t tmp[]={
			(uint8_t)S2LP_GPIO_DIG_OUT_TX_RX_MODE_VALUE | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP_VALUE,
			(uint8_t)S2LP_GPIO_DIG_OUT_RX_STATE_VALUE   | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP_VALUE,
			(uint8_t)S2LP_GPIO_DIG_OUT_TX_STATE_VALUE   | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP_VALUE
	};

	S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);
}

void Config_RangeExt(PA_OperationType operation) {

	uint8_t tmp[] = {0x00,0x00,0x00};

	FEM_Init();

	switch (operation) {
	case PA_SHUTDOWN:
		tmp[0]=LOW_WORD;
		tmp[1]=LOW_WORD;
		tmp[2]=LOW_WORD;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);
		break;

	case PA_TX_BYPASS:
		tmp[0]=HIGH_WORD;
		tmp[1]=LOW_WORD;
		tmp[2]=HIGH_WORD;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;

	case PA_TX:
		tmp[0]=HIGH_WORD;
		tmp[1]=HIGH_WORD;
		tmp[2]=HIGH_WORD;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;

	case PA_RX:
		tmp[0]=LOW_WORD;
		tmp[1]=LOW_WORD;
		tmp[2]=HIGH_WORD;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;

	default:
		/* Error */
		tmp[0]=LOW_WORD;
		tmp[1]=LOW_WORD;
		tmp[2]=LOW_WORD;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);
		break;
	}
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
