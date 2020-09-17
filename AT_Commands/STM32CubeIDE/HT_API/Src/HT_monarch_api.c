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

void HT_MonarchApi_stopMonarchScan(void) {
	printf("Stop Monarch error: %X\n", SIGFOX_MONARCH_API_stop_rc_scan());
}

void HT_MonarchApi_monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer){
	sfx_error_t err;

	err = SIGFOX_MONARCH_API_execute_rc_scan(rc_capabilities_bit_mask, timer, SFX_TIME_M, HT_MonarchApi_callback);
	printf("\nScan Monarch error: %X \n", err);
}

uint8_t HT_MonarchApi_getRcBitMask(uint8_t rcz) {
	uint8_t bitMask = 0;

	bitMask = 1 << (rcz-1);

	return bitMask;
}

sfx_u8 HT_MonarchApi_callback(sfx_u8 rc_bit_mask, sfx_s16 rssi)
{
	printf("Return rssi %d\r\n", rssi);

	switch (rc_bit_mask)
	{
		case 0x01:

			printf("Detected RC1!!!\r\n");
			break;
		case 0x02: //RC2

			printf("Detected RC2!!!\r\n");
			break;

		case 0x04:  //RC3a

			printf("Detected RC3!!!\r\n");
			break;
		case 0x08:  //RC4

			printf("Detected RC4!!!\r\n");
			break;
		case 0x10: //RC5

			printf("Detected RC5!!!\r\n");
			break;
		case 0x20:  //RC6

			printf("Detected RC6!!!\r\n");
			break;
		case 0x40:  //RC7

			printf("Detected RC7!!!\r\n");
			break;

		default:
			break;
	}

	return 1;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
