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

#include "HT_P2P_s2lp_interface.h"
#include "HT_P2P_s2lp_timer.h"
#include "HT_P2P_s2lp_packetHandler.h"
#include "HT_P2P_s2lp_types.h"
#include "S2LP_AUX_FEM.h"
#include "HT_P2P_s2lp_config.h"
#include "HT_P2P_s2lp_commands.h"
#include "HT_P2P_s2lp_util.h"
#include "HT_P2P_api.h"
#include "HT_P2P_s2lp_pktBasic.h"
#include "HT_P2P_s2lp_gpio.h"
#include "HT_P2P_s2lp_fifo.h"
#include "HT_P2P_s2lp_Qi.h"

extern SRadioInit xRadioInit;

void S2LP_PacketConfig(void) {
	HT_P2P_BasicProtocolInit();
}

void S2LP_EnableTxIrq(void) {
	/* S2LP IRQs enable */
	S2LPGpioIrqConfig(TX_DATA_SENT, S_ENABLE);
}

void S2LP_EnableRxIrq(void) {
	/* S2LP IRQs enable */
	S2LPGpioIrqConfig(RX_DATA_READY, S_ENABLE);
	S2LPGpioIrqConfig(RX_TIMEOUT, S_ENABLE);
	S2LPGpioIrqConfig(RX_DATA_DISC, S_ENABLE);
}

void S2LP_SetPayloadlength(uint8_t length) {
	/* payload length config */
	S2LPPktBasicSetPayloadLength(length);
}

void S2LP_GetRxPacket(uint8_t *buffer, uint8_t *cRxData) {
	uint8_t noofbytes = 0;
	/* when rx data ready read the number of received bytes */
	*cRxData=S2LPFifoReadNumberBytesRxFifo();
	noofbytes = *cRxData;
	/* read the RX FIFO */
	S2LPSpiReadFifo(noofbytes, buffer);

	S2LPCmdStrobeFlushRxFifo();
}

void S2LP_SetRxTimeout(float cRxTimeOut) {
	if(cRxTimeOut == 0) {
		/* rx timeout config */
		SET_INFINITE_RX_TIMEOUT();
		S2LPTimerSetRxTimerStopCondition(ANY_ABOVE_THRESHOLD);
	} else {
		/* RX timeout config */
		S2LPTimerSetRxTimerMs(cRxTimeOut);
		S2LP_EnableSQI();
		S2LPTimerSetRxTimerStopCondition(RSSI_AND_SQI_ABOVE_THRESHOLD);
	}
}

void S2LP_SetDestinationAddress(uint8_t address) {
	/* destination address */
	S2LPSetRxSourceReferenceAddress(address);
}

void S2LP_EnableSQI(void) {
	/* enable SQI check */
	S2LPRadioSetPqiCheck(0x00);
	S2LPRadioSetPqiCheck(S_ENABLE);
}

void S2LPInterfaceInit(void) {
	/* S2LP ON */
	S2LPEnterShutdown();
	HAL_Delay(10);
	S2LPExitShutdown();

	xRadioInit.lFrequencyBase = xRadioInit.lFrequencyBase + S2LPManagementGetOffset();
}

void S2LP_StartRx(void) {
	if(g_xStatus.MC_STATE==MC_STATE_RX) {
		S2LPCmdStrobeSabort();
	}
	/* RX command */
	S2LPCmdStrobeRx();
}

void S2LP_StartTx(uint8_t *buffer, uint8_t size) {
	if(g_xStatus.MC_STATE==MC_STATE_RX) {
		S2LPCmdStrobeSabort();
	}

	/* fit the TX FIFO */
	S2LPCmdStrobeFlushTxFifo();

	S2LPSpiWriteFifo(size, buffer);

	/* send the TX command */
	S2LPCmdStrobeTx();
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
