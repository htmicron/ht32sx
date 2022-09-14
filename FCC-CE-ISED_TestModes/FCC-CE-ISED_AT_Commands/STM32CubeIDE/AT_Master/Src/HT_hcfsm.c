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

#include "HT_sigfox_api.h"
#include "HT_monarch_api.h"
#include "HT_hcfsm.h"
#include "HT_ATcmd.h"
#include "HT_mcu_api.h"
#include "usart.h"

extern uint8_t auxBuffer[DMA_RX_BUFFER_SIZE];
uint8_t cmd[DMA_RX_BUFFER_SIZE];
uint8_t pLen = 0;

uint8_t dmaEnable = 1;

state_func current_state = AT_Cmd;
AT_cmdStrError this_errStatus;

char cmdStr[UART_BUFFER_SIZE];
char firstParam[10];
char secParam[24];

AT_cmdStruct status;

AT_Cmd_stateFlags atCmdFlags;

#ifdef USE_ASC2_DATA
uint8_t asc2_data_flag = 1;
#else
uint8_t asc2_data_flag = 0;
#endif

void * AT_Cmd(void) {
	printf("AT_Cmd...\n");

	if(atCmdFlags.buttonFlag) return AT_SigFox;
	else if(atCmdFlags.cmdReceived && atCmdFlags.cmdType == AT_sigfox) return AT_SigFox;
	else if(atCmdFlags.cmdReceived && atCmdFlags.cmdType == AT_mcu) return AT_Mcu;

	return AT_Cmd_waitingCmd;
}

void * AT_Cmd_waitingCmd(void) {

	if(dmaEnable) {
		dmaEnable = 0;
		printf("AT_cmd_Waiting...\n");
		HAL_Delay(50);
		HW_Config_Circular_DMA();
	}

	if(atCmdFlags.cmdReceived) {
		CLEAR_BIT(huart1.Instance->CR1, USART_CR1_IDLEIE);
		SET_BIT(huart1.Instance->ICR, USART_ICR_IDLECF);
		return AT_Cmd_getCmd;
	} else if(atCmdFlags.buttonFlag) {
		CLEAR_BIT(huart1.Instance->CR1, USART_CR1_IDLEIE);
		SET_BIT(huart1.Instance->ICR, USART_ICR_IDLECF);
		return AT_SigFox_executeCmd;
	}

	return AT_Cmd_waitingCmd;
}

void * AT_Cmd_getCmd(void) {

	printf("AT_Cmd_getCmd...\n");

	AT_getCmdData(cmd);

	AT_DMA_clearUartRxBuffer();

	status = AT_checkCmdHdr((char *)cmd);

	if(status.AT_err != ERR_NONE)
		return AT_errorRoutine;

	AT_splitString((char *)cmd, "+", cmdStr);

	status = AT_checkCmdString(cmdStr);

	if(status.AT_err != ERR_NONE)
		return AT_errorRoutine;

	atCmdFlags.cmdType = status.AT_type;

	return AT_Cmd;
}

void * AT_errorRoutine(void) {
	AT_DMA_clearUartRxBuffer();

	switch(status.AT_err) {
	case DUMMY:
		break;
	case ERR_NONE:
		break;
	case ERR_PARAM_CMD:
		printf("AT_Cmd error: 0x%X\n", AT_ERR_PARAM_CMD);
		break;
	case ERR_UNAVAILABLE_CMD:
		printf("AT_Cmd error: 0x%X\n", AT_ERR_UNAVAILABLE_CMD);
		break;
	case ERR_HDR:
		printf("AT_Cmd error: 0x%X\n", AT_ERR_HDR);
		break;
	case ERR_OVF:
		printf("AT_Cmd error: 0x%X\n", AT_ERR_OVF);
		break;
	}

	return AT_cmd_returnStatus;
}

void * AT_Mcu(void) {

	printf("AT_Mcu...\n");

	return AT_Mcu_executeCmd;
}

void * AT_Mcu_executeCmd(void) {
	int32_t argp;
	char str_tmp[10];

	printf("AT_Mcu_executeCmd...\n");

	switch(status.AT_mcuCmd) {
	case AT_deepSleep:
		status.AT_mcuCmd = -1;
		HT_McuApi_enterDeepSleepMode();
		break;
	case AT_stop:
		status.AT_mcuCmd = -1;
		HT_McuApi_enterStopMode();
		break;
	case AT_reset:
		HT_McuApi_softwareReset();
		break;
	case AT_freq_offset:

		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp);

		argp = atoi(str_tmp);

		HT_McuApi_setFreqOffset(argp);

		break;
	case AT_rssi_offset:
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp);

		argp = atoi(str_tmp);

		HT_McuApi_setRssiOffset(argp);

		break;
	case AT_lbt_offset:

		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp);

		argp = atoi(str_tmp);

		HT_McuApi_setLbtOffset(argp);
		break;
	case AT_wkp:
		break;

	default:
		return AT_errorRoutine;
	}

	return AT_cmd_returnStatus;
}

void * AT_SigFox(void) {
	printf("AT_SigFox...\n");

	return AT_SigFox_executeCmd;
}

void * AT_SigFox_executeCmd(void) {
	printf("AT_SigFox_executeCmd\n");
	sfx_u8 customer_response[8];
	sfx_u8 customer_data[12];
	uint8_t integerParam1;
	uint16_t integerParam2;
	uint32_t argv[2];

	switch(status.AT_sigfoxCmd) {
	case AT_sendFrame:

		AT_splitString(cmdStr, "=", firstParam);

		integerParam1 = firstParam[0] - '0';

		AT_splitString(firstParam, ":", secParam);

		if(secParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		secParam[strlen(secParam)-1] = '\0';

		if(!asc2_data_flag) {
			if(strlen((char *)secParam) > MAX_HEX_SIZE) {
				status.AT_err = ERR_OVF;
				return AT_errorRoutine;
			}

			AT_getHexValue(secParam, (uint8_t *)customer_data);
			HT_SigfoxApi_sendFrame(customer_data, customer_response, integerParam1, strlen((char *)secParam));
		} else {
			if(strlen((char *)secParam) > MAX_PAYLOAD_SIZE) {
				status.AT_err = ERR_OVF;
				return AT_errorRoutine;
			}

			HT_SigfoxApi_sendFrame((sfx_u8 *)secParam, customer_response, integerParam1, strlen((char *)secParam));
		}

		break;
	case AT_monarchScan:

		AT_splitString(cmdStr, "=", firstParam);
		AT_splitString(firstParam, ":", secParam);

		if(firstParam[0] == '\0' || secParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		integerParam1 = atoi(firstParam);
		integerParam2 = atoi(secParam);

		integerParam1 = (integerParam1 == ALL_REGIONS) ? integerParam1 : HT_MonarchApi_getRcBitMask(integerParam1);

		HT_MonarchApi_monarchScan((sfx_u8)integerParam1, (sfx_u16)integerParam2);

		break;
	case AT_cfgrcz:

		AT_splitString(cmdStr, "=", firstParam);
		AT_splitString(firstParam, ":", secParam);

		if(firstParam[0] == '\0' || secParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		argv[0] = atoi(firstParam);
		argv[1] = atoi(secParam);

		HT_SigfoxApi_SetTestMode(AT_TX);
		HT_SigfoxApi_configRegion(argv[0], argv[1]);

		break;

	case AT_stpMonarch:

		HT_MonarchApi_stopMonarchScan();

		break;

	case AT_close:

		HT_SigfoxApi_closeSigfoxLib();

		break;


	case AT_continuous_wave:

		AT_splitString(cmdStr, "=", firstParam);
		AT_splitString(firstParam, ":", secParam);

		if(firstParam[0] == '\0' || secParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		argv[0] = atoi(firstParam);
		argv[1] = atoi(secParam);

		HT_SigfoxApi_CW((sfx_modulation_type_t)argv[0], argv[1]);

		break;

	case AT_stop_continuous_wave:

		HT_SigfoxApi_StopCW();
		break;

	case AT_tx_test:

		HT_SigfoxApi_SendTestFrame(AT_MultipleTx, AT_Command);

		atCmdFlags.buttonFlag = 0;

		break;

	case AT_cfgrx:

		AT_splitString(cmdStr, "=", firstParam);
		AT_splitString(firstParam, ":", secParam);

		if(firstParam[0] == '\0' || secParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		argv[0] = atoi(firstParam);
		argv[1] = atoi(secParam);

		HT_SigfoxApi_SetTestMode(AT_RX);
		HT_SigfoxApi_CfgRX((rc_mask)argv[0], argv[1]);

		break;

	case AT_rx_test:

		HT_SigfoxApi_receiveGFSK(0, 0, AT_MultipleRx, AT_Command);

		atCmdFlags.buttonFlag = 0;

		break;

	case AT_single_tx:

		HT_SigfoxApi_SendTestFrame(AT_SingleTx, AT_Command);
		break;

	case AT_rx_per:

		AT_splitString(cmdStr, "=", firstParam);

		if(firstParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		argv[0] = atoi(firstParam);

		HT_Sigfox_RxPER((sfx_u32)argv[0]);

		break;
	case AT_setTest_tx:
		AT_splitString(cmdStr, "=", firstParam);

		if(firstParam[0] == '\0'){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		argv[0] = atoi(firstParam);

		HT_SigfoxApi_SetTest(argv[0]);

		break;

	case AT_startFrequencyHop:

		HT_SigfoxApi_StartFrequencyHopping();

		break;

	default:
		return AT_errorRoutine;
	}

	return AT_cmd_returnStatus;
}

void * AT_cmd_returnStatus(void) {
	printf("AT_cmd_returnStatus\n");

	atCmdFlags.cmdReceived = 0;
	atCmdFlags.cmdReceivingCmd = 0;
	atCmdFlags.cmdType = AT_cmd;
	atCmdFlags.returnStatus = 1;
	status.AT_mcuCmd = 3;

	memset(cmd, 0, sizeof(cmd));
	memset(cmdStr, 0, sizeof(cmdStr));
	memset(firstParam, 0, sizeof(firstParam));
	memset(secParam, 0, sizeof(secParam));

	AT_cleanAuxBuffer();

	status.AT_err = DUMMY;

	dmaEnable = 1;

	return AT_Cmd;
}

void AT_getHexValue(char *str, uint8_t *output) {
	char tmp[2];
	uint8_t ind = 0;
	uint8_t i = 0;

	do {
		tmp[0] = str[ind];
		tmp[1] = str[++ind];

		ind++;

		output[i] = (uint8_t)strtol(tmp, NULL, 16);

		i++;
	} while (i < 12);

}

void AT_splitString(char *cmd, char *splitChar, char *ptr) {
	char aux[DMA_RX_BUFFER_SIZE];

	strcpy(aux, cmd);

	char *token = strtok((char *)aux, splitChar);

	token = strtok(NULL, splitChar);

	strcpy(ptr, token);

	token = NULL;
}

void AT_setCmdFlag(AT_flagu8 flag) {
	atCmdFlags.cmdReceived = flag;
}

void AT_updateFsm(AT_cmdStrError errStatus, AT_cmdType cmdType) {
	this_errStatus = errStatus;
	current_state = (state_func)(current_state)();
}

void AT_setFsmStatus(AT_cmdStruct new_status, AT_flagu8 buttonFlag) {
	atCmdFlags.buttonFlag = buttonFlag;
	status = new_status;
}

void AT_setFsmState(state_func state) {
	current_state = state;
}

void HT_hcfsm(void) {
	atCmdFlags.returnStatus = AT_ok;
	atCmdFlags.cmdType = AT_cmd;

	while (1)
		current_state = (state_func)(current_state)();
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
