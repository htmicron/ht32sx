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

extern uint8_t asc2_data_flag;
NVM_BoardDataType sfx_credentials;

uint8_t HT_SigfoxApi_configRegion(rc_mask RCZ) {
	uint8_t open_err = 0;

	switch(RCZ){
	case RCZ1:
		ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
		open_err = St_Sigfox_Open_RCZ(RCZ1);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	case RCZ2:
		ST_RF_API_reduce_output_power(RCZ2_OUTPUT_POWER);
		open_err = St_Sigfox_Open_RCZ(RCZ2);
		HT_SigfoxApi_switchPa(1);
		HT_SigfoxApi_setSmpsVoltageAction(2);

		break;
	case RCZ3:
		open_err = St_Sigfox_Open_RCZ(RCZ3);
		ST_RF_API_reduce_output_power(RCZ3_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	case RCZ4:
		open_err = St_Sigfox_Open_RCZ(RCZ4);
		ST_RF_API_reduce_output_power(RCZ4_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(1);
		HT_SigfoxApi_setSmpsVoltageAction(2);

		break;
	case RCZ5:
		open_err= St_Sigfox_Open_RCZ(RCZ5);
		ST_RF_API_reduce_output_power(RCZ5_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	case RCZ6:
		open_err = St_Sigfox_Open_RCZ(RCZ6);
		ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(0);
		HT_SigfoxApi_setSmpsVoltageAction(7);

		break;
	case RCZ7:
		open_err = St_Sigfox_Open_RCZ(RCZ7);
		ST_RF_API_reduce_output_power(RCZ7_OUTPUT_POWER);
		HT_SigfoxApi_switchPa(1);

		break;
	default:
		break;
	}

	return open_err;
}

void HT_SigfoxApi_switchPa(uint8_t state) {
	ST_RF_API_set_pa(state);
}

void HT_SigfoxApi_setSmpsVoltageAction(sfx_u8 mode) {
	ST_RF_API_smps(mode);
}

uint8_t HT_SigfoxApi_closeSigfoxLib(void) {
	uint8_t err;

	err = SIGFOX_API_close();

	return err;
}

uint8_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_u8 *customer_response, sfx_bool initiate_downlink_flag, uint8_t len) {
	uint8_t err = 0;
	uint8_t tmp[19];

	if(initiate_downlink_flag)
		HAL_TIM_Base_Start_IT(&htim21);

	err = SIGFOX_API_send_frame(customer_data, len,customer_response, 3, initiate_downlink_flag);

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
