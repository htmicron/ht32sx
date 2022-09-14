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
#include "HT_hcfsm.h"
#include "HT_RF_API.h"

extern uint8_t asc2_data_flag;

sfx_u32 test_frequency = 0;
sfx_u32 uplink_center = 0;

AT_test_state test_mode = AT_RX;

extern uint8_t stop_test;
extern uint8_t test_running;
uint8_t rx_timeout = 0;

static const uint32_t freq_lookup[54] = {
		137500, 162500, 187500, 212500, 237500, 262500, 437500, 462500, 487500,
		512500, 537500, 562500, 737500, 762500, 787500, 812500, 837500, 862500,
		1037500, 1062500, 1087500, 1112500, 1137500, 1162500, 1337500, 1362500, 1387500,
		1412500, 1437500, 1462500, 1637500, 1662500, 1687500, 1712500, 1737500, 1762500,
		1937500, 1962500, 1987500, 2012500, 2037500, 2062500, 2237500, 2262500, 2287500,
		2312500, 2337500, 2362500, 2537500, 2562500, 2587500, 2612500, 2637500, 2662500
};

static sfx_u8 rx_gfsk_pattern[15] = {0x1F, 0x20, 0x41, 0x84, 0x32, 0x68, 0xC5, 0xBA, 0x53, 0xAE, 0x79, 0xE7, 0xF6, 0xDD, 0x9B};

void HT_RF_API_SetUplinkCenter(sfx_u32 uplinkCenter){
	uplink_center = uplinkCenter;
}

void HT_SigfoxApi_SetTestMode(AT_test_state test_type) {
	test_mode = test_type;
}

AT_test_state HT_SigfoxApi_GetTestMode(void) {
	return test_mode;
}

void HT_SigfoxApi_StartFrequencyHopping(void) {
	sfx_error_t err = 0;
	uint8_t customer_data[3] = {0xAA,0xAA,0xAA};
	AT_test_state tmp;
	uint8_t a;
	static uint8_t seed [52] = {35, 44, 25, 38, 41, 31, 40, 8, 51, 4, 48, 18, 43, 27, 11, 37, 22, 49, 7, 42,
			39, 14, 12, 5, 15, 32, 16, 1, 13, 6, 17, 19, 23, 54, 53, 29, 30, 46, 49, 34,
			3, 34, 26, 50, 21, 45, 14, 10, 28, 9, 33, 36};
	static uint8_t j = 0;
	uint32_t newbit;

	/* PRBS-7 with repetition 127 */

	a = seed[j];

	stop_test = 0;
	tmp = test_mode;
	test_mode = AT_FREQHOP;

	for(int32_t i = 1;  ; i++){
		newbit  = (((a >> 6) ^ (a >> 5)) & 1);

		a = ((a << 1) | newbit) & 0x7f;

		if(a < 55)
			err = SIGFOX_API_send_test_frame((freq_lookup[a-1] + 902000000), customer_data, sizeof(customer_data), 0);

//		if (a == seed[j]) {
//			j++;
//
//			if (j == 52)
//				j = 0;
//
//			//break;
//		}

		if(stop_test || err != SFX_ERR_NONE) {
			j++;

			if (j == 52)
				j = 0;

			break;
		}

	}

	test_mode = tmp;

	printf("Frequency hopping error: %X\n", err);
}

void HT_Sigfox_RxPER(sfx_u32 frequency) {
	int32_t i;
	uint32_t rx_counter = 0;
	int32_t err_counter = 0;
	sfx_error_t err = 0;

	printf("Running test...\n");

	HAL_Delay(500);

	stop_test = 0;
	test_running = 1;

	for(i = 1; ; i++) {
		err = HT_SigfoxApi_receiveGFSK(frequency, RX_PER_TIMEOUT, AT_Per, AT_Command);
		rx_counter = HT_API_getDownlinkCounter();

		printf("Test ID: %ld\n", i);
		printf("Received: %ld\n", rx_counter);

		if(rx_counter == MAX_N_FRAMES || stop_test || err != SFX_ERR_NONE)
			break;
	}

	printf("\nTest finished.\n");
	printf("Downlink received: %ld\n", rx_counter);

	err_counter = (i + HT_API_getDownlinkFailed()) - rx_counter;
	printf("Number of errors occurred: %ld\n", err_counter);

	test_running = 0;
	HT_API_setDownlinkFailed(0);
	HT_API_setDownlinkCounter(0);
	HT_API_setInd(0);
}

void HT_SigfoxApi_CfgRX(rc_mask RCZ, uint32_t frequency) {
	HT_SigfoxApi_configRegion(RCZ, frequency);
}

sfx_error_t HT_SigfoxApi_receiveGFSK(sfx_s32 frequency, sfx_u8 timeout, AT_RX_State state, AT_interface interface) {
	sfx_s16 rssi;
	sfx_u8 frame_buffer[15];
	sfx_error_t err = 0;

	for(int i = 0; i < DOWNLINK_MAX_FRAME_LENGTH; i++) {
		/* AA AA B2 27 1F 20 41 84 32 68 C5 BA 53 AE 79 E7 F6 DD 9B */
		frame_buffer[i] = rx_gfsk_pattern[i];
	}

	switch(state) {
	case AT_SingleRx:

		printf("\nRX Frequency: %ld - RX Timeout: %d\n", ((interface==AT_Button)?test_frequency:frequency),
				((interface==AT_Button)?rx_timeout:timeout));

		printf("RX\n");

		err = SIGFOX_API_receive_test_frame((interface==AT_Button)?test_frequency:frequency,
				AUTHENTICATION_OFF, frame_buffer, (interface==AT_Button)?rx_timeout:timeout, &rssi);

		printf("Receive test frame error code: %X\n", err);
		break;
	case AT_Per:

		err = SIGFOX_API_receive_test_frame(frequency, AUTHENTICATION_OFF, frame_buffer, timeout, &rssi);

		if(err != SFX_ERR_INT_GET_RECEIVED_FRAMES_TIMEOUT)
			printf("Receive test frame error code: %X\n", err);
		else
			err = 0;
		break;

	case AT_MultipleRx:

		printf("\nRX Frequency: %ld - RX Timeout: %d\n", test_frequency, RX_PER_TIMEOUT);

		printf("RX\n");

		test_running = 1;
		stop_test = 0;

		do {
			err = SIGFOX_API_receive_test_frame(test_frequency, AUTHENTICATION_OFF, frame_buffer, RX_PER_TIMEOUT, &rssi);
			err = (err == SFX_ERR_INT_GET_RECEIVED_FRAMES_TIMEOUT) ? SFX_ERR_NONE : err;
		} while(!stop_test && err == SFX_ERR_NONE);

		printf("Receive test frame error code: %X\n", err);

		test_running = 0;

		break;
	}

	return err;
}

void HT_SigfoxApi_StopCW(void) {
	sfx_error_t err;

	err = SIGFOX_API_stop_continuous_transmission();

	printf("Stop continuous wave error code: %X\n", err);
}

void HT_SigfoxApi_CW(sfx_modulation_type_t modulation, uint32_t frequency) {
	sfx_error_t err = 0;

	printf("Frequency: %ld -- Mod: %d\n", frequency, modulation);
	err = SIGFOX_API_start_continuous_transmission(frequency, modulation);

	printf("Continuous wave error code: %X\n", err);
}

void HT_SigfoxApi_SendTestFrame(AT_TX_State state, AT_interface interface) {
	sfx_error_t err = 0;
	sfx_u8 customer_data[12] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

	switch(state) {
	case AT_SingleTx:

		printf("Frequency: %ld\n", uplink_center);

		err = SIGFOX_API_send_test_frame(uplink_center, customer_data,
				sizeof((uint8_t *)customer_data), SFX_FALSE);
		break;

	case AT_MultipleTx:

		stop_test = 0;
		test_running = 1;

		printf("Frequency: %ld\n", test_frequency);

		do {
			err = SIGFOX_API_send_test_frame(test_frequency, customer_data, sizeof((uint8_t *)customer_data), SFX_FALSE);
		} while(!stop_test && !err);

		test_running = 0;

		break;
	}

	printf("Send test frame error code: %X\n", err);
}

void HT_SigfoxApi_closeSigfoxLib(void) {
	sfx_error_t err;

	err = SIGFOX_API_close();
	printf("Close api error: %X\n", err);
}

sfx_error_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_u8 *customer_response, sfx_bool initiate_downlink_flag, uint8_t len) {
	uint8_t buffSize;
	sfx_error_t err;

	printf("Sending frame...\n");

	if(asc2_data_flag)
		buffSize = len;
	else
		buffSize = !(len % 2) ? len/2 : (len/2)+1;

	if(initiate_downlink_flag)
		HAL_TIM_Base_Start_IT(&htim21);

	err = SIGFOX_API_send_frame(customer_data, buffSize,customer_response, 3, initiate_downlink_flag);

	if(initiate_downlink_flag && !err) {
		printf("Customer resp: {");

		for(uint16_t i = 0; i < 7; i++)
			printf("0x%x,", customer_response[i]);

		printf("0x%x}\n\r", customer_response[7]);
	}

	printf("\nError Send Frame: %X\n", err);

	if(initiate_downlink_flag)
		HAL_TIM_Base_Stop_IT(&htim21);

	return err;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
