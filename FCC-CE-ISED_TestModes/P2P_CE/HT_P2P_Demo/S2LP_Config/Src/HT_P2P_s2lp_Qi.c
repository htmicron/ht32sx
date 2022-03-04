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

#include "HT_P2P_s2lp_Qi.h"
#include "HT_P2P_s2lp_regs.h"
#include "spi.h"

void S2LPRadioSetRssiThreshdBm(int32_t wRssiThrehsold) {
  uint8_t tmp;
  s_assert_param(IS_RSSI_DBM(wRssiThrehsold));

  tmp = S2LPRadioRssidBmToReg(wRssiThrehsold);
  //tmp = 0x07;
  g_xStatus = S2LPSpiWriteRegisters(RSSI_TH_ADDR, 1, &tmp);
}

void S2LPRadioSetPqiCheck(uint8_t cPqiLevel) {
  uint8_t tmp;
  s_assert_param(IS_PQI_LVL_CHECK(cPqiLevel));

  S2LPSpiReadRegisters(QI_ADDR, 1, &tmp);
  tmp &= ~PQI_TH_REGMASK;
  tmp |= (((uint8_t)cPqiLevel)<<1);
  S2LPSpiWriteRegisters(QI_ADDR, 1, &tmp);

}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
