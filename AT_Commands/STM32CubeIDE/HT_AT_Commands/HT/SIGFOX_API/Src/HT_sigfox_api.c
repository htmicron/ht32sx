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
#include "tim.h"
#include "HT_sigfox_api.h"

extern uint8_t asc2_data_flag;
NVM_BoardDataType sfx_credentials;

static const uint32_t freq_lookup[54] = {
		137500, 162500, 187500, 212500, 237500, 262500, 437500, 462500, 487500,
		512500, 537500, 562500, 737500, 762500, 787500, 812500, 837500, 862500,
		1037500, 1062500, 1087500, 1112500, 1137500, 1162500, 1337500, 1362500, 1387500,
		1412500, 1437500, 1462500, 1637500, 1662500, 1687500, 1712500, 1737500, 1762500,
		1937500, 1962500, 1987500, 2012500, 2037500, 2062500, 2237500, 2262500, 2287500,
		2312500, 2337500, 2362500, 2537500, 2562500, 2587500, 2612500, 2637500, 2662500
};

static const uint32_t freqMatrix[9][6] = {
		{137500, 162500, 187500, 212500, 237500, 262500},
		{437500, 462500, 487500, 512500, 537500, 562500},
		{737500, 762500, 787500, 812500, 837500, 862500},
		{1037500, 1062500, 1087500, 1112500, 1137500, 1162500},
		{1337500, 1362500, 1387500, 1412500, 1437500, 1462500},
		{1637500, 1662500, 1687500, 1712500, 1737500, 1762500},
		{1937500, 1962500, 1987500, 2012500, 2037500, 2062500},
		{2237500, 2262500, 2287500, 2312500, 2337500, 2362500},
		{2537500, 2562500, 2587500, 2612500, 2637500, 2662500}
};

volatile uint8_t test_stop = 0;
static volatile uint8_t reduce_output_test = 0;

uint8_t HT_SigfoxApi_closeSigfoxLib(void) {
	uint8_t err;

	err = SIGFOX_API_close();

	return err;
}

uint8_t HT_SigfoxApi_sendBit(sfx_bool bit_value, sfx_bool initiate_downlink_flag) {
	uint8_t err = 0;
	uint8_t customer_response[8];
	uint8_t tmp[19];

	if(initiate_downlink_flag)
		HAL_TIM_Base_Start_IT(&htim21);

	err = SIGFOX_API_send_bit(bit_value, customer_response, 2, initiate_downlink_flag);

	if(initiate_downlink_flag && !err) {
		memset(tmp, 0, sizeof(tmp));

		tmp[0] = '{';
		sprintf((char *)&tmp[1], "%02X%02X%02X%02X%02X%02X%02X%02X\n", customer_response[0], customer_response[1], customer_response[2], customer_response[3],
				customer_response[4], customer_response[5], customer_response[6], customer_response[7]);
		tmp[17] = '}';

		HAL_UART_Transmit(&huart1, tmp, strlen((char *)tmp), 0xFFFF);
	}

	if(initiate_downlink_flag)
		HAL_TIM_Base_Stop_IT(&htim21);

	return err;
}

uint8_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_bool initiate_downlink_flag, uint8_t len) {
	uint8_t err = 0;
	uint8_t tmp[19];
	uint8_t customer_response[8];

	if(initiate_downlink_flag)
		HAL_TIM_Base_Start_IT(&htim21);

	err = SIGFOX_API_send_frame(customer_data, len,customer_response, 2, initiate_downlink_flag);

	if(initiate_downlink_flag && !err) {
		memset(tmp, 0, sizeof(tmp));

		tmp[0] = '{';
		sprintf((char *)&tmp[1], "%02X%02X%02X%02X%02X%02X%02X%02X\n", customer_response[0], customer_response[1], customer_response[2], customer_response[3],
				customer_response[4], customer_response[5], customer_response[6], customer_response[7]);
		tmp[17] = '}';

		HAL_UART_Transmit(&huart1, tmp, strlen((char *)tmp), 0xFFFF);
	}

	if(initiate_downlink_flag)
		HAL_TIM_Base_Stop_IT(&htim21);

	return err;
}

uint8_t HT_SigfoxApi_GetCredentials(void) {
	uint8_t err = 0;
	char str[37];

	memset((uint8_t *)str, '\0', sizeof(str));

	sprintf(str, "{%08X:%02X%02X%02X%02X%02X%02X%02X%02X}", (unsigned int)sfx_credentials.id, sfx_credentials.pac[0], sfx_credentials.pac[1],
			sfx_credentials.pac[2], sfx_credentials.pac[3], sfx_credentials.pac[4], sfx_credentials.pac[5],
			sfx_credentials.pac[6], sfx_credentials.pac[7]);

	HAL_UART_Transmit(&huart1, (uint8_t *)str, strlen(str), 0xFFFF);

	return err;

}

uint8_t HT_SigfoxApi_CtMacroChannel(int8_t i) {
	sfx_error_t err = 0;
	uint8_t startSeed = 1;
	uint8_t a = 1;
	uint8_t customer_data[3] = {0xAA,0xAA,0xAA};
	int32_t newbit = 0;

	test_stop = 0;

	/* PRBS-7 with repetition 127 */
	while(!test_stop && !err) {
		while(!test_stop && !err) {
			newbit = (((a >> 6) ^ (a >> 5)) & 1);
			a = ((a << 1) | newbit) & 0x7f;
			if(a < 7){
				err = SIGFOX_API_send_test_frame(((freqMatrix[i-1][a-1]) + 902000000), customer_data, sizeof(customer_data), 0);
			}
			if (a == startSeed) {
				break;
			}
		}
	}

	return err;

}

uint8_t HT_SigfoxApi_CtMicroChannel(int8_t i) {
	sfx_error_t err = 0;
	uint8_t customer_data[3] = {0xAA,0xAA,0xAA};

	test_stop = 0;

	while(!err && !test_stop){
		err = SIGFOX_API_send_test_frame((freq_lookup[i-1] + 902000000), customer_data, sizeof(customer_data), 0);
	}

	return err;
}

uint8_t HT_SigfoxApi_StartFreqHopping(void) {
	sfx_error_t err = 0;
	uint8_t customer_data[3] = {0xAA,0xAA,0xAA};
	uint8_t a;
	static uint8_t seed [52] = {35, 44, 25, 38, 41, 31, 40, 8, 51, 4, 48, 18, 43, 27, 11, 37, 22, 49, 7, 42,
			39, 14, 12, 5, 15, 32, 16, 1, 13, 6, 17, 19, 23, 54, 53, 29, 30, 46, 49, 34,
			3, 34, 26, 50, 21, 45, 14, 10, 28, 9, 33, 36};
	static uint8_t j = 0;
	int32_t newbit = 0;

	test_stop = 0;

	/* PRBS-7 with repetition 127 */
	a = seed[j];

	while(!err && !test_stop){
		while(!err && !test_stop) {
			newbit = (((a >> 6) ^ (a >> 5)) & 1);
			a = ((a << 1) | newbit) & 0x7f;

			if(a < 55){
				err = SIGFOX_API_send_test_frame((freq_lookup[a-1] + 902000000), customer_data, sizeof(customer_data), 0);
			}

			if (a == seed[j]) {
				j++;
				if (j == 52)
					j = 0;

				break;
			}
		}
	}

	return err;
}

uint8_t HT_SigfoxApi_ContinuousWave(uint32_t frequency) {
	uint8_t err = 0;

	err = SIGFOX_API_start_continuous_transmission(frequency, SFX_NO_MODULATION);

	return err;
}

uint8_t HT_SigfoxApi_StopCW(void) {
	uint8_t err;

	err = SIGFOX_API_stop_continuous_transmission();
	return err;
}

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
