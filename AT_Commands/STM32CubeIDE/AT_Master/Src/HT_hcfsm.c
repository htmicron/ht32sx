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

uint8_t auxBuffer[DMA_RX_BUFFER_SIZE];
uint8_t cmd[DMA_RX_BUFFER_SIZE];
uint8_t pLen = 0;

uint8_t dmaEnable = 1;

state_func current_state = AT_Cmd;
AT_cmdStrError this_errStatus;

char cmdStr[UART_BUFFER_SIZE];
char firstParam[2];
char secParam[MAX_HEX_SIZE];

AT_cmdStruct status = {0};
static uint8_t status_bkp = 0;
static uint8_t integerParam1 = 0;

AT_Cmd_stateFlags atCmdFlags;

AT_ErrorCode error_code = {0};

#ifdef USE_ASC2_DATA
uint8_t asc2_data_flag = 1;
#else
uint8_t asc2_data_flag = 0;
#endif

static uint8_t AT_ValidRcMask(uint8_t rc_mask) {
	return ((rc_mask >= 1 && rc_mask <= 7) || (rc_mask == 127)) ? 1 : 0;
}

static uint8_t AT_IsHexValue(char value) {
	return (((uint8_t)value >= 0x30 && (uint8_t)value <= 0x39) || ((uint8_t)value >= 0x41 && (uint8_t)value <= 0x46)
			? 1 : 0);
}

static uint8_t AT_IsHexPayload(char *payload) {

	for (uint8_t i = 0; i < strlen(payload); i++) {
		if(!AT_IsHexValue(payload[i]))
			return 0;
	}

	return 1;
}

static uint8_t AT_ValidParameterFlag(uint8_t param) {
	return (param == 0 || param == 1 ? 1 : 0);
}

static uint8_t AT_NullString(char *str) {
	return (str[0] == '\0' || str[0] == ';' || strlen(str) == 0) ? 1 : 0;
}

void * AT_Cmd(void) {

	if(atCmdFlags.cmdReceived && atCmdFlags.cmdType == AT_sigfox) return AT_SigFox;
	else if(atCmdFlags.cmdReceived && atCmdFlags.cmdType == AT_mcu) return AT_Mcu;

	return AT_Cmd_waitingCmd;
}

void * AT_Cmd_waitingCmd(void) {

	if(dmaEnable) {
		dmaEnable = 0;
		HAL_Delay(50);
		HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);
		HW_Config_Circular_DMA();
	}

	if(atCmdFlags.cmdReceived) {
		CLEAR_BIT(huart1.Instance->CR1, USART_CR1_IDLEIE);
		SET_BIT(huart1.Instance->ICR, USART_ICR_IDLECF);
		HAL_NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);
		return AT_Cmd_getCmd;
	}

	return AT_Cmd_waitingCmd;
}

void * AT_Cmd_getCmd(void) {

	AT_getCmdData(cmd);

	AT_DMA_clearUartRxBuffer();

	status = AT_checkCmdHdr((char *)cmd);

	if(status.AT_err != ERR_NONE)
		return AT_errorRoutine;

	AT_splitString((char *)cmd, "+", cmdStr, AT_False);

	status = AT_checkCmdString(cmdStr);

	if(status.AT_err != ERR_NONE)
		return AT_errorRoutine;

	atCmdFlags.cmdType = status.AT_type;

	return AT_Cmd;
}

void * AT_errorRoutine(void) {
	uint8_t ret[MAX_RESPONSE_SIZE+1];

	AT_DMA_clearUartRxBuffer();

	switch(status.AT_err) {
	case DUMMY:
		break;

	case ERR_NONE:

		error_code.at_error = AT_ERR_NONE;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	case ERR_PARAM_CMD:

		error_code.at_error = AT_ERR_PARAM_CMD;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	case ERR_UNAVAILABLE_CMD:

		error_code.at_error = AT_ERR_UNAVAILABLE_CMD;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	case ERR_HDR:

		error_code.at_error = AT_ERR_HDR;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	case ERR_OVF:

		error_code.at_error = AT_ERR_OVF;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	case ERR_INVALID_HEX_VALUE:

		error_code.at_error = (uint8_t)ERR_INVALID_HEX_VALUE;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	default:

		error_code.sfx_error = status.AT_err;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		break;
	}

	return AT_cmd_returnStatus;
}

void * AT_Mcu(void) {
	return AT_Mcu_executeCmd;
}

void * AT_Mcu_executeCmd(void) {
	int16_t tmp;
	int32_t argp;
	char str_tmp[10];

	switch(status.AT_mcuCmd) {
	case AT_deepSleep:
	{
		status.AT_mcuCmd = -1;

		uint8_t ret[MAX_RESPONSE_SIZE+1];

		error_code.at_error = AT_ERR_NONE;
		sprintf((char *)ret, "0x%02X%02X\n", error_code.sfx_error, error_code.at_error);
		HAL_UART_Transmit(&huart1, (uint8_t *)ret, MAX_RESPONSE_SIZE, 0xFFFF);

		HT_McuApi_enterDeepSleepMode();
		break;
	}
	case AT_reset:
	{
		HT_McuApi_softwareReset();
		break;
	}
	case AT_switch_pa:
	{
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", firstParam, AT_False);
		integerParam1 = firstParam[0] - '0';

		status.AT_err = HT_McuApi_switchPa(integerParam1);

		break;
	}
	case AT_switch_boost:
	{
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", firstParam, AT_False);
		integerParam1 = firstParam[0] - '0';

		status.AT_err = HT_McuApi_switchBoost(integerParam1);

		break;
	}
	case AT_reduce_power:
	{
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp, AT_False);
		tmp = atoi(str_tmp);

		status.AT_err = HT_McuApi_reduceOutputPower(tmp);

		break;
	}
	case AT_freq_offset:
	{
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp, AT_False);

		argp = atoi(str_tmp);

		status.AT_err = HT_McuApi_setFreqOffset(argp);

		break;
	}
	case AT_rssi_offset:
	{
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp, AT_False);

		argp = atoi(str_tmp);

		status.AT_err = HT_McuApi_setRssiOffset(argp);

		break;
	}
	case AT_lbt_offset:
	{
		status.AT_mcuCmd = -1;
		AT_splitString(cmdStr, "=", str_tmp, AT_False);

		argp = atoi(str_tmp);

		status.AT_err = HT_McuApi_setLbtOffset(argp);
		break;
	}
	default:
		return AT_errorRoutine;
	}

	return AT_errorRoutine;
}

void * AT_SigFox(void) {
	return AT_SigFox_executeCmd;
}

void * AT_SigFox_executeCmd(void) {
	sfx_u8 customer_response[8];
	sfx_u8 customer_data[12];
	uint16_t integerParam2;

	switch(status.AT_sigfoxCmd) {
	case AT_sendFrame:
	{
		AT_splitString(cmdStr, "=", firstParam, AT_False);

		integerParam1 = atoi(firstParam);
		if(!AT_ValidParameterFlag(integerParam1)) {
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		status.AT_err = AT_splitString(firstParam, ":", secParam, AT_True);
		if(status_bkp != ERR_NONE)
			return AT_errorRoutine;

		secParam[strlen(secParam)-1] = '\0';

		if(strlen((char *)secParam) > MAX_HEX_SIZE) {
			status.AT_err = ERR_OVF;
			return AT_errorRoutine;
		}

		if(!AT_IsHexPayload(secParam)) {
			status.AT_err = ERR_INVALID_HEX_VALUE;
			return AT_errorRoutine;
		}

		if(AT_NullString(secParam)) {
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		AT_getHexValue(secParam, (uint8_t *)customer_data);

		status.AT_err = HT_SigfoxApi_sendFrame(customer_data, customer_response, integerParam1, strlen((char *)secParam)/2);

		break;
	}
	case AT_monarchScan:
	{
		AT_splitString(cmdStr, "=", firstParam, AT_False);
		AT_splitString(firstParam, ":", secParam, AT_False);

		if(AT_NullString(firstParam) || AT_NullString(secParam)){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		integerParam1 = atoi(firstParam);

		if(!AT_ValidRcMask(integerParam1)) {
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		integerParam2 = atoi(secParam);

		integerParam1 = (integerParam1 == ALL_REGIONS) ? integerParam1 : HT_MonarchApi_getRcBitMask(integerParam1);

		status.AT_err = HT_MonarchApi_monarchScan((sfx_u8)integerParam1, (sfx_u16)integerParam2);

		break;
	}
	case AT_cfgrcz:
	{
		AT_splitString(cmdStr, "=", firstParam, AT_False);
		integerParam1 = firstParam[0] - '0';

		if(AT_NullString(firstParam)){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		if(!AT_ValidRcMask(integerParam1)) {
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		status.AT_err = HT_SigfoxApi_configRegion(integerParam1);

		break;
	}
	case AT_stpMonarch:
	{
		status.AT_err = HT_MonarchApi_stopMonarchScan();
		break;
	}
	case AT_close:
	{
		status.AT_err = HT_SigfoxApi_closeSigfoxLib();
		break;
	}
	case AT_id_pac:
	{
		status.AT_err = HT_SigfoxApi_GetCredentials();
		break;
	}
	case AT_cw:
	{
		char argp[10];
		uint32_t frequency;

		AT_splitString(cmdStr, "=", argp, AT_False);
		frequency = atoi(argp);

		if(AT_NullString(argp)){
			status.AT_err = ERR_PARAM_CMD;
			return AT_errorRoutine;
		}

		status.AT_err = HT_SigfoxApi_ContinuousWave(frequency);
		break;
	}

	case AT_stop_cw:
	{
		status.AT_err = HT_SigfoxApi_StopCW();
		break;
	}
	default:
		break;
	}

	return AT_errorRoutine;
}

void * AT_cmd_returnStatus(void) {
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

	memset((uint8_t *)&error_code, 0, sizeof(AT_ErrorCode));
	status.AT_err = 0x00;

	return AT_Cmd;
}

void AT_getHexValue(char *str, uint8_t *output) {
	char tmp[2];
	char aux[MAX_HEX_SIZE];
	uint8_t ind = 0;
	uint8_t i = 0;

	memset((uint8_t *)tmp, 0, strlen(tmp));
	memset((uint8_t *)aux, 0, MAX_HEX_SIZE);
	memset((uint8_t *)output, 0, MAX_PAYLOAD_SIZE);

	if(strlen(str) % 2 != 0) {

		aux[0] = '0';
		strcpy(&aux[1], str);

		memset((uint8_t *)str, 0, MAX_HEX_SIZE);
		strcpy(str, aux);
	}

	do {

		tmp[0] = str[ind];
		tmp[1] = str[++ind];

		ind++;

		output[i] = (uint8_t)strtol(tmp, NULL, 16);

		i++;
	} while (i < (strlen(str)/2));

}

AT_cmdStrError AT_splitString(char *cmd, char *splitChar, char *ptr, AT_TxFlag tx_flag) {
	char aux[DMA_RX_BUFFER_SIZE];

	strcpy(aux, cmd);

	char *token = strtok((char *)aux, splitChar);

	token = strtok(NULL, splitChar);

	if(tx_flag == AT_True && (strlen(token)-1) > MAX_HEX_SIZE) {
		token = NULL;
		return ERR_OVF;
	}

	strcpy(ptr, token);
	token = NULL;

	return ERR_NONE;
}

void AT_setCmdFlag(AT_flagu8 flag) {
	atCmdFlags.cmdReceived = flag;
}

void AT_updateFsm(AT_cmdStrError errStatus, AT_cmdType cmdType) {
	this_errStatus = errStatus;
	current_state = (state_func)(current_state)();
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

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
