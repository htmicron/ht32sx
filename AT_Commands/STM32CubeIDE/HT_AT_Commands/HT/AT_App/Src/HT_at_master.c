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

/* Includes------------------------------------------------------------------------- */

#include "HT_at_master.h"
#include "HT_sigfox_api.h"
#include "HT_monarch_api.h"
#include "HT_mcu_api.h"
#include "HT_uart_api.h"
#include "usart.h"
#include "crc.h"
#include "string.h"

/* Global variables and constants --------------------------------------------------- */

static const AT_Command *AT_command_tab[] = {
		"IDPAC",
		"CFGRCZ",
		"SEND",
		"SENDCRC",
		"SENDBIT",
		"MONARCH",
		"STPMONARCH",
		"CLOSE",
		"CW",
		"STPCW",
		"RESET",
		"DEEPSLEEP",
		"SWITCHPA",
		"SWITCHBOOST",
		"REDUCEPOWER",
		"FREQOFFSET",
		"RSSIOFFSET",
		"LBTOFFSET"
};

/* Function Prototypes ------------------------------------------------------------ */

/*!******************************************************************
 * \fn static uint8_t HT_AT_GetCommandLen(uint8_t *cmd)
 * \brief Gets the command string length.
 *
 * \param[in]  uint8_t *cmd					Command string.
 * \param[out] none
 *
 * \retval Command string length.
 *******************************************************************/
static uint8_t HT_AT_GetCommandLen(uint8_t *cmd);

/*!******************************************************************
 * \fn static uint8_t HT_AT_IsHexValue(char value)
 * \brief Check if a specific ASCII character is a valid hexadecimal value.
 *
 * \param[in] char value					Character to be checked.
 * \param[out] none
 *
 * \retval 1 if it is a valid hex value, 0 if not.
 *******************************************************************/
static uint8_t HT_AT_IsHexValue(char value);

/*!******************************************************************
 * \fn static uint8_t HT_AT_ValidCrc(HT_AT_Parameter *argp)
 * \brief Check if the received payload parameter has the right CRC.
 *
 * \param[in] HT_AT_Parameter *argp			Parameter table.
 * \param[out] none
 *
 * \retval 1 if it is a valid hex value, 0 if not.
 *******************************************************************/
static uint8_t HT_AT_ValidCrc(HT_AT_Parameter *argp);

/*!******************************************************************
 * \fn static uint8_t HT_AT_IsHexPayload(char *payload)
 * \brief Check if the entire payload string is made up of valid hexadecimal values.
 *
 * \param[in] char *payload					Payload string to be checked.
 * \param[out] none
 *
 * \retval 1 if the entire payload is made up of valid hexadecimal values, 0 if not.
 *******************************************************************/
static uint8_t HT_AT_IsHexPayload(char *payload);

/*!******************************************************************
 * \fn static uint8_t HT_AT_ValidRcMask(uint8_t rc_mask)
 * \brief Check if a specific number is a Sigfox RCZ mask.
 *
 * \param[in] uint8_t rc_mask				RC number received by an user command.
 * \param[out] none
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_AT_ValidRcMask(uint8_t rc_mask);

/*!******************************************************************
 * \fn static uint8_t HT_AT_ValidParameterFlag(uint8_t param)
 * \brief Check if it is a valid parameter flag (if it is 0 or 1).
 *
 * \param[in] uint8_t param					Parameter to be checked.
 * \param[out] none
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_AT_ValidParameterFlag(uint8_t param);

/*!******************************************************************
 * \fn static uint8_t HT_AT_NullParameter(uint8_t *param)
 * \brief Verify if the received parameter has a null string.
 *
 * \param[in] uint8_t *param					Parameter to be checked.
 * \param[out] none
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_AT_NullParameter(uint8_t *param);

/*!******************************************************************
 * \fn static uint8_t HT_AT_NullParameter(uint8_t *param)
 * \brief Verify if the received parameter has a null string.
 *
 * \param[in] uint8_t *param					Parameter to be checked.
 * \param[out] none
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_AT_NullParameter(uint8_t *param);

/*!******************************************************************
 * \fn static uint8_t HT_AT_CheckNumberOfParameter(uint8_t *param, uint8_t nParam)
 * \brief Verify if the received command was sent with the expected number of parameters.
 *
 * \param[in] uint8_t *param					Parameter to be checked.
 * \param[in] uint8_t nParam					Expected number of parameters.
 * \param[out] none
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_AT_CheckNumberOfParameter(uint8_t *param, uint8_t nParam);

/*!******************************************************************
 * \fn static uint8_t HT_AT_ValidLength(uint8_t *param, uint8_t size)
 * \brief Compare a received parameter to an expected length and returns if both are equal.
 *
 * \param[in] uint8_t *param					Parameter to be checked.
 * \param[in] uint8_t nParam					Expected number of parameters.
 * \param[out] none
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_AT_ValidLength(uint8_t *param, uint8_t size);

/*!******************************************************************
 * \fn static void HT_AT_StrRev(char *x, uint8_t begin, uint8_t end)
 * \brief Recursive function to reverse a string.
 *
 * \param[in/out] char *x						String to be reverted (it is also the output of this function).
 * \param[in] uint8_t begin						First index of the string to be reverted.
 * \param[in] uint8_t end						Last index of the string to be reverted.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_StrRev(char *x, uint8_t begin, uint8_t end);

/*!******************************************************************
 * \fn static void HT_AT_GetInterval(int16_t k, HT_AT_Parameter *ptr, char *str, char delim)
 * \brief Gets the interval between two parameters.
 *
 * \param[in] int16_t k							First index of the string.
 * \param[in] char *str							String that the search is being done.
 * \param[in] char delim						Delimiter character.
 * \param[out] HT_AT_Parameter *ptr				Parameter output.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_GetInterval(int16_t k, HT_AT_Parameter *ptr, char *str, char delim);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_LbtOffsetCmd(uint8_t *ptr)
 * \brief Set LBT offset command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_LbtOffsetCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_RssiOffsetCmd(uint8_t *ptr)
 * \brief Set RSSI offset command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_RssiOffsetCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_FreqOffsetCmd(uint8_t *ptr)
 * \brief Set Frequency offset command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_FreqOffsetCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_FreqOffsetCmd(uint8_t *ptr)
 * \brief Set Frequency offset command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_FreqOffsetCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_ReducePowerCmd(uint8_t *ptr)
 * \brief Reduce output power command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_ReducePowerCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_SwitchBoostCmd(uint8_t *ptr)
 * \brief Switch boost mode command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_SwitchBoostCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_SwitchBoostCmd(uint8_t *ptr)
 * \brief Switch boost mode command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_SwitchBoostCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_SwitchPACmd(uint8_t *ptr)
 * \brief Switch PA mode command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_SwitchPACmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_DeepSleepCmd(uint8_t *ptr)
 * \brief Start Deep Sleep Mode command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_DeepSleepCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_ResetCmd(void)
 * \brief Reset device command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_ResetCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_StpCwCmd(uint8_t *ptr)
 * \brief Stop Continuous Wave command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_StpCwCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_CwCmd(uint8_t *ptr)
 * \brief Continuous Wave command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_CwCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_CloseCmd(uint8_t *ptr)
 * \brief Close Sigfox library command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_CloseCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_StopMonarchCmd(uint8_t *ptr)
 * \brief Stop Monarch command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode  HT_AT_StopMonarchCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_MonarchCmd(uint8_t *ptr)
 * \brief Monarch command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_MonarchCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_IdPacCmd(uint8_t *ptr)
 * \brief Get device ID/PAC command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_IdPacCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_CfgRczCmd(uint8_t *ptr)
 * \brief Configure Sigfox RCZ command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_CfgRczCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_SendBitCmd(uint8_t *ptr)
 * \brief Send Bit command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_SendBitCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_SendCrcCmd(uint8_t *ptr)
 * \brief Send Frame with CRC command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_SendCrcCmd(uint8_t *ptr);

/*!******************************************************************
 * \fn static HT_AT_ErrorCode HT_AT_SendCmd(uint8_t *ptr)
 * \brief Send Sigfox Frame command implementation.
 *
 * \param[in] uint8_t *ptr						Parameter pointer.
 *
 * \retval Error code of the command process.
 *******************************************************************/
static HT_AT_ErrorCode HT_AT_SendCmd(uint8_t *ptr);

/* Private Methods ----------------------------------------------------------------- */

static uint8_t HT_AT_GetCommandLen(uint8_t *cmd) {
	uint8_t len = 0;

	for (uint8_t i = 0; i < strlen((char *)cmd); i++) {
		if((char)cmd[i] == '=' || (char)cmd[i] == ';')
			break;

		len += 1;
	}

	return len;
}

static uint8_t HT_AT_IsHexValue(char value) {
	return (((uint8_t)value >= 0x30 && (uint8_t)value <= 0x39) || ((uint8_t)value >= 0x41 && (uint8_t)value <= 0x46)
			? 1 : 0);
}

static uint8_t HT_AT_ValidCrc(HT_AT_Parameter *argp) {
	uint32_t crc;
	char crc_str[9];

	crc = HT_CRC_CalculateCRC((uint8_t *)argp[1].param, strlen(argp[1].param));
	sprintf(crc_str, "%08X", (unsigned int)crc);

	return (!strcmp(crc_str, argp[2].param)) ? 1 : 0;
}

static uint8_t HT_AT_IsHexPayload(char *payload) {

	for (uint8_t i = 0; i < strlen(payload); i++) {
		if(!HT_AT_IsHexValue(payload[i]))
			return 0;
	}

	return 1;
}

static uint8_t HT_AT_ValidRcMask(uint8_t rc_mask) {
	return ((rc_mask >= 1 && rc_mask <= 7) || (rc_mask == 127)) ? 1 : 0;
}

static uint8_t HT_AT_ValidParameterFlag(uint8_t param) {
	return (param == 0 || param == 1 ? 1 : 0);
}

static uint8_t HT_AT_NullParameter(uint8_t *param) {
	return (strlen((char *)param) == 0 || param[0] == '\0') ? 1 : 0;
}

static uint8_t HT_AT_CheckNumberOfParameter(uint8_t *param, uint8_t nParam) {
	uint8_t cnt = 1;

	for (uint8_t i = 0; i < strlen((char *)param); i++) {
		if(i == 0) {
			if(((char)param[i] == PARAMETER_DELIMITER[0]) && ((!HT_AT_NullParameter(&param[i+1]) && i != 0))) {
				cnt += 1;
			}
		} else {
			if(((char)param[i] == PARAMETER_DELIMITER[0] && (char)param[i-1] != PARAMETER_DELIMITER[0]) && ((!HT_AT_NullParameter(&param[i+1]) && i != 0))) {
				cnt += 1;
			}
		}
	}

	return ((cnt == nParam) ? 1 : 0);
}

static uint8_t HT_AT_ValidLength(uint8_t *param, uint8_t size) {
	return (strlen((char *)param) <= size) ? 1 : 0;
}

static void HT_AT_StrRev(char *x, uint8_t begin, uint8_t end) {
	char c;

	if (begin >= end)
		return;

	c = *(x + begin);
	*(x + begin) = *(x + end);
	*(x + end) = c;

	HT_AT_StrRev(x, ++begin, --end);
}

static void HT_AT_GetInterval(int16_t k, HT_AT_Parameter *ptr, char *str, char delim) {
	int16_t j = 0;

	for (int16_t i = k; i >= 0; i--) {
		if (str[i] != delim) {
			ptr->param[j] = str[i];
			j += 1;
		} else {
			break;
		}
	}

	HT_AT_StrRev(ptr->param, 0, strlen(ptr->param)-1);
}

/* ---------------------------------Command Implementation ------------------------------------------------------*/

static HT_AT_ErrorCode HT_AT_LbtOffsetCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp;
	int32_t offset;
	char offset_str[11];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(offset_str, 0, sizeof(offset_str));
	argp.param = offset_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, &argp);

	offset = atoi(argp.param);
	error.at_cmd_error = HT_McuApi_setLbtOffset(offset);

	return error;
}

static HT_AT_ErrorCode HT_AT_RssiOffsetCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp;
	int32_t offset;
	char offset_str[11];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(offset_str, 0, sizeof(offset_str));
	argp.param = offset_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, &argp);

	offset = atoi(argp.param);
	error.at_cmd_error = HT_McuApi_setRssiOffset(offset);

	return error;
}

static HT_AT_ErrorCode HT_AT_FreqOffsetCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp;
	int32_t offset;
	char offset_str[11];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(offset_str, 0, sizeof(offset_str));
	argp.param = offset_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, &argp);

	offset = atoi(argp.param);
	error.at_cmd_error = HT_McuApi_setFreqOffset(offset);

	return error;
}

static HT_AT_ErrorCode HT_AT_ReducePowerCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp;
	int16_t reduce_value;
	char reduce_str[6];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(reduce_str, 0, sizeof(reduce_str));
	argp.param = reduce_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, &argp);

	reduce_value = atoi(argp.param);
	error.at_cmd_error = HT_McuApi_reduceOutputPower(reduce_value);

	return error;
}

static HT_AT_ErrorCode HT_AT_SwitchBoostCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	uint8_t param;
	uint8_t len;

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	len = strlen((char *)ptr);
	HT_AT_ParseCommandData((char *)ptr, len, &param);

	if(!HT_AT_ValidParameterFlag(param)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.at_cmd_error = HT_McuApi_switchBoost(param);

	return error;
}

static HT_AT_ErrorCode HT_AT_SwitchPACmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	uint8_t param;
	uint8_t len;

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	len = strlen((char *)ptr);
	HT_AT_ParseCommandData((char *)ptr, len, &param);

	if(!HT_AT_ValidParameterFlag(param)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.at_cmd_error = HT_McuApi_switchPa(param);

	return error;
}

static HT_AT_ErrorCode HT_AT_DeepSleepCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};

	if(!HT_AT_NullParameter(ptr)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	HAL_UART_Transmit(&huart1, (uint8_t *)"0x0000\n", MAX_RESPONSE_SIZE-1, 0xFFFF);

	HT_McuApi_enterDeepSleepMode();

	return error;
}

static HT_AT_ErrorCode HT_AT_ResetCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};

	if(!HT_AT_NullParameter(ptr)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	HT_McuApi_softwareReset();
	return error;
}

static HT_AT_ErrorCode HT_AT_StpCwCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};

	if(!HT_AT_NullParameter(ptr)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.sigfox_error = HT_SigfoxApi_StopCW();

	return error;
}

static HT_AT_ErrorCode HT_AT_CwCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp;
	uint32_t frequency;
	char freq_str[10];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(freq_str, 0, sizeof(freq_str));

	argp.param = freq_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, &argp);

	frequency = atoi(argp.param);
	error.sigfox_error = HT_SigfoxApi_ContinuousWave(frequency);

	return error;
}

static HT_AT_ErrorCode HT_AT_CloseCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};

	if(!HT_AT_NullParameter(ptr)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.sigfox_error = HT_SigfoxApi_closeSigfoxLib();

	return error;
}

static HT_AT_ErrorCode  HT_AT_StopMonarchCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};

	if(!HT_AT_NullParameter(ptr)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.sigfox_error = HT_MonarchApi_stopMonarchScan();

	return error;
}

static HT_AT_ErrorCode HT_AT_MonarchCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp[2];
	uint8_t rc_mask;
	uint16_t timeout;
	char rc_str[4], timeout_str[6];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 2)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(rc_str, 0, sizeof(rc_str));
	memset(timeout_str, 0, sizeof(timeout_str));

	argp[0].param = rc_str;
	argp[1].param = timeout_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, argp);

	rc_mask = atoi(argp[0].param);
	timeout = atoi(argp[1].param);

	if(!HT_AT_ValidRcMask(rc_mask)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	rc_mask = (rc_mask == ALL_REGIONS) ? rc_mask : HT_MonarchApi_getRcBitMask(rc_mask);

	error.sigfox_error = HT_MonarchApi_monarchScan((sfx_u8)rc_mask, (sfx_u16)timeout);

	return error;
}

static HT_AT_ErrorCode HT_AT_IdPacCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};

	if(!HT_AT_NullParameter(ptr)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.at_cmd_error = HT_SigfoxApi_GetCredentials();

	return error;
}

static HT_AT_ErrorCode HT_AT_CfgRczCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	uint8_t param;
	uint8_t len;

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 1)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	len = strlen((char *)ptr);
	HT_AT_ParseCommandData((char *)ptr, len, &param);

	if(!HT_AT_ValidRcMask(param)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.sigfox_error = HT_SigfoxApi_configRegion(param);

	return error;
}

static HT_AT_ErrorCode HT_AT_SendBitCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	HT_AT_Parameter argp[2];
	uint8_t downlink_flag;
	uint8_t bit;
	char downlink_str[2], bit_str[2];

	if(HT_AT_NullParameter(ptr) || !HT_AT_CheckNumberOfParameter(ptr, 2)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(downlink_str, 0, sizeof(downlink_str));
	memset(bit_str, 0, sizeof(bit_str));

	argp[0].param = downlink_str;
	argp[1].param = bit_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, argp);

	downlink_flag = atoi(argp[0].param);
	bit = atoi(argp[1].param);

	if(!HT_AT_ValidParameterFlag(downlink_flag) || !HT_AT_ValidParameterFlag(bit)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	error.sigfox_error = HT_SigfoxApi_sendBit(bit, downlink_flag);

	return error;
}

static HT_AT_ErrorCode HT_AT_SendCrcCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	uint8_t len;
	HT_AT_Parameter argp[3];
	uint8_t payload[SIGFOX_MAX_PAYLOAD_SIZE];
	uint8_t output_flag[2];
	char flag_str[MAX_FLAG_STRING], payload_str[MAX_PAYLOAD_STRING], crc_str[9];

	if(!HT_AT_CheckNumberOfParameter(ptr, 3)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(flag_str, 0, MAX_FLAG_STRING);
	memset(payload_str, 0, MAX_PAYLOAD_STRING);
	memset(crc_str, 0, sizeof(crc_str));

	argp[0].param = flag_str;
	argp[1].param = payload_str;
	argp[2].param = crc_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, argp);

	HT_AT_ParseCommandData(argp[0].param, strlen(argp[0].param), (uint8_t *)output_flag);
	if(!HT_AT_ValidParameterFlag(output_flag[0])) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	if (!HT_AT_ValidLength((uint8_t *)argp[1].param, MAX_PAYLOAD_STRING-1)) {
		error.at_cmd_error = AT_ERROR_OVF;
		return error;
	}

	if(!HT_AT_ValidCrc(argp)) {
		error.at_cmd_error = AT_ERROR_INVALID_CRC;
		return error;
	}

	if(!HT_AT_IsHexPayload(argp[1].param)) {
		error.at_cmd_error = AT_ERROR_INVALID_HEX_VALUE;
		return error;
	}

	len = strlen(argp[1].param);
	memset(payload, 0x00, SIGFOX_MAX_PAYLOAD_SIZE);

	if((len % 2) != 0) {

		char aux[SIGFOX_MAX_PAYLOAD_SIZE*2];

		memset(aux, 0, SIGFOX_MAX_PAYLOAD_SIZE*2);
		aux[0] = '0';
		strcpy(&aux[1], argp[1].param);

		len += 1;
		HT_AT_ParseCommandData(aux, (uint8_t)(len/2), payload);
	} else{
		HT_AT_ParseCommandData(argp[1].param, (uint8_t)(len/2), payload);
	}

	error.sigfox_error = HT_SigfoxApi_sendFrame(payload, output_flag[0], (uint8_t)(len/2));

	return error;
}

static HT_AT_ErrorCode HT_AT_SendCmd(uint8_t *ptr) {
	HT_AT_ErrorCode error = {0};
	uint8_t len;
	HT_AT_Parameter argp[2];
	uint8_t payload[SIGFOX_MAX_PAYLOAD_SIZE];
	uint8_t output_flag[2];
	char flag_str[MAX_FLAG_STRING], payload_str[MAX_PAYLOAD_STRING];

	if(!HT_AT_CheckNumberOfParameter(ptr, 2)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	memset(flag_str, 0, MAX_FLAG_STRING);
	memset(payload_str, 0, MAX_PAYLOAD_STRING);

	argp[0].param = flag_str;
	argp[1].param = payload_str;

	HT_AT_SplitCommandData(ptr, (char *)PARAMETER_DELIMITER, argp);

	if(HT_AT_NullParameter((uint8_t *)argp[0].param) || HT_AT_NullParameter((uint8_t *)argp[1].param)) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	HT_AT_ParseCommandData(argp[0].param, strlen(argp[0].param), (uint8_t *)output_flag);
	if(!HT_AT_ValidParameterFlag(output_flag[0])) {
		error.at_cmd_error = AT_ERROR_PARAM_CMD;
		return error;
	}

	if (!HT_AT_ValidLength((uint8_t *)argp[1].param, MAX_PAYLOAD_STRING-1)) {
		error.at_cmd_error = AT_ERROR_OVF;
		return error;
	}

	if(!HT_AT_IsHexPayload(argp[1].param)) {
		error.at_cmd_error = AT_ERROR_INVALID_HEX_VALUE;
		return error;
	}

	len = strlen((char *)&ptr[2]);
	memset(payload, 0x00, SIGFOX_MAX_PAYLOAD_SIZE);

	if((len % 2) != 0) {

		char aux[SIGFOX_MAX_PAYLOAD_SIZE*2];

		memset(aux, 0, SIGFOX_MAX_PAYLOAD_SIZE*2);
		aux[0] = '0';
		strcpy(&aux[1], (char *)&ptr[2]);

		len += 1;
		HT_AT_ParseCommandData(aux, (uint8_t)(len/2), payload);
	} else{
		HT_AT_ParseCommandData((char *)&ptr[2], (uint8_t)(len/2), payload);
	}

	error.sigfox_error = HT_SigfoxApi_sendFrame(payload, output_flag[0], (uint8_t)(len/2));

	return error;
}

HT_AT_ErrorCode HT_AT_ExecuteCommand(uint8_t *ptr, HT_AT_Commands cmd) {
	HT_AT_ErrorCode error = {0};

	switch (cmd) {
	case AT_SIGFOX_IDPAC_CMD:

		error = HT_AT_IdPacCmd(ptr);
		break;
	case AT_SIGFOX_CFGRCZ_CMD:

		error = HT_AT_CfgRczCmd(ptr);
		break;
	case AT_SIGFOX_SEND_CMD:

		error = HT_AT_SendCmd(ptr);
		break;
	case AT_SIGFOX_SENDCRC_CMD:

		error = HT_AT_SendCrcCmd(ptr);
		break;
	case AT_SIGFOX_SENDBIT_CMD:

		error = HT_AT_SendBitCmd(ptr);
		break;
	case AT_SIGFOX_MONARCH_CMD:

		error = HT_AT_MonarchCmd(ptr);
		break;
	case AT_SIGFOX_STPMONARCH_CMD:

		error = HT_AT_StopMonarchCmd(ptr);
		break;
	case AT_SIGFOX_CLOSE_CMD:

		error = HT_AT_CloseCmd(ptr);
		break;
	case AT_SIGFOX_CW:

		error = HT_AT_CwCmd(ptr);
		break;
	case AT_SIGFOX_STPCW_CMD:

		error = HT_AT_StpCwCmd(ptr);
		break;
	case AT_MCU_RESET_CMD:

		error = HT_AT_ResetCmd(ptr);
		break;
	case AT_MCU_DEEPSLEEP_CMD:

		error = HT_AT_DeepSleepCmd(ptr);
		break;
	case AT_MCU_SWITCHPA_CMD:

		error = HT_AT_SwitchPACmd(ptr);
		break;
	case AT_MCU_SWITCHBOOST_CMD:

		error = HT_AT_SwitchBoostCmd(ptr);
		break;
	case AT_MCU_REDUCEPOWER_CMD:

		error = HT_AT_ReducePowerCmd(ptr);
		break;
	case AT_MCU_FREQOFFSET_CMD:

		error = HT_AT_FreqOffsetCmd(ptr);
		break;
	case AT_MCU_RSSIOFFSET_CMD:

		error = HT_AT_RssiOffsetCmd(ptr);
		break;
	case AT_MCU_LBTOFFSET_CMD:

		error = HT_AT_LbtOffsetCmd(ptr);
		break;
	default:
		break;
	}

	return error;
}

/* Public methods -------------------------------------------------------------------- */

void HT_AT_SplitCommandData(uint8_t *cmdBuffer, char *delim, HT_AT_Parameter *ptr) {
	int16_t k = 0;
	int16_t count = 0;
	int16_t i;

	for (i = 0; i < strlen((char *)cmdBuffer); i++) {
		if ((char)cmdBuffer[i] == *delim)
			count += 1;
	}

	for (i = 0; i < strlen((char *)cmdBuffer); i++){
		if ((char)cmdBuffer[i] == *delim || i == (strlen((char *)cmdBuffer)-1)) {
			HT_AT_GetInterval(i != (strlen((char *)cmdBuffer)-1) ? (i - 1) : i, &ptr[k], (char *)cmdBuffer, *delim);
			if (k < count)
				k += 1;
			else
				break;
		}
	}

}

void HT_AT_ParseCommandData(char *str, uint8_t len, uint8_t *output) {
	char tmp[2];
	uint8_t ind = 0;
	uint8_t i = 0;

	do {
		tmp[0] = str[ind];
		tmp[1] = str[++ind];

		ind++;

		output[i] = (uint8_t)strtol(tmp, NULL, 16);

		i++;
	} while (i < len);
}

HT_ATCommand_ErrorCode HT_AT_ValidCommand(uint8_t *cmd, HT_AT_Commands *cmd_id) {
	uint8_t len = HT_AT_GetCommandLen(cmd);

	for (uint16_t i = 0; i < N_CMD(AT_command_tab); i++) {
		if(!strncmp(AT_command_tab[i], (char *)cmd, len)) {
			*cmd_id = (HT_AT_Commands)i;
			return AT_ERROR_NONE;
		}
	}

	return AT_ERROR_UNAVAILABLE_CMD;
}

uint8_t HT_AT_GetCommandStrLen(HT_AT_Commands cmd) {
	return (strlen(AT_command_tab[cmd]));
}

uint8_t HT_AT_CheckErrorCode(HT_AT_ErrorCode code)  {
	return (code.at_cmd_error == AT_ERROR_NONE && code.uart_error == UART_ERROR_NONE) ? 1 : 0;
}

HT_ATCommand_ErrorCode HT_AT_ValidHeaderCommand(uint8_t *str) {
	return (!strncmp((char *)str, HEADER_STR, strlen((char *)HEADER_STR)) ? AT_ERROR_NONE :
			AT_ERROR_HDR);
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
