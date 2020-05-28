/**
 * @file    S2LP_EVAL_Utils.c
 * @author  LowPower RF BU - AMG
 * @version 3.3.0
 * @date    Oct 1, 2018
 * @brief   Identification functions for S2LP DK.
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
 */


/* Includes ------------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_AUX_UTILS.h"

/* In order to preserve compatibility with the BlueNRG-1/2 lib */
#define EepromSpiInitialization() EepromSpiInitialization(0)

/**
* @addtogroup S2LP_DK                   S2LP DK
* @{
*/


/**
* @brief This flag is used to synchronize the TIM3 ISR with the XtalMeasurement routine.
*/
static volatile uint8_t s_RfModuleBand = 0, s_RfRangeExtender = 0, s_Tcxo=0;
static volatile uint32_t s_RfXtalFrequency=50000000;
static volatile S2LPDaughterBoard _daughterBoard = FKI_SERIES;

/**
* @defgroup SDK_S2LP_MANAGEMENT_FUNCTIONS    SDK S2LP Management Functions
* @{
*/

/**
* @defgroup IDENTIFICATION_FUNCTIONS      SDK S2LP Management Identification Functions
* @{
*/
void S2LPRadioSetXtalFrequency(uint32_t lXtalFrequency);

uint8_t S2LPManagementGetRangeExtender(void)
{
  return s_RfRangeExtender;
}

void S2LPManagementSetRangeExtender(uint8_t value)
{
  s_RfRangeExtender = value;
}

void S2LPManagementSetBand(uint8_t band)
{
  s_RfModuleBand = band;
}

uint32_t S2LPGetFrequencyBand(void)
{
  uint32_t frequency = 0;
  const uint32_t band_frequencies[] = {
    169000000,
    315000000,
    433000000,
    868000000,
    915000000,
    450000000
  };

  if (s_RfModuleBand < (sizeof(band_frequencies)/sizeof(uint32_t))) {
    frequency = band_frequencies[s_RfModuleBand];
  }

  return frequency;
}

uint8_t S2LPManagementGetBand(void)
{
  return s_RfModuleBand;
}

uint32_t S2LPManagementComputeXtalFrequency(void)
{
  s_RfXtalFrequency=50000000;
  return s_RfXtalFrequency;
}

uint32_t S2LPManagementGetXtalFrequency(void)
{
  return s_RfXtalFrequency;
}

uint8_t S2LPManagementGetTcxo(void)
{
  return s_Tcxo;
}

__weak void S2LPRadioSetXtalFrequency(uint32_t xtal)
{
  s_RfXtalFrequency = xtal;
}

void S2LPManagementIdentificationRFBoard(void)
{
#if EEPROM_PRESENT == EEPROM_YES
  uint8_t was_in_sdn=1;
  int32_t xtal_comp_value;

  if(S2LPShutdownCheck()==RESET)
  {
    /* if reset it was not in SDN */
    S2LPShutdownEnter();
    was_in_sdn=0;
  }

  EepromIdentification();

  //read the memory and set the variable
  uint8_t tmpBuffer[32];
  EepromRead(0x0000, 32, tmpBuffer);

  float foffset=0;
  if(tmpBuffer[0]==0 || tmpBuffer[0]==0xFF) {
    S2LPManagementComputeXtalFrequency();
    return;
  }
  switch(tmpBuffer[1]) {
  case 0:
    s_RfXtalFrequency = 24000000;
    break;
  case 1:
    s_RfXtalFrequency = 25000000;
    break;
  case 2:
    s_RfXtalFrequency = 26000000;
    break;
  case 3:
    s_RfXtalFrequency = 48000000;
    break;
  case 4:
    s_RfXtalFrequency = 50000000;
    break;
  case 5:
    s_RfXtalFrequency = 52000000;
    break;
  default:
    s_RfXtalFrequency=S2LPManagementComputeXtalFrequency();

    break;
  }

  s_RfModuleBand=tmpBuffer[3];

  EepromRead(0x0021,4,tmpBuffer);
  for(uint8_t i=0;i<4;i++)
  {
    ((uint8_t*)&foffset)[i]=tmpBuffer[3-i];
  }

  xtal_comp_value = 0;

  /*foffset is a value measured during manufacturing as follows:
  foffset=fnominal-fmeasured. To compensate such value it should
  be reported to xtal freq and then subtracted*/
  if (foffset != 0xFFFFFFFF) {
    uint32_t frequency = S2LPGetFrequencyBand();

      if (frequency != 0) {
      uint32_t xtal_frequency = s_RfXtalFrequency;

      /* This is the value to be added to the xtal nominal value
      to compensate the xtal offset*/
      xtal_comp_value = (int32_t) ((xtal_frequency*(-foffset))/frequency);
    }
  }

  S2LPRadioSetXtalFrequency(s_RfXtalFrequency+xtal_comp_value);

  S2LPManagementSetRangeExtender(tmpBuffer[5]);

  if(was_in_sdn==0)
  {
    S2LPShutdownExit();
  }
#endif
}

//uint8_t EepromIdentification(void)
//{
//  uint8_t status=0;

//  /*Configure EEPROM SPI with default configuration*/
//  EepromSpiInitialization();

//  /* FKI is default */
//  SdkEvalSetDaughterBoardType(FKI_SERIES);
//  EepromCsPinInitialization();

//  /* Try this procedure for both FKIxxx and XNUCLEO-S2868 boards */
//  for (int i=0; i<2; i++)
//  {
//    /*Switch to new CS configuration only if previoisly conf failed */
//    if (i==1 && status==0)
//    {
//      SdkEvalSetDaughterBoardType(X_NUCLEO_SERIES);
//      EepromCsXnucleoPinInitialization();
//    }

//    /* try to get the status of the EEPROM */
//    status = EepromStatus();
//    if((status&0xF0) == EEPROM_STATUS_SRWD) {
//      /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
//      status=1;
//    }
//    else
//    {
//      EepromWriteEnable();
//      SdkDelayMs(10);
//      /* else the bit may be not set (first time we see this EEPROM), try to set it*/
//      status = EepromSetSrwd();
//      SdkDelayMs(10);
//      /*check again*/
//      status = EepromStatus();

//      if((status&0xF0) == EEPROM_STATUS_SRWD) { // 0xF0 mask [SRWD 0 0 0]
//        /* if it is EEPROM_STATUS_SRWD, ok the EEPROM is present and ready to work */
//        status=1;
//      }
//      else
//      {
//        /* else no EEPROM is present */
//        status = 0;
//      }
//    }
//  }

//  return status;
//}

void SdkEvalSetDaughterBoardType(S2LPDaughterBoard boardType)
{
  _daughterBoard = boardType;
}

S2LPDaughterBoard SdkEvalGetDaughterBoardType()
{
  return _daughterBoard;
}

/**
* @}
*/



/**
* @}
*/


/**
* @}
*/

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
