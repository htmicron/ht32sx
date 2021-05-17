/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 */

/*!
 * \file HT_P2P_s2lp_packetHandler.h
 * \brief Configuration and management of the common features of S2-LP packets.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_PACKETHANDLER_H__
#define __HT_P2P_S2LP_PACKETHANDLER_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "HT_P2P_s2lp_types.h"

/* Typedef ------------------------------------------------------------------*/

/**
 * \enum PktCrcMode
 * \brief CRC length in bytes enumeration.
 */
typedef enum {
  PKT_NO_CRC               = 0x00, /*!< No CRC                              */
  PKT_CRC_MODE_8BITS       = 0x20, /*!< CRC length 8 bits  - poly: 0x07     */
  PKT_CRC_MODE_16BITS_1    = 0x40, /*!< CRC length 16 bits - poly: 0x8005   */
  PKT_CRC_MODE_16BITS_2    = 0x60, /*!< CRC length 16 bits - poly: 0x1021   */
  PKT_CRC_MODE_24BITS      = 0x80, /*!< CRC length 24 bits - poly: 0x864CFB */
  PKT_CRC_MODE_32BITS      = 0xA0, /*!< CRC length 32 bits - poly: 0x04C011BB7 */
} PktCrcMode;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void S2LPSetRxSourceReferenceAddress(uint8_t address);
 * \brief Set the SOURCE_REFERENCEK.
 *
 * \param[in] uint8_t address						Source address to be used as a reference .
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPSetRxSourceReferenceAddress(uint8_t address);

/*!******************************************************************
 * \fn uint8_t S2LPGetReceivedDestinationAddress(void)
 * \brief Get the received destination address.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval uint8_t 									The last received destination address.
 *******************************************************************/
uint8_t S2LPGetReceivedDestinationAddress(void);

/*!******************************************************************
 * \fn void S2LPInterfaceInit(void)
 * \brief Enable or Disable the filtering on CRC.
 *
 * \param[in] SFunctionalState xNewState			New state for CRC_CHECK. This parameter can be S_ENABLE or S_DISABLE.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPPktCommonFilterOnCrc(SFunctionalState xNewState);

#endif /* __HT_P2P_S2LP_PACKETHANDLER_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
