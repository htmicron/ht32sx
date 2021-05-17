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

#include "HT_data_repeater.h"
#include "HT_P2P_api.h"
#include "HT_mcu_api.h"
#include "S2LP_AUX_FEM.h"
#include "HT_sigfox_api.h"

HT_DR_States state;

volatile FlagStatus xStartRx=RESET, rx_timeout=RESET, exitTime=RESET;
volatile FlagStatus xRxDoneFlag = RESET, xTxDoneFlag=RESET, cmdFlag=RESET;

HT_DR_Payload pRxBuff = {0}, pTxBuff = {0};

void HT_DR_GetData(void) {

	sprintf((char *)pTxBuff.data, "Hello World");

	if(MY_ADDRESS != FIRST_ADDRESS) {
		memcpy(pTxBuff.sfx_credentials, pRxBuff.sfx_credentials, sizeof(pRxBuff.sfx_credentials));
		memcpy(pTxBuff.sfx_context, pRxBuff.sfx_context, sizeof(pRxBuff.sfx_context));
		memcpy(pTxBuff.sfx_environment, pRxBuff.sfx_environment, sizeof(pRxBuff.sfx_environment));
	} else if(MY_ADDRESS == FIRST_ADDRESS) {
		HT_NVM_GetSigfoxContext(pTxBuff.sfx_credentials, pTxBuff.sfx_environment, pTxBuff.sfx_context);
	}
}

void HT_DR_StartRxState(void) {
	uint8_t address;

	printf("Start RX...\n");

	address = (pTxBuff.payload_type == SIGFOX_DATA && MY_ADDRESS != MAX_N_DEVICES) ? MY_ADDRESS+1 : MY_ADDRESS-1;

	printf("Receiving from 0x%02X\n", address);

	HT_P2P_ReceivePayload(address);
	state = SM_STATE_WAIT_FOR_RX_DONE;
}

void HT_DR_WaitForRxDoneState(void) {
	if((RESET != xRxDoneFlag)||(RESET != rx_timeout)||(SET != exitTime)) {
		if((rx_timeout==SET)||(exitTime==RESET)) {
			rx_timeout = RESET;
			state = (pTxBuff.payload_type == SIGFOX_DATA) ? SM_STATE_SEND_DATA : SM_STATE_START_RX; //send payload again until receive an ack signal
		} else if(xRxDoneFlag && !rx_timeout) {
			xRxDoneFlag=RESET;
			state = SM_STATE_DATA_RECEIVED;
		}
	}
}

void HT_DR_DataReceivedState(void) {
	uint8_t __attribute__((unused)) len;

	S2LP_GetRxPacket((uint8_t *)&pRxBuff, &len);

	printf("Payload received: %s\n", (char *)pRxBuff.data);
	state = (pRxBuff.payload_type == ACK_DATA) ? SM_STATE_ACK_RECEIVED : SM_STATE_SEND_ACK;
}

void HT_DR_SendDataState(void) {
	printf("Sending Sigfox data to 0x%02X...\n", MY_ADDRESS+1);

	pTxBuff.payload_type = SIGFOX_DATA;
	HT_DR_GetData();

	HT_P2P_SendPayload(&pTxBuff, MY_ADDRESS+1);
	state = SM_STATE_WAIT_FOR_TX_DONE;
}

void HT_DR_WaitForTxDoneState(void) {
	if(xTxDoneFlag) {
		xTxDoneFlag = RESET;

		if(pTxBuff.payload_type == SIGFOX_DATA)
			state = SM_STATE_START_RX;
		else {
			if(MY_ADDRESS == FIRST_ADDRESS)
				state = SM_STATE_DEEP_SLEEP;
			else if(MY_ADDRESS != FIRST_ADDRESS && MY_ADDRESS != MAX_N_DEVICES)
				state = SM_STATE_SEND_DATA;
			else
				state = SM_SEND_SFX_DATA;
		}
	}
}

void HT_DR_AckReceivedState(void) {
	printf("Ack received...\n");

	state = SM_STATE_DEEP_SLEEP;
}

void HT_DR_SendAckState(void) {
	printf("Sending ack to 0x%02X...\n", MY_ADDRESS-1);

	memset(&pTxBuff, 0, sizeof(HT_DR_Payload));
	pTxBuff.payload_type = ACK_DATA;
	sprintf((char *)pTxBuff.data, "ACK");

	HT_P2P_SendPayload(&pTxBuff, MY_ADDRESS-1);
	state = SM_STATE_WAIT_FOR_TX_DONE;
}

void HT_DR_SendSfxDataState(void) {

	printf("Sending to Sigfox Network...\n");

	HT_NVM_ConfigEEPROM(pRxBuff.sfx_credentials, pRxBuff.sfx_environment, pRxBuff.sfx_context);

	HT_Main_McuConfig();

	/* Change it to your current Sigfox region. */
	HT_API_ConfigRegion(RCZ2);

	HT_API_SendFrame(&pRxBuff);

	HAL_Delay(50);

	state = SM_STATE_DEEP_SLEEP;
}

void HT_DR_DeepSleepState(void) {

	printf("Sleeping...");

	if(MY_ADDRESS == FIRST_ADDRESS)
		HT_NVM_StoreContext();

	HT_McuApi_EnableRTCWkp(WAKE_UP_TIME-MY_ADDRESS);
	HT_McuApi_DeepSleepMode();
}

void HT_DR_Fsm(void) {

	switch(state) {
	case SM_STATE_START_RX:
		HT_DR_StartRxState();
		break;
	case SM_STATE_WAIT_FOR_RX_DONE:
		HT_DR_WaitForRxDoneState();
		break;
	case SM_STATE_DATA_RECEIVED:
		HT_DR_DataReceivedState();
		break;
	case SM_STATE_SEND_DATA:
		HT_DR_SendDataState();
		break;
	case SM_STATE_WAIT_FOR_TX_DONE:
		HT_DR_WaitForTxDoneState();
		break;
	case SM_STATE_ACK_RECEIVED:
		HT_DR_AckReceivedState();
		break;
	case SM_STATE_SEND_ACK:
		HT_DR_SendAckState();
		break;
	case SM_SEND_SFX_DATA:
		HT_DR_SendSfxDataState();
		break;
	case SM_STATE_DEEP_SLEEP:
		HT_DR_DeepSleepState();
		break;
	}
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
