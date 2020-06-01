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

void AT_cleanAuxBuffer(void) {
	memset(auxBuffer, 0, sizeof(auxBuffer));
}

void AT_getCmdData(uint8_t *cmd) {
	strcpy((char *)cmd, (char *)auxBuffer);
}

AT_cmdStruct AT_checkCmdHdr(char *cmd) {
	AT_cmdStruct status;

	if(!strncmp(cmd, "AT+", strlen("AT+")))
		status.AT_err = ERR_NONE;
	else if(cmd[0] == ';')
		status.AT_err = ERR_OVF;
	else
		status.AT_err = ERR_HDR;

	return status;
}

AT_cmdStruct AT_checkCmdString(char *cmdStr) {
	AT_cmdStruct status;

	if(!strncmp(cmdStr, "SEND", strlen("SEND"))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_sendFrame;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "MONARCH", strlen("MONARCH"))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_monarchScan;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "CFGRCZ", strlen("CFGRCZ"))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_cfgrcz;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "STOP", strlen("STOP"))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_stop;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "DEEPSLEEP", strlen("DEEPSLEEP"))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_deepSleep;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "RESET", strlen("RESET"))) {
		status.AT_type = AT_mcu;
		status.AT_mcuCmd = AT_reset;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "STPMONARCH", strlen("STPMONARCH"))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_stpMonarch;
		status.AT_err = ERR_NONE;
	} else if(!strncmp(cmdStr, "CLOSE", strlen("CLOSE"))) {
		status.AT_type = AT_sigfox;
		status.AT_sigfoxCmd = AT_close;
		status.AT_err = ERR_NONE;
	} else {
		status.AT_err = ERR_UNAVAILABLE_CMD;
	}

	return status;
}

void AT_prepareBuffer(uint8_t *buffer) {
	uint8_t aux[DMA_RX_BUFFER_SIZE];

	memcpy(aux, auxBuffer, sizeof(auxBuffer));
	memset(auxBuffer, 0, sizeof(auxBuffer));

	for(uint8_t i = 0;i < sizeof(aux);i++) {
		if(aux[i] != '\0')
			auxBuffer[i] = aux[i];
	}
}

uint8_t AT_searchForNextFree(uint8_t *buffer) {
	uint8_t i;

	for(i = 0;i < DMA_RX_BUFFER_SIZE;i++) {
		if(buffer[i] == '\0')
			return i;
	}

	return 0;
}

void AT_setCmdStr(uint8_t *ptr, uint8_t Write, uint8_t tocopy) {
	AT_cmdStrError status = ERR_NONE;
	uint8_t i = AT_searchForNextFree(auxBuffer);

	memcpy(&auxBuffer[i], ptr, tocopy);

	if(*ptr == ';' || auxBuffer[strlen((char *)auxBuffer)-1] == ';') {
		AT_prepareBuffer(auxBuffer);
		AT_setCmdFlag(1);
		AT_updateFsm(status, 0);
	}

}

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
