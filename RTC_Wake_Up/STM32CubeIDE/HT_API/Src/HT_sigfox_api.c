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
#include "HT_sigfox_api.h"


void HT_SigfoxApi_closeSigfoxLib(void) {
	sfx_error_t err;

	err = SIGFOX_API_close();
	printf("Close api error: %X\n", err);
}

sfx_error_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_u8 *customer_response, sfx_bool initiate_downlink_flag) {

	sfx_error_t err;

	printf("Sending frame...\n");

	err = SIGFOX_API_send_frame(customer_data,strlen((char *)customer_data),customer_response, 3, initiate_downlink_flag);

	if(initiate_downlink_flag) {
		printf("Customer resp: {");

		for(uint16_t i = 0; i < 7; i++)
			printf("0x%x,", customer_response[i]);

		printf("0x%x}\n\r", customer_response[7]);
	}

	printf("\nError Send Frame: %X\n", err);

	return err;
}

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
