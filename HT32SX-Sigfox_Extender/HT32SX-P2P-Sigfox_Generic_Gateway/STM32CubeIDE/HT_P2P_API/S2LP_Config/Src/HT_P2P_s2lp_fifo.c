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

#include "HT_P2P_s2lp_fifo.h"
#include "HT_P2P_s2lp_regs.h"

uint8_t S2LPFifoReadNumberBytesRxFifo(void) {
  uint8_t tmp;
  g_xStatus = S2LPSpiReadRegisters(RX_FIFO_STATUS_ADDR, 1, &tmp);

  return tmp;
}

uint8_t S2LPFifoReadNumberBytesTxFifo(void) {
  uint8_t tmp;
  g_xStatus = S2LPSpiReadRegisters(TX_FIFO_STATUS_ADDR, 1, &tmp);

  return tmp;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
