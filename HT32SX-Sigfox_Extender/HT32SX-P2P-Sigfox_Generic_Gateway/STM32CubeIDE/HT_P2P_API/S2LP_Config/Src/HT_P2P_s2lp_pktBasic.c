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

#include "spi.h"
#include "HT_P2P_s2lp_pktBasic.h"

void S2LPPktBasicInit(PktBasicInit* pxPktBasicInit) {
  uint8_t tmpBuffer[6];

  /* Check the parameters */
  s_assert_param(IS_BASIC_PREAMBLE_LENGTH(pxPktBasicInit->xPreambleLength));
  s_assert_param(IS_BASIC_SYNC_LENGTH(pxPktBasicInit->xSyncLength));
  s_assert_param(IS_BASIC_CRC_MODE(pxPktBasicInit->xCrcMode));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->cExtendedPktLenField));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xFixVarLength));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xAddressField));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xFec));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicInit->xDataWhitening));

  //S2LPPktWMbusSetSubmode(0); //WMBUS NT CONFIGURED

  /* Always set the automatic packet filtering */
  S2LPSpiReadRegisters(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);
  tmpBuffer[0] |= AUTO_PCKT_FLT_REGMASK;
  S2LPSpiWriteRegisters(PROTOCOL1_ADDR, 1, &tmpBuffer[0]);

  tmpBuffer[0] = ((pxPktBasicInit->xSyncLength)<<2) | (uint8_t)((pxPktBasicInit->xPreambleLength)>>8);
  tmpBuffer[1] = (uint8_t)(pxPktBasicInit->xPreambleLength);
  tmpBuffer[2] = (((uint8_t)pxPktBasicInit->xAddressField)<<3);

  if((pxPktBasicInit->cExtendedPktLenField)==S_ENABLE)
  {
    tmpBuffer[2]|=0x80;
  }

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmpBuffer[3]);
  tmpBuffer[3] &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmpBuffer[3] |= PKT_FORMAT_BASIC_CODE;

  S2LPSpiReadRegisters(PCKTCTRL2_ADDR, 2, &tmpBuffer[4]);

  if(pxPktBasicInit->xFixVarLength == S_ENABLE) {
     tmpBuffer[4] |= FIX_VAR_LEN_REGMASK;
  }
  else {
     tmpBuffer[4] &= ~FIX_VAR_LEN_REGMASK;
  }
  tmpBuffer[4] &= ~(MANCHESTER_EN_REGMASK | MBUS_3OF6_EN_REGMASK);

  tmpBuffer[5] &= ~(CRC_MODE_REGMASK | TXSOURCE_REGMASK);
  tmpBuffer[5] |= (uint8_t)pxPktBasicInit->xCrcMode;

  if(pxPktBasicInit->xDataWhitening == S_ENABLE) {
     tmpBuffer[5] |= WHIT_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~WHIT_EN_REGMASK;
  }

  if(pxPktBasicInit->xFec == S_ENABLE)
  {
     tmpBuffer[5] |= FEC_EN_REGMASK;
  }
  else {
     tmpBuffer[5] &= ~FEC_EN_REGMASK;
  }

  S2LPSpiWriteRegisters(PCKTCTRL6_ADDR, 6, tmpBuffer);

  /* SYNC word */
  for(uint8_t i=0 ; i<4 ; i++) {
    tmpBuffer[i] = (uint8_t)(pxPktBasicInit->lSyncWords>>(8*i));
  }
  g_xStatus = S2LPSpiWriteRegisters(SYNC3_ADDR, 4, tmpBuffer);

  /* Sets CRC check bit */
  if(pxPktBasicInit->xCrcMode == PKT_NO_CRC) {
    S2LPPktBasicFilterOnCrc(S_DISABLE);
  }
  else {
    S2LPPktBasicFilterOnCrc(S_ENABLE);
  }

    /* Constellation map setting */
  S2LPSpiReadRegisters(MOD1_ADDR, 1, tmpBuffer);
  tmpBuffer[0] &= ~G4FSK_CONST_MAP_REGMASK;
  S2LPSpiWriteRegisters(MOD1_ADDR, 1, tmpBuffer);
}

void S2LPPktBasicGetInfo(PktBasicInit* pxPktBasicInit)
{
  uint8_t tmpBuffer[6];

  S2LPSpiReadRegisters(PCKTCTRL6_ADDR, 6, tmpBuffer);

  /* Sync length */
  pxPktBasicInit->xSyncLength = ((tmpBuffer[0] & SYNC_LEN_REGMASK)>>2);

  /* Preamble length */
  pxPktBasicInit->xPreambleLength = (((uint16_t)(tmpBuffer[0] & PREAMBLE_LEN_9_8_REGMASK))<<8) | ((uint16_t)tmpBuffer[1]);

  /* Length width */
  pxPktBasicInit->cExtendedPktLenField = (SFunctionalState)((tmpBuffer[2] & LEN_WID_REGMASK)>>7);

  /* Address field */
  pxPktBasicInit->xAddressField = (SFunctionalState)((tmpBuffer[2] & ADDRESS_LEN_REGMASK)>>3);

  /* FIX or VAR bit */
  pxPktBasicInit->xFixVarLength = (SFunctionalState)(tmpBuffer[4] & FIX_VAR_LEN_REGMASK);

  /* CRC mode */
  pxPktBasicInit->xCrcMode = (BasicCrcMode)(tmpBuffer[5] & CRC_MODE_REGMASK);

  /* Whitening */
  pxPktBasicInit->xDataWhitening = (SFunctionalState)((tmpBuffer[5] & WHIT_EN_REGMASK)>> 4);

  /* FEC */
  pxPktBasicInit->xFec = (SFunctionalState)(tmpBuffer[5] & FEC_EN_REGMASK);

  g_xStatus = S2LPSpiReadRegisters(SYNC3_ADDR, 4, tmpBuffer);

  /* SYNC word */
  pxPktBasicInit->lSyncWords = 0;
  for(uint8_t i=0 ; i<4 ; i++) {
    pxPktBasicInit->lSyncWords |= ((uint32_t)tmpBuffer[i])<<(8*i);
  }
}

void S2LPPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses)
{
  uint8_t tmpBuffer[3];
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnMyAddress));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnMulticastAddress));
  s_assert_param(IS_SFUNCTIONAL_STATE(pxPktBasicAddresses->xFilterOnBroadcastAddress));

  /* Reads the PCKT_FLT_OPTIONS ragister */
  S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Enables or disables filtering on my address */
  if(pxPktBasicAddresses->xFilterOnMyAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_SOURCE_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_SOURCE_ADDR_REGMASK;
  }

  /* Enables or disables filtering on multicast address */
  if(pxPktBasicAddresses->xFilterOnMulticastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_MULTICAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_MULTICAST_ADDR_REGMASK;
  }

  /* Enables or disables filtering on broadcast address */
  if(pxPktBasicAddresses->xFilterOnBroadcastAddress == S_ENABLE) {
    tmpBuffer[0] |= DEST_VS_BROADCAST_ADDR_REGMASK;
  }
  else {
    tmpBuffer[0] &= ~DEST_VS_BROADCAST_ADDR_REGMASK;
  }

  S2LPSpiWriteRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);

  /* Fills the array with the addresses passed in the structure */
#if 0 //indar2
  tmpBuffer[0] = pxPktBasicAddresses->cBroadcastAddress;
  tmpBuffer[1] = pxPktBasicAddresses->cMulticastAddress;
  tmpBuffer[2] = pxPktBasicAddresses->cMyAddress;
#else
  tmpBuffer[0] = pxPktBasicAddresses->cMyAddress;
  tmpBuffer[1] = pxPktBasicAddresses->cMulticastAddress;
  tmpBuffer[2] = pxPktBasicAddresses->cBroadcastAddress;
#endif
  g_xStatus = S2LPSpiWriteRegisters(PCKT_FLT_GOALS2_ADDR, 3, tmpBuffer);
}

void S2LPPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses) {
  uint8_t tmpBuffer[3];

  S2LPSpiReadRegisters(PCKT_FLT_GOALS3_ADDR, 3, tmpBuffer);
  pxPktBasicAddresses->cMyAddress = tmpBuffer[0];
  pxPktBasicAddresses->cBroadcastAddress = tmpBuffer[1];
  pxPktBasicAddresses->cMulticastAddress = tmpBuffer[2];

  g_xStatus = S2LPSpiReadRegisters(PCKT_FLT_OPTIONS_ADDR, 1, &tmpBuffer[0]);
  pxPktBasicAddresses->xFilterOnBroadcastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_BROADCAST_ADDR_REGMASK) >> 3);
  pxPktBasicAddresses->xFilterOnMulticastAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_MULTICAST_ADDR_REGMASK) >> 2);
  pxPktBasicAddresses->xFilterOnMyAddress = (SFunctionalState)((tmpBuffer[0] & DEST_VS_SOURCE_ADDR_REGMASK) >> 1);
}

void S2LPPktBasicSetFormat(void) {
  uint8_t tmp;

  S2LPSpiReadRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  /* Build the new value. Also set to 0 the direct RX mode bits */
  tmp &= ~(PCKT_FRMT_REGMASK | RX_MODE_REGMASK);
  tmp |= PKT_FORMAT_BASIC_CODE;
  S2LPSpiWriteRegisters(PCKTCTRL3_ADDR, 1, &tmp);

  S2LPSpiReadRegisters(PCKTCTRL1_ADDR, 1, &tmp);

  /* Set to 0 the direct TX mode bits */
  tmp &= ~TXSOURCE_REGMASK;
  g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL1_ADDR, 1, &tmp);

  //S2LPPktWMbusSetSubmode(WMBUS_SUBMODE_NOT_CONFIGURED);
}

void S2LPPktBasicAddressField(SFunctionalState xAddressField) {
  uint8_t tmp;
  s_assert_param(IS_SFUNCTIONAL_STATE(xAddressField));

  S2LPSpiReadRegisters(PCKTCTRL4_ADDR, 1, &tmp);
  if(xAddressField==S_ENABLE) {
    tmp |= ADDRESS_LEN_REGMASK;
  }
  else {
    tmp &= ADDRESS_LEN_REGMASK;
  }
  g_xStatus = S2LPSpiWriteRegisters(PCKTCTRL4_ADDR, 1, &tmp);

}

SFunctionalState S2LPPktBasicGetAddressField(void) {
  uint8_t tmp;

  g_xStatus = S2LPSpiReadRegisters(PCKTCTRL4_ADDR, 1, &tmp);
  if(tmp & ADDRESS_LEN_REGMASK) {
    return S_ENABLE;
  }
  else {
    return S_DISABLE;
  }

}

void S2LPPktBasicSetPayloadLength(uint16_t nPayloadLength) {
  uint8_t tmpBuffer[2];

  if(S2LPPktBasicGetAddressField()) {
    nPayloadLength++;
  }
  tmpBuffer[0] = (uint8_t)(nPayloadLength>>8);
  tmpBuffer[1] = (uint8_t)nPayloadLength;
  g_xStatus = S2LPSpiWriteRegisters(PCKTLEN1_ADDR, 2, tmpBuffer);
}


uint16_t S2LPPktBasicGetPayloadLength(void) {
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  g_xStatus = S2LPSpiReadRegisters(PCKTLEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);

  if(S2LPPktBasicGetAddressField()) {
    nPayloadLength--;
  }
  return nPayloadLength;
}

uint16_t S2LPPktBasicGetReceivedPktLength(void) {
  uint8_t tmpBuffer[2];
  uint16_t nPayloadLength;

  g_xStatus = S2LPSpiReadRegisters(RX_PCKT_LEN1_ADDR, 2, tmpBuffer);
  nPayloadLength = (((uint16_t)tmpBuffer[0])<<8) | ((uint16_t)tmpBuffer[1]);

  if(S2LPPktBasicGetAddressField()) {
    nPayloadLength--;
  }
  return nPayloadLength;
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
