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

#include "HT_P2P_s2lp_packetHandler.h"
#include "spi.h"

void S2LPSetRxSourceReferenceAddress(uint8_t address) {
  g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS3_ADDR, 1, &address);
}

uint8_t S2LPGetReceivedDestinationAddress(void) {
  uint8_t tmp;
  g_xStatus = S2LPSpiReadRegisters(RX_ADDRE_FIELD0_ADDR, 1, &tmp);
  return tmp;
}

void S2LPPktCommonFilterOnCrc(SFunctionalState xNewState) {
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
  if(xNewState == S_ENABLE) {
    tmp |= CRC_FLT_REGMASK;
  }
  else {
    tmp &= ~CRC_FLT_REGMASK;
  }
  g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);

}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
