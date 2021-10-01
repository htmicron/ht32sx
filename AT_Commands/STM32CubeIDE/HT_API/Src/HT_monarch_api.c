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

#include "HT_monarch_api.h"
#include "usart.h"

uint8_t HT_MonarchApi_stopMonarchScan(void) {
	uint8_t err;

	err = SIGFOX_MONARCH_API_stop_rc_scan();
	return err;
}

uint8_t HT_MonarchApi_monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer){
	uint8_t err;

	err = SIGFOX_MONARCH_API_execute_rc_scan(rc_capabilities_bit_mask, timer, SFX_TIME_M, HT_MonarchApi_callback);
	return err;
}

uint8_t HT_MonarchApi_getRcBitMask(uint8_t rcz) {
	uint8_t bitMask = 0;

	bitMask = 1 << (rcz-1);

	return bitMask;
}

sfx_u8 HT_MonarchApi_callback(sfx_u8 rc_bit_mask, sfx_s16 rssi) {
	char ret[10];
	uint8_t rc = 0;

	sprintf(ret, "{%d:", rssi);

	switch (rc_bit_mask) {
		case 0x01:
			rc = 1;
			break;
		case 0x02: //RC2
			rc = 2;
			break;
		case 0x04:  //RC3a
			rc = 3;
			break;
		case 0x08:  //RC4
			rc = 4;
			break;
		case 0x10: //RC5
			rc = 5;
			break;
		case 0x20:  //RC6
			rc = 6;
			break;
		case 0x40:  //RC7
			rc = 7;
			break;
	}

	sprintf(&ret[strlen(ret)], "%d}\n", rc);
	HAL_UART_Transmit(&huart1, (uint8_t *)ret, strlen(ret), 0xFFFF);

	return 0;
}

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
