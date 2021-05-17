/**
 *
 * Copyright (c) 2021 HT Micron Semicondutors S.A.
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

#include "HT_nvm_api.h"

void HT_NVM_InitBkpAddress(void) {
	uint8_t credentials[64];
	uint8_t tmp = 0x00;

	FlashRead(SFX_BKP_ADDRESS, sizeof(tmp), &tmp);

	if(tmp == 0x00) {
		printf("Creating a bkp point...\n");
		FlashRead(CREDENTIALS_ADDRESS, sizeof(credentials), credentials);
		FlashWrite(SFX_BKP_ADDRESS, sizeof(credentials), credentials, 1);
	}
}

void HT_NVM_StoreContext(void) {
	uint8_t data[4];
	uint8_t buff[16] = {0xFE, 0x00, 0x00, 0x00, 0xFC};
	uint32_t tmp;

	FlashRead(CONTEXT_ADDRESS+8, sizeof(data), data);

	tmp = HT_RET_ConvertSeqNumber(data);
	HT_RET_GetSeqNumber(tmp, data);

	memcpy(&buff[8], data, sizeof(data));
	FlashWrite(CONTEXT_ADDRESS, sizeof(buff), buff, 1);
}

void HT_NVM_ConfigEEPROM(uint8_t *sfx_credentials, uint8_t *sfx_environment, uint8_t *sfx_context) {
	uint32_t address = CONTEXT_ADDRESS;
	uint8_t data[4];

	FlashWrite(CREDENTIALS_ADDRESS, SFX_CREDENTIALS_SIZE, sfx_credentials, 1);
	FlashWrite(SFX_ENVIRONMENT_ADDR, SFX_ENVIRONMENT_SIZE, sfx_environment, 1);

	memset(data, 0, sizeof(data));

	while(1) {
		FlashRead(address, sizeof(data), data);

		if(address > EEPROM_LAST_ADDRESS) {

			for(uint8_t i = 0; i < CONTEXT_SIZE; i+=4)
				DataEepromErase(address+i);

			FlashWrite(CONTEXT_ADDRESS+8, SFX_CONTEXT_SIZE, sfx_context, 1);

			break;
		} else if(data[0] == 0x00) {
			FlashWrite((address-16)+8, SFX_CONTEXT_SIZE, sfx_context, 1);
			break;
		}

		address += 16;
	}

}

void HT_NVM_GetSigfoxContext(uint8_t *sfx_credentials, uint8_t *sfx_environment, uint8_t *sfx_context) {
	uint8_t data[4];

	FlashRead(CREDENTIALS_ADDRESS, SFX_CREDENTIALS_SIZE, sfx_credentials);
	FlashRead(SFX_ENVIRONMENT_ADDR, SFX_ENVIRONMENT_SIZE, sfx_environment);

	FlashRead(CONTEXT_ADDRESS+8, sizeof(data), data);

	memcpy(sfx_context, data, sizeof(data));
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
