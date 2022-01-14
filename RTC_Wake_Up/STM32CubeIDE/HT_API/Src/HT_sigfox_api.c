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

void HT_SigfoxApi_configRegion(rc_mask RCZ) {
	sfx_error_t error = SFX_ERR_NONE;

	switch(RCZ){
	case RCZ1:
	{
		error = SIGFOX_API_open(&(sfx_rc_t)RC1);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	}
	case RCZ2:
	{
		sfx_u32 config_words[3]={1,0,0};

		error = SIGFOX_API_open(&(sfx_rc_t)RC2);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ2_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(1);
		HT_SigfoxApi_setSmpsVoltageAction(2);


		break;
	}
	case RCZ3:
	{
		sfx_u32 config_words[3] = RC3C_CONFIG;

		error = SIGFOX_API_open(&(sfx_rc_t)RC3C);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ3_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	}
	case RCZ4:
	{
		sfx_u32 config_words[3]={0,0x40000000,0};

		error = SIGFOX_API_open(&(sfx_rc_t)RC4);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}


		ST_RF_API_reduce_output_power(RCZ4_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(1);
		HT_SigfoxApi_setSmpsVoltageAction(2);

		break;
	}
	case RCZ5:
	{
		sfx_u32 config_words[3]=RC5_CONFIG;

		error = SIGFOX_API_open(&(sfx_rc_t)RC5);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ5_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	}
	case RCZ6:
	{
		error = SIGFOX_API_open(&(sfx_rc_t)RC6);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	}
	case RCZ7:
	{
		error = SIGFOX_API_open(&(sfx_rc_t)RC7);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ7_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	}
	default:
		break;
	}
}

void HT_SigfoxApi_switchPa(uint8_t state) {
	ST_RF_API_set_pa(state);
	printf("Switch PA: %d\n", state);
}

void HT_SigfoxApi_setSmpsVoltageAction(sfx_u8 mode) {
	ST_RF_API_smps(mode);
	printf("Set_smps_voltage %d\n", mode);
}

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
