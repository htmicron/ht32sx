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

#include "HT_P2P_s2lp_timer.h"
#include "HT_P2P_s2lp_regs.h"
#include "HT_P2P_s2lp_types.h"
#include "spi.h"
#include "HT_P2P_s2lp_config.h"

void S2LPTimerComputeRxTimerRegValues(uint32_t lDesiredUsec , uint8_t* pcCounter , uint8_t* pcPrescaler) {
  uint32_t f_dig = S2LPRadioGetXtalFrequency();
  uint32_t n;
  uint64_t tgt,tgt1,tgt2;

  /* if xtal is doubled divide it by 2 */
  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  /* N cycles in the time base of the timer:
     - clock of the timer is f_dig/1210
     - divide times 1000000 more because we have an input in us
  */
  tgt=(uint64_t)lDesiredUsec*f_dig;
  n=(uint32_t)(tgt/1210000000);
  tgt1=(uint64_t)1210000000*n;
  tgt2=(uint64_t)1210000000*(n+1);

  n=((tgt2-tgt)<(tgt-tgt1))?(n+1):(n);

  /* check if it is possible to reach that target with prescaler and counter of S2LP */
  if(n/0xFF>0xFD) {
    /* if not return the maximum possible value */
    (*pcCounter) = 0xFF;
    (*pcPrescaler) = 0xFF;
    return;
  }

  /* prescaler is really 2 as min value */
  (*pcPrescaler)=(n/0xFF)+2;
  (*pcCounter) = n / (*pcPrescaler);


  /* decrement prescaler and counter according to the logic of this timer in S2LP */
  (*pcPrescaler)--;

  if((*pcCounter)==0)
    (*pcCounter)=1;
}

void S2LPTimerSetRxTimerCounter(uint8_t cCounter) {
  g_xStatus = S2LPSpiWriteRegisters(TIMERS5_ADDR, 1, &cCounter);
}

void S2LPTimerSetRxTimerUs(uint32_t lDesiredUsec) {
  uint8_t tmpBuffer[2];
  S2LPTimerComputeRxTimerRegValues(lDesiredUsec , &tmpBuffer[0] , &tmpBuffer[1]);
  g_xStatus = S2LPSpiWriteRegisters(TIMERS5_ADDR, 2, tmpBuffer);
}


void S2LPTimerSetRxTimerStopCondition(RxTimeoutStopCondition xStopCondition) {
  uint8_t tmp;

  s_assert_param(IS_RX_TIMEOUT_STOP_CONDITION(xStopCondition));

  S2LPSpiReadRegisters(PROTOCOL2_ADDR, 1, &tmp);
  tmp &= ~(CS_TIMEOUT_MASK_REGMASK | SQI_TIMEOUT_MASK_REGMASK | PQI_TIMEOUT_MASK_REGMASK);
  tmp |= (((uint8_t)xStopCondition) << 5);
  S2LPSpiWriteRegisters(PROTOCOL2_ADDR, 1, &tmp);

  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
  tmp &= ~RX_TIMEOUT_AND_OR_SEL_REGMASK;
  tmp |= (((uint8_t)xStopCondition) >> 1);
  g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmp);
}

void S2LPTimerSetRxTimerMs(float fDesiredMsec) {
  S2LPTimerSetRxTimerUs((uint32_t)(fDesiredMsec*1000));
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
