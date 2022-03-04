
#include "HT_CE_App.h"
#include "HT_P2P_app.h"

#ifdef TX_DEVICE
HT_FSM_State state = SM_STATE_SEND_DATA;
#else
HT_FSM_State state = SM_STATE_START_RX;
#endif

volatile FlagStatus xStartRx=RESET, rx_timeout=RESET, exitTime=RESET;
volatile FlagStatus xRxDoneFlag = RESET, xTxDoneFlag=RESET, cmdFlag=RESET;

uint32_t rx_cnt = 0;

uint8_t timeout = 0;

#ifdef TX_DEVICE
HT_Payload txPayload = {0, PAYLOAD_DATA};
HT_Payload rxPayload = {0};
#else
HT_Payload txPayload = {0};
HT_Payload rxPayload = {0};
#endif

void P2P_StartRx(void) {
	printf("Start RX...\n");

	AppliReceiveBuff();
	state = SM_STATE_WAIT_FOR_RX_DONE;
}

void P2P_WaitForRxDone(void) {

	if((RESET != xRxDoneFlag)||(RESET != rx_timeout)||(SET != exitTime)) {

#ifdef TX_DEVICE
		if(exitTime == RESET) {
			rx_timeout = RESET;
			state = SM_STATE_START_RX;
		} else if(rx_timeout == SET && !timeout) {
			timeout = 1;
			state = SM_STATE_START_RX;
			rx_timeout = 0;
		} else if(rx_timeout == SET && timeout) {
			timeout = 0;
			state = SM_STATE_SEND_DATA;
			rx_timeout = 0;
		}
#else
		if(exitTime == RESET || rx_timeout == SET) {
			rx_timeout = RESET;
			state = SM_STATE_START_RX;
		}
#endif
		else if(xRxDoneFlag) {
			xRxDoneFlag=RESET;
			state = SM_STATE_DATA_RECEIVED;
		}
	}

}

void P2P_DataReceived(HT_Payload *pRxBuff) {
	uint8_t __attribute__((unused)) len = sizeof(HT_Payload);

	memset(pRxBuff, 0x00, sizeof(HT_Payload)); //clear buffer

	S2LP_GetRxPacket((uint8_t *)pRxBuff, &len);

	printf("[%ld] RX: { ", rx_cnt);
	for(uint8_t i = 0; i < PAYLOAD_SIZE; i++)
		printf("0x%02X ", pRxBuff->data[i]);
	printf("}\n");

	rx_cnt += 1;

	state = SM_STATE_SEND_DATA;

	HAL_Delay(200);
}

void P2P_SendData(HT_Payload *pTxBuff) {
	printf("Sending data...\n");

	AppliSendBuff(pTxBuff);
	state = SM_STATE_WAIT_FOR_TX_DONE;

	HAL_Delay(200);
}

void P2P_WaitForTxDone(void) {
	if(xTxDoneFlag) {
		xTxDoneFlag = RESET;
		state = SM_STATE_START_RX;
	}
}

void HT_CE_Fsm(void) {

	switch(state) {
	case SM_STATE_START_RX:

		P2P_StartRx();
		break;

	case SM_STATE_WAIT_FOR_RX_DONE:

		P2P_WaitForRxDone();
		break;

	case SM_STATE_DATA_RECEIVED:

		P2P_DataReceived(&rxPayload);

		memcpy(&txPayload, &rxPayload, sizeof(HT_Payload)); //copy rx buffer to tx buffer
		break;

	case SM_STATE_SEND_DATA:

		P2P_SendData(&txPayload);
		break;

	case SM_STATE_WAIT_FOR_TX_DONE:

		P2P_WaitForTxDone();
		break;
	}

}
