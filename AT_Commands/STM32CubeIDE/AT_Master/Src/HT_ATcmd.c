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


/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "HT_ATcmd.h"

uint8_t auxBuffer[DMA_RX_BUFFER_SIZE];
uint8_t cmdBuffer[DMA_RX_BUFFER_SIZE];

/* Cmd header -----------------------------------------------------------*/
static char AT_hdr[] = {"AT+"};

/* Available commands -----------------------------------------------------------*/
static char AT_send_str[] = {"SEND"};
static char AT_monarch_str[] = {"MONARCH"};
static char AT_cfgrcz_str[] = {"CFGRCZ"};
static char AT_stop_str[] = {"STOP"};
static char AT_deep_sleep_str[] = {"DEEPSLEEP"};
static char AT_reset_str[] = {"RESET"};
static char AT_stp_monarch_str[] = {"STPMONARCH"};
static char AT_close_str[] = {"CLOSE"};
static char AT_switch_pa_str[] = {"SWITCHPA"};
static char AT_switch_boost_str[] = {"SWITCHBOOST"};
static char AT_reduce_power_str[] = {"REDUCEPOWER"};
static char AT_freq_offset_str[] = {"FREQOFFSET"};
static char AT_rssi_offset_str[] = {"RSSIOFFSET"};
static char AT_lbt_offset_str[] = {"LBTOFFSET"};

void AT_cleanAuxBuffer(void) {
	memset(auxBuffer, 0, sizeof(auxBuffer));
}

void AT_getCmdData(uint8_t *cmd) {
	strcpy((char *)cmd, (char *)auxBuffer);
}

AT_cmdStruct AT_checkCmdHdr(char *cmd) {
	AT_cmdStruct status;

	if(!strncmp(cmd, AT_hdr, strlen(AT_hdr)))
		status.AT_err = ERR_NONE;
	else if(cmd[0] == ';')
		status.AT_err = ERR_OVF;
	else
		status.AT_err = ERR_HDR;

	return status;
}

AT_cmdStruct AT_checkCmdString(char *cmdStr) {
	AT_cmdStruct status;

	if(!strncmp(cmdStr, AT_send_str, strlen(AT_send_str))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_sendFrame;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_monarch_str, strlen(AT_monarch_str))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_monarchScan;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_cfgrcz_str, strlen(AT_cfgrcz_str))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_cfgrcz;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_stop_str, strlen(AT_stop_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_stop;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_deep_sleep_str, strlen(AT_deep_sleep_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_deepSleep;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_reset_str, strlen(AT_reset_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_reset;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_stp_monarch_str, strlen(AT_stp_monarch_str))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_stpMonarch;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_close_str, strlen(AT_close_str))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_close;
		status.AT_err = ERR_NONE;
	}  else if(!strncmp(cmdStr, AT_switch_pa_str, strlen(AT_switch_pa_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_switch_pa;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_switch_boost_str, strlen(AT_switch_boost_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_switch_boost;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_reduce_power_str, strlen(AT_reduce_power_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_reduce_power;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_freq_offset_str, strlen(AT_freq_offset_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_freq_offset;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_rssi_offset_str, strlen(AT_rssi_offset_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_rssi_offset;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, AT_lbt_offset_str, strlen(AT_lbt_offset_str))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_lbt_offset;
		status.AT_err = ERR_NONE;
	} else {
		status.AT_err = ERR_UNAVAILABLE_CMD;
	}

	return status;
}

void AT_setCmdStr(uint8_t *ptr) {
	AT_cmdStrError status = ERR_NONE;

	memcpy(auxBuffer, ptr, strlen((char *)ptr));
	AT_setCmdFlag(1);
	AT_updateFsm(status, 0);

}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
