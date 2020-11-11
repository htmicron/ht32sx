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

#include "HT_P2P_app.h"

SM_State_t SM_State =  SM_STATE_START_RX;

static AppliFrame_t xTxFrame, xRxFrame;

volatile FlagStatus xStartRx=RESET, rx_timeout=RESET, exitTime=RESET;
volatile FlagStatus xRxDoneFlag = RESET, xTxDoneFlag=RESET, cmdFlag=RESET;

uint16_t exitCounter = 0;
uint16_t txCounter = 0;

uint8_t TxFrameBuff[MAX_BUFFER_LEN] = {0x00};

S2LPIrqs xIrqStatus;

uint8_t test_mode = 0;

PktBasicInit xBasicInit={
		PREAMBLE_LENGTH,
		SYNC_LENGTH,
		SYNC_WORD,
		VARIABLE_LENGTH,
		EXTENDED_LENGTH_FIELD,
		CRC_MODE,
		EN_ADDRESS,
		EN_FEC,
		EN_WHITENING
};

PktBasicAddressesInit xAddressInit={
		EN_FILT_MY_ADDRESS,
		MY_ADDRESS,
		EN_FILT_MULTICAST_ADDRESS,
		MULTICAST_ADDRESS,
		EN_FILT_BROADCAST_ADDRESS,
		BROADCAST_ADDRESS
};

SGpioInit xGpioIRQ={
		S2LP_GPIO_3,
		S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
		S2LP_GPIO_DIG_OUT_IRQ
};

SRadioInit xRadioInit = {
		BASE_FREQUENCY,
		MODULATION_SELECT,
		DATARATE,
		FREQ_DEVIATION,
		BANDWIDTH
};


void HT_P2P_Init(void) {
	/* S2LP IRQ config */
	S2LPGpioInit(&xGpioIRQ);

	/* S2LP Radio config */
	S2LPRadioInit(&xRadioInit);

	/* S2LP Radio set power */
	S2LPRadioSetMaxPALevel(S_DISABLE);

	FEM_Init();
#ifdef BASE_FREQ_433
	Config_RangeExt(PA_SHUTDOWN);
#endif

	S2LP_PacketConfig();

	S2LPRadioSetRssiThreshdBm(RSSI_THRESHOLD);
}

void HAL_Radio_Init(void) {
	S2LPInterfaceInit();
}

void BasicProtocolInit(void) {
	/* Radio Packet config */
	S2LPPktBasicInit(&xBasicInit);
}

void Set_KeyStatus(FlagStatus val) {

	HAL_Delay(500);

	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	__HAL_GPIO_EXTI_CLEAR_IT(SOFT_RESET_Pin);

	if(val==SET)
		SM_State = SM_STATE_SEND_DATA;
}

void P2P_StartRx(void) {
	printf("Start RX...\n");
	AppliReceiveBuff();
	SM_State = SM_STATE_WAIT_FOR_RX_DONE;
}

void P2P_WaitForRxDone(void) {
	if((RESET != xRxDoneFlag)||(RESET != rx_timeout)||(SET != exitTime)) {
		if((rx_timeout==SET)||(exitTime==RESET)) {
			rx_timeout = RESET;
			SM_State = SM_STATE_START_RX;
		} else if(xRxDoneFlag) {
			xRxDoneFlag=RESET;
			SM_State = SM_STATE_DATA_RECEIVED;
		}
	}
}

void P2P_DataReceived(uint8_t *pRxBuff, uint8_t *cRxlen) {
	uint8_t xIndex = 0;
	char received_str[TX_BUFFER_SIZE];
	uint8_t i = 0;

	S2LP_GetRxPacket(pRxBuff, cRxlen);

	xRxFrame.Cmd = pRxBuff[0];
	xRxFrame.CmdLen = pRxBuff[1];
	xRxFrame.Cmdtag = pRxBuff[2];
	xRxFrame.CmdType = pRxBuff[3];
	xRxFrame.DataLen = pRxBuff[4];

	for (xIndex = 5; xIndex < *cRxlen; xIndex++) {
		xRxFrame.DataBuff[xIndex] = pRxBuff[xIndex];
		received_str[i] = pRxBuff[xIndex];
		i++;
	}

	if(xRxFrame.Cmd != ACK_OK)
		printf("%s\n", (char *)received_str);

	if(xRxFrame.Cmd == LED_TOGGLE)
		SM_State = SM_STATE_TOGGLE_LED;

	if(xRxFrame.Cmd == ACK_OK)
		SM_State = SM_STATE_ACK_RECEIVED;
}

void P2P_SendAck(uint8_t *pTxBuff, uint8_t cTxlen) {
	printf("Sending ack...\n");

	xTxFrame.Cmd = ACK_OK;
	xTxFrame.CmdLen = 0x01;
	xTxFrame.Cmdtag = xRxFrame.Cmdtag;
	xTxFrame.CmdType = APPLI_CMD;
	xTxFrame.DataBuff = pTxBuff;
	xTxFrame.DataLen = cTxlen;

	HAL_Delay(DELAY_TX_LED_GLOW);
	AppliSendBuff(&xTxFrame, xTxFrame.DataLen);
	SM_State = SM_STATE_WAIT_FOR_TX_DONE;
}

void P2P_SendData(uint8_t *pTxBuff, uint8_t cTxlen) {
	printf("Sending data...\n");

	xTxFrame.Cmd = LED_TOGGLE;
	xTxFrame.CmdLen = 0x01;
	xTxFrame.Cmdtag = txCounter++;
	xTxFrame.CmdType = APPLI_CMD;
	xTxFrame.DataBuff = pTxBuff;
	xTxFrame.DataLen = cTxlen;

	AppliSendBuff(&xTxFrame, xTxFrame.DataLen);
	SM_State = SM_STATE_WAIT_FOR_TX_DONE;
}

void P2P_WaitForTxDone(void) {

	if(xTxDoneFlag) {
		xTxDoneFlag = RESET;

		if(xTxFrame.Cmd == LED_TOGGLE)
			SM_State = !test_mode ? SM_STATE_START_RX : SM_STATE_TEST_MODE;
		else if(xTxFrame.Cmd == ACK_OK)
			SM_State = SM_STATE_IDLE;
	}

	//HAL_Delay(50); /* tx test mode period */

}

void P2P_AckReceived(void) {

	printf("Ack received...\n");

	SM_State = SM_STATE_IDLE;
}

void P2P_ToggleLed(void) {
	uint8_t  dest_addr;

	GPIOA->BSRR = 1 << 5; /* turn on led */

	dest_addr = S2LPGetReceivedDestinationAddress();

	if ((dest_addr == MULTICAST_ADDRESS) || (dest_addr == BROADCAST_ADDRESS)) {
		/* in that case do not send ACK to avoid RF collisions between several RF ACK messages */
		HAL_Delay(200);
		SM_State = SM_STATE_IDLE;
	} else {
		SM_State = SM_STATE_SEND_ACK;
	}
}

void P2P_TestMode(uint8_t *pTxBuff, uint8_t cTxlen) {
	printf("Sending data...\n");

	test_mode = 1;

	xTxFrame.Cmd = LED_TOGGLE;
	xTxFrame.CmdLen = 0x01;
	xTxFrame.Cmdtag = txCounter++;
	xTxFrame.CmdType = APPLI_CMD;
	xTxFrame.DataBuff = pTxBuff;
	xTxFrame.DataLen = cTxlen;

	AppliSendBuff(&xTxFrame, xTxFrame.DataLen);
	SM_State = SM_STATE_WAIT_FOR_TX_DONE;
}

void P2P_Idle(void) {
	GPIOA->BSRR = 1 << 21; /*turn off led */

	SM_State = SM_STATE_START_RX;
}

void P2P_Process(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t *pRxBuff, uint8_t cRxlen) {

	switch(SM_State) {
	case SM_STATE_START_RX:
		P2P_StartRx();
		break;
	case SM_STATE_WAIT_FOR_RX_DONE:
		P2P_WaitForRxDone();
		break;
	case SM_STATE_DATA_RECEIVED:
		P2P_DataReceived(pRxBuff, &cRxlen);
		break;
	case SM_STATE_SEND_ACK:
		P2P_SendAck(pTxBuff, cTxlen);
		break;
	case SM_STATE_SEND_DATA:
		P2P_SendData(pTxBuff, cTxlen);
		break;
	case SM_STATE_WAIT_FOR_TX_DONE:
		P2P_WaitForTxDone();
		break;
	case SM_STATE_ACK_RECEIVED:
		P2P_AckReceived();
		break;
	case SM_STATE_TOGGLE_LED:
		P2P_ToggleLed();
		break;
	case SM_STATE_TEST_MODE:
		P2P_TestMode(pTxBuff, cTxlen);
		break;
	case SM_STATE_IDLE:
		P2P_Idle();
		break;
	}

}

void AppliSendBuff(AppliFrame_t *xTxFrame, uint8_t cTxlen) {
	uint8_t xIndex = 0;
	uint8_t trxLength = 0;
	uint8_t tmp = 0x72;

	S2LPPktBasicAddressesInit(&xAddressInit);

	TxFrameBuff[0] = xTxFrame->Cmd;
	TxFrameBuff[1] = xTxFrame->CmdLen;
	TxFrameBuff[2] = xTxFrame->Cmdtag;
	TxFrameBuff[3] = xTxFrame->CmdType;
	TxFrameBuff[4] = xTxFrame->DataLen;

	for(; xIndex < xTxFrame->DataLen; xIndex++) {
		TxFrameBuff[xIndex+5] =  xTxFrame->DataBuff[xIndex];
	}

	trxLength = (xIndex+5);

	S2LPGpioIrqDeInit(NULL);

	S2LP_EnableTxIrq();

	/* payload length config */
	S2LP_SetPayloadlength(trxLength);

	/* rx timeout config */
	S2LP_SetRxTimeout(RECEIVE_TIMEOUT);

	/* IRQ registers blanking */
	S2LPGpioIrqClearStatus();

	/* Destination address. It could be also changed to BROADCAST_ADDRESS or MULTICAST_ADDRESS. */
	S2LP_SetDestinationAddress(DESTINATION_ADDRESS);

#ifndef BASE_FREQ_433
	Config_RangeExt(PA_TX);
#endif

	/* S2LP Boost mode*/
	S2LPSpiWriteRegisters(0x79, sizeof(tmp), &tmp);

	/* send the TX command */
	S2LP_StartTx(TxFrameBuff, trxLength);
}

void AppliReceiveBuff(void) {
	/*float rRSSIValue = 0;*/
	exitTime = SET;
	exitCounter = TIME_TO_EXIT_RX;

	S2LPPktBasicAddressesInit(&xAddressInit);

	/* S2LP IRQs disable */
	S2LPGpioIrqDeInit(NULL);

	/* S2LP IRQs enable */
	S2LP_EnableRxIrq();

	/* payload length config */
	S2LP_SetPayloadlength(PAYLOAD_LEN);

	S2LPTimerSetRxTimerMs(700.0);
	SET_INFINITE_RX_TIMEOUT();

	/* destination address */
	S2LP_SetDestinationAddress(DESTINATION_ADDRESS);

	/* IRQ registers blanking */
	S2LPGpioIrqClearStatus();

#ifndef BASE_FREQ_433
	Config_RangeExt(PA_RX);
#endif

	/* RX command */
	S2LP_StartRx();
}

void P2PInterruptHandler(void) {

	S2LPGpioIrqGetStatus(&xIrqStatus);

	/* Check the S2LP TX_DATA_SENT IRQ flag */
	if(xIrqStatus.IRQ_TX_DATA_SENT)
		xTxDoneFlag = SET;

	/* Check the S2LP RX_DATA_READY IRQ flag */
	if((xIrqStatus.IRQ_RX_DATA_READY))
		xRxDoneFlag = SET;

	/* Restart receive after receive timeout*/
	if (xIrqStatus.IRQ_RX_TIMEOUT) {
		rx_timeout = SET;
		S2LPCmdStrobeRx();
	}

	/* Check the S2LP RX_DATA_DISC IRQ flag */
	if(xIrqStatus.IRQ_RX_DATA_DISC) {
		/* RX command - to ensure the device will be ready for the next reception */
		S2LPCmdStrobeRx();
	}
}

void HAL_SYSTICK_Callback(void) {
	if(exitTime) {
		/*Decreament the counter to check when 3 seconds has been elapsed*/
		exitCounter--;
		/*3 seconds has been elapsed*/
		if(exitCounter <= TIME_UP)
			exitTime = RESET;
	}
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
