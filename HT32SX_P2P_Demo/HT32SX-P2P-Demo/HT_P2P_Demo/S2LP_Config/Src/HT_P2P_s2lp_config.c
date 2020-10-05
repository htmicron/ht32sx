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

#include "HT_P2P_s2lp_config.h"
#include "HT_P2P_s2lp_commands.h"

static const uint16_t s_vectnBandwidth26M[90]=
{
  8001, 7951, 7684, 7368, 7051, 6709, 6423, 5867, 5414, \
    4509, 4259, 4032, 3808, 3621, 3417, 3254, 2945, 2703, \
      2247, 2124, 2015, 1900, 1807, 1706, 1624, 1471, 1350, \
        1123, 1062, 1005,  950,  903,  853,  812,  735,  675, \
          561,  530,  502,  474,  451,  426,  406,  367,  337, \
            280,  265,  251,  237,  226,  213,  203,  184,  169, \
              140,  133,  126,  119,  113,  106,  101,   92,   84, \
                70,   66,   63,   59,   56,   53,   51,   46,   42, \
                  35,   33,   31,   30,   28,   27,   25,   23,   21, \
                    18,   17,   16,   15,   14,   13,   13,   12,   11
};


/**
* @brief  The Xtal frequency. To be set by the user (see SetXtalFreq() function)
*/
static uint32_t s_lXtalFrequency=50000000;

void S2LPEnterShutdown(void) {
  /* Puts high the GPIO connected to shutdown pin */
  HAL_GPIO_WritePin(RADIO_SPI_CS_PIN_GPIO_Port, RADIO_GPIO_SDN_PIN_Pin, GPIO_PIN_SET);
}

void S2LPExitShutdown(void) {
  /* Puts low the GPIO connected to shutdown pin */
  HAL_GPIO_WritePin(RADIO_SPI_CS_PIN_GPIO_Port, RADIO_GPIO_SDN_PIN_Pin, GPIO_PIN_RESET);

  /* Delay to allow the circuit POR, about 700 us */
  for(volatile uint32_t i=0;i<0x1E00;i++);
}

SFunctionalState S2LPRadioGetDigDiv(void) {
  uint8_t tmp;

  g_xStatus = S2LPSpiReadRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);

  if(tmp & PD_CLKDIV_REGMASK) {
    return S_DISABLE;
  } else {
    return S_ENABLE;
  }
}

void S2LPRadioSetDigDiv(SFunctionalState xNewState) {
  uint8_t tmp;

  s_assert_param(IS_SFUNCTIONAL_STATE(xNewState));

  S2LPSpiReadRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp &= ~PD_CLKDIV_REGMASK;
  } else {
    tmp |= PD_CLKDIV_REGMASK;
  }
  g_xStatus = S2LPSpiWriteRegisters(XO_RCO_CONF1_ADDR, 1, &tmp);
}

void S2LPRadioComputeIF(uint32_t nIF, uint8_t* pcAnaIf, uint8_t* pcDigIf) {
  uint32_t f_dig=s_lXtalFrequency;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  (*pcAnaIf)=(uint8_t)((((uint64_t)nIF)<<13)*3/s_lXtalFrequency-100);
  (*pcDigIf)=(uint8_t)((((uint64_t)nIF)<<13)*3/f_dig-100);
}

void S2LPRadioSetMaxPALevel(SFunctionalState xNewState) {
  uint8_t tmp;

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 1, &tmp);

  if(xNewState == S_ENABLE) {
    tmp |= PA_MAXDBM_REGMASK;
  } else {
    tmp &= ~PA_MAXDBM_REGMASK;
  }

  g_xStatus = S2LPSpiWriteRegisters(PA_POWER0_ADDR, 1, &tmp);
}

uint32_t S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE) {
  uint32_t f_dig=s_lXtalFrequency;
  uint64_t dr;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  if(cE==0) {
    dr=((uint64_t)f_dig*cM);
    return (uint32_t)(dr>>32);
  }

  dr=((uint64_t)f_dig)*((uint64_t)cM+65536);

  return (uint32_t)(dr>>(33-cE));
}

void S2LPRadioSearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE) {
  uint32_t lDatarateTmp, f_dig=s_lXtalFrequency;
  uint8_t uDrE;
  uint64_t tgt1,tgt2,tgt;

  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  /* Search the exponent value */
  for(uDrE = 0; uDrE != 12; uDrE++) {
    lDatarateTmp = S2LPRadioComputeDatarate(0xFFFF, uDrE);
    if(lDatarate<=lDatarateTmp)
      break;
  }
  (*pcE) = (uint8_t)uDrE;

  if(uDrE==0) {
    tgt=((uint64_t)lDatarate)<<32;
    (*pcM) = (uint16_t)(tgt/f_dig);
    tgt1=(uint64_t)f_dig*(*pcM);
    tgt2=(uint64_t)f_dig*((*pcM)+1);
  }
  else {
    tgt=((uint64_t)lDatarate)<<(33-uDrE);
    (*pcM) = (uint16_t)((tgt/f_dig)-65536);
    tgt1=(uint64_t)f_dig*((*pcM)+65536);
    tgt2=(uint64_t)f_dig*((*pcM)+1+65536);
  }


  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);

}

uint32_t S2LPRadioComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs, uint8_t refdiv) {
  uint32_t f_xo=s_lXtalFrequency;

  if(cE==0) {
    return (uint32_t)(((uint64_t)f_xo*cM)>>22);
  }

  return (uint32_t)(((uint64_t)f_xo*(256+cM))>>(23-cE));
}

uint32_t S2LPRadioGetXtalFrequency(void) {
  return s_lXtalFrequency;
}

void S2LPRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE) {
  uint8_t uFDevE, tmp, bs = MIDDLE_BAND_FACTOR, refdiv = 1;
  uint32_t lFDevTmp;
  uint64_t tgt1,tgt2,tgt;

  s_assert_param(IS_F_DEV(lFDev, s_lXtalFrequency));

  S2LPSpiReadRegisters(SYNT3_ADDR, 1, &tmp);
  if((tmp&BS_REGMASK) == 0) {
    bs = HIGH_BAND_FACTOR;
  }

  if(S2LPRadioGetRefDiv()) {
    refdiv = 2;
  }

  /* Search the exponent of the frequency deviation value */
  for(uFDevE = 0; uFDevE != 12; uFDevE++) {
    lFDevTmp = S2LPRadioComputeFreqDeviation(255, uFDevE, bs, refdiv);
    if(lFDev<lFDevTmp)
      break;
  }
  (*pcE) = (uint8_t)uFDevE;

  if(uFDevE==0)
  {
    tgt=((uint64_t)lFDev)<<22;
    (*pcM)=(uint32_t)(tgt/s_lXtalFrequency);
    tgt1=(uint64_t)s_lXtalFrequency*(*pcM);
    tgt2=(uint64_t)s_lXtalFrequency*((*pcM)+1);
  }
  else
  {
    tgt=((uint64_t)lFDev)<<(23-uFDevE);
    (*pcM)=(uint32_t)(tgt/s_lXtalFrequency)-256;
    tgt1=(uint64_t)s_lXtalFrequency*((*pcM)+256);
    tgt2=(uint64_t)s_lXtalFrequency*((*pcM)+1+256);
  }

  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);
}

uint8_t S2LPRadioInit(SRadioInit* pxSRadioInitStruct) {
  uint8_t tmpBuffer[6], tmp8, dr_e, fdev_m, fdev_e, bw_m, bw_e;
  uint16_t dr_m;
  SFunctionalState xState;

  s_assert_param(IS_FREQUENCY_BAND(pxSRadioInitStruct->lFrequencyBase));
  s_assert_param(IS_MODULATION(pxSRadioInitStruct->xModulationSelect));
  s_assert_param(IS_DATARATE(pxSRadioInitStruct->lDatarate,s_lXtalFrequency));
  s_assert_param(IS_F_DEV(pxSRadioInitStruct->lFreqDev,s_lXtalFrequency));

  /* Configure the digital, ADC, SMPS reference clock divider */
  xState = S2LPRadioGetDigDiv();
  if(((s_lXtalFrequency<DIG_DOMAIN_XTAL_THRESH) && (xState==S_ENABLE)) || ((s_lXtalFrequency>DIG_DOMAIN_XTAL_THRESH) && (xState==S_DISABLE))) {
    S2LPSpiCommandStrobes(CMD_STANDBY);
    do{
      for(volatile uint8_t i=0; i!=0xFF; i++);
      S2LPRefreshStatus();      // add a timer expiration callback
    }while(g_xStatus.MC_STATE!=MC_STATE_STANDBY);

    xState = (SFunctionalState)!xState;
    S2LPRadioSetDigDiv(xState);

    S2LPSpiCommandStrobes(CMD_READY);
    do{
      for(volatile uint8_t i=0; i!=0xFF; i++);
      S2LPRefreshStatus();      // add a timer expiration callback
    }while(g_xStatus.MC_STATE!=MC_STATE_READY);
  }

  if(xState==S_ENABLE) {
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,(s_lXtalFrequency>>1)));
  }
  else {
    s_assert_param(IS_CH_BW(pxSRadioInitStruct->lBandwidth,s_lXtalFrequency));
  }

  /* Intermediate Frequency setting */
  S2LPRadioComputeIF(300000, &tmpBuffer[0], &tmpBuffer[1]);
  S2LPSpiWriteRegisters(IF_OFFSET_ANA_ADDR, 2, tmpBuffer);

  /* Calculates the datarate register values */
  S2LPRadioSearchDatarateME(pxSRadioInitStruct->lDatarate, &dr_m, &dr_e);
  tmpBuffer[0] = (uint8_t)(dr_m>>8);
  tmpBuffer[1] = (uint8_t)dr_m;
  tmpBuffer[2] = (uint8_t)(pxSRadioInitStruct->xModulationSelect | dr_e);



  /* Calculates the frequency deviation register values */
  S2LPRadioSearchFreqDevME(pxSRadioInitStruct->lFreqDev, &fdev_m, &fdev_e);
  S2LPSpiReadRegisters(MOD1_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~FDEV_E_REGMASK;
  tmpBuffer[3] |= fdev_e;
  tmpBuffer[4] = fdev_m;

  /* Calculates the channel filter register values */
  S2LPRadioSearchChannelBwME(pxSRadioInitStruct->lBandwidth, &bw_m, &bw_e);
  tmpBuffer[5] = (bw_m<<4) | bw_e;

  /* Configures the radio registers */
  S2LPSpiWriteRegisters(MOD4_ADDR, 6, tmpBuffer);

  S2LPSpiReadRegisters(PA_POWER0_ADDR, 3, &tmpBuffer[0]);

  /* if OOK is selected enable the PA_FC else enable it */
  if((pxSRadioInitStruct->xModulationSelect)!=MOD_ASK_OOK)
  {
    tmpBuffer[0] &= 0x7F;
    tmpBuffer[1] &= 0xFD;
  }
  else
  {
    tmpBuffer[0] |= 0x80;
    tmpBuffer[1] |= 0x02;
  }


  tmpBuffer[2]&=0xFC;

  /* Bessel filter config */
  if(pxSRadioInitStruct->lDatarate<16000)
  {
    tmpBuffer[2]|=0x00;
  }
  else if(pxSRadioInitStruct->lDatarate<32000)
  {
    tmpBuffer[2]|=0x01;
  }
  else if(pxSRadioInitStruct->lDatarate<62500)
  {
    tmpBuffer[2]|=0x02;
  }
  else
  {
    tmpBuffer[2]|=0x03;
  }
  S2LPSpiWriteRegisters(PA_POWER0_ADDR, 3, &tmpBuffer[0]);

  /* Enable the freeze option of the AFC on the SYNC word */
  S2LPSpiReadRegisters(AFC2_ADDR, 1, &tmp8);
  tmp8 |= AFC_FREEZE_ON_SYNC_REGMASK; S2LPSpiWriteRegisters(AFC2_ADDR, 1, &tmp8);

  return S2LPRadioSetFrequencyBase(pxSRadioInitStruct->lFrequencyBase);

}

uint32_t S2LPRadioComputeSynthWord(uint32_t frequency, uint8_t refdiv) {
  uint8_t band;

  if(IS_FREQUENCY_BAND_HIGH(frequency)) {
    band = HIGH_BAND_FACTOR;
  }
  else {
    band = MIDDLE_BAND_FACTOR;
  }

  uint64_t tgt1,tgt2,tgt;
  uint32_t synth;

  tgt = (((uint64_t)frequency)<<19)*(band*refdiv);
  synth=(uint32_t)(tgt/s_lXtalFrequency);
  tgt1 = (uint64_t)s_lXtalFrequency*(synth);
  tgt2 = (uint64_t)s_lXtalFrequency*(synth+1);

  synth=((tgt2-tgt)<(tgt-tgt1))?(synth+1):(synth);

  return synth;
}

SFunctionalState S2LPRadioGetRefDiv(void) {
  uint8_t tmp;

  g_xStatus = S2LPSpiReadRegisters(XO_RCO_CONF0_ADDR, 1, &tmp);

  if(tmp & REFDIV_REGMASK) {
    return S_ENABLE;
  } else {
    return S_DISABLE;
  }
}

void S2LPRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE) {
  int8_t i, i_tmp;
  uint32_t f_dig=s_lXtalFrequency;
  int32_t chfltCalculation[3];


  if(f_dig>DIG_DOMAIN_XTAL_THRESH) {
    f_dig >>= 1;
  }

  s_assert_param(IS_CH_BW(lBandwidth,f_dig));

  /* Search the channel filter bandwidth table index */
  for(i=0;i<90 && (lBandwidth<(uint32_t)(((uint64_t)s_vectnBandwidth26M[i]*f_dig)/260000));i++);

  if(i!=0) {
    /* Finds the index value with best approximation in i-1, i and i+1 elements */
    i_tmp = i;

    for(uint8_t j=0;j<3;j++) {
      if(((i_tmp+j-1)>=0) && ((i_tmp+j-1)<=89)) {
        chfltCalculation[j] = (int32_t)lBandwidth - (int32_t)(((uint64_t)s_vectnBandwidth26M[i_tmp+j-1]*f_dig)/260000);
      }
      else {
        chfltCalculation[j] = 0x7FFFFFFF;
      }
    }
    uint32_t chfltDelta = 0xFFFFFFFF;

    for(uint8_t j=0;j<3;j++) {
      if(S_ABS(chfltCalculation[j])<chfltDelta) {
        chfltDelta = S_ABS(chfltCalculation[j]);
        i=i_tmp+j-1;
      }
    }
  }
  (*pcE) = (uint8_t)(i/9);
  (*pcM) = (uint8_t)(i%9);

}

void S2LPRadioSearchWCP(uint8_t* cp_isel, uint8_t* pfd_split, uint32_t lFc, uint8_t refdiv) {
  uint32_t vcofreq, lFRef;
  uint8_t BFactor = MIDDLE_BAND_FACTOR;

  s_assert_param(IS_FREQUENCY_BAND(lFc));

  /* Search the operating band */
  if(IS_FREQUENCY_BAND_HIGH(lFc)) {
    BFactor = HIGH_BAND_FACTOR;
  }

  /* Calculates the VCO frequency VCOFreq = lFc*B */
  vcofreq = lFc*BFactor;

  /* Calculated the reference frequency clock */
  lFRef = s_lXtalFrequency/refdiv;

  /* Set the correct charge pump word */
  if(vcofreq>=VCO_CENTER_FREQ) {
    if(lFRef>DIG_DOMAIN_XTAL_THRESH) {
      *cp_isel = 0x02;
      *pfd_split = 0;
    }
    else {
      *cp_isel = 0x01;
      *pfd_split = 1;
    }
  }
  else {
    if(lFRef>DIG_DOMAIN_XTAL_THRESH) {
      *cp_isel = 0x03;
      *pfd_split = 0;
    }
    else {
      *cp_isel = 0x02;
      *pfd_split = 1;
    }
  }

}

uint8_t S2LPRadioSetFrequencyBase(uint32_t lFBase) {
  uint32_t tmp32;
  uint8_t tmpBuffer[4], cp_isel, bs = 1, pfd_split, tmp, cRefDiv;

  s_assert_param(IS_FREQUENCY_BAND(lFBase));

  tmp32 = S2LPRadioComputeSynthWord(lFBase, ((uint8_t)S2LPRadioGetRefDiv()+1));

  if(IS_FREQUENCY_BAND_HIGH(lFBase)) {
    bs = 0;
  }

  cRefDiv = (uint8_t)S2LPRadioGetRefDiv() + 1;

  /* Search the VCO charge pump word and set the corresponding register */
  S2LPRadioSearchWCP(&cp_isel, &pfd_split, lFBase, cRefDiv);

  S2LPSpiReadRegisters(SYNTH_CONFIG2_ADDR, 1, &tmp);
  tmp &= ~PLL_PFD_SPLIT_EN_REGMASK;
  tmp |= (pfd_split<<2);
  S2LPSpiWriteRegisters(SYNTH_CONFIG2_ADDR, 1, &tmp);

  /* Build the array of registers values for the analog part */
  tmpBuffer[0] = (((uint8_t)(tmp32>>24)) & SYNT_27_24_REGMASK) | cp_isel<<5 | (bs<<4) ;
  tmpBuffer[1] = (uint8_t)(tmp32>>16);
  tmpBuffer[2] = (uint8_t)(tmp32>>8);
  tmpBuffer[3] = (uint8_t)tmp32;

  g_xStatus = S2LPSpiWriteRegisters(SYNT3_ADDR, 4, tmpBuffer);

  return 0;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
