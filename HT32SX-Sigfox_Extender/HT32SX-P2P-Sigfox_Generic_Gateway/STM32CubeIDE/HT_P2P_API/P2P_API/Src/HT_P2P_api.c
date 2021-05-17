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

#include "HT_P2P_api.h"
#include "HT_data_repeater.h"

extern volatile FlagStatus xStartRx, rx_timeout, exitTime;
extern volatile FlagStatus xRxDoneFlag, xTxDoneFlag, cmdFlag;

uint16_t exitCounter = 0;
uint16_t txCounter = 0;

S2LPIrqs xIrqStatus;

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
	FEM_Operation(FEM_SHUTDOWN);
#endif

	S2LP_PacketConfig();

	S2LPRadioSetRssiThreshdBm(RSSI_THRESHOLD);
}

void HT_P2P_Radio_Init(void) {
	S2LPInterfaceInit();
}

void HT_P2P_BasicProtocolInit(void) {
	/* Radio Packet config */
	S2LPPktBasicInit(&xBasicInit);
}

void HT_P2P_SendPayload(HT_DR_Payload *xTxFrame, uint8_t dst_addr) {
	uint8_t tmp = 0x72;

	S2LPPktBasicAddressesInit(&xAddressInit);

	S2LPGpioIrqDeInit(NULL);

	S2LP_EnableTxIrq();

	/* payload length config */
	S2LP_SetPayloadlength(sizeof(HT_DR_Payload));

	/* IRQ registers blanking */
	S2LPGpioIrqClearStatus();

	/* Destination address. It could be also changed to BROADCAST_ADDRESS or MULTICAST_ADDRESS. */
	S2LP_SetDestinationAddress(dst_addr);

#ifndef BASE_FREQ_433
	FEM_Operation(FEM_TX);
#endif

	/* S2LP Boost mode*/
	S2LPSpiWriteRegisters(0x79, sizeof(tmp), &tmp);

	/* send the TX command */
	S2LP_StartTx((uint8_t *)xTxFrame, sizeof(HT_DR_Payload));
}

void HT_P2P_ReceivePayload(uint8_t src_addr) {

	/*float rRSSIValue = 0;*/
	exitTime = SET;
	exitCounter = TIME_TO_EXIT_RX;

	S2LPPktBasicAddressesInit(&xAddressInit);

	/* S2LP IRQs disable */
	S2LPGpioIrqDeInit(NULL);

	/* S2LP IRQs enable */
	S2LP_EnableRxIrq();

	/* payload length config */
	S2LP_SetPayloadlength(sizeof(HT_DR_Payload));

	S2LPTimerSetRxTimerMs(1000);

	/* destination address */
	S2LP_SetDestinationAddress(src_addr);

	/* IRQ registers blanking */
	S2LPGpioIrqClearStatus();

#ifndef BASE_FREQ_433
	FEM_Operation(FEM_RX);
#endif

	/* RX command */
	S2LP_StartRx();
}

void HT_P2P_InterruptHandler(void) {

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
