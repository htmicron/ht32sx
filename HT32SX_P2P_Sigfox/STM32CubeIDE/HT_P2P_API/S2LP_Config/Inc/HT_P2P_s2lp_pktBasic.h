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
 * \file HT_P2P_s2lp_pktBasic.h
 * \brief Configuration and management of S2-LP Basic packets.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_PKTBASIC_H__
#define __HT_P2P_S2LP_PKTBASIC_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "HT_P2P_s2lp_packetHandler.h"
#include "HT_P2P_s2lp_types.h"
#include "HT_P2P_s2lp_packetHandler.h"

/* Defines ------------------------------------------------------------------*/

#define PKT_FORMAT_BASIC_CODE                   0x00
#define IS_BASIC_PREAMBLE_LENGTH           IS_PREAMBLE_LEN
#define IS_BASIC_SYNC_LENGTH               IS_SYNC_LEN
#define IS_BASIC_PKT_LEN_FIELD_WID         IS_PKT_LEN_FIELD_WID
#define IS_BASIC_CRC_MODE                  IS_PKT_CRC_MODE

/**
 * @brief  Sets the PREAMBLE field length for S2LP Basic packets.
 * @param  xPreambleLength length of PREAMBLE field in bytes.
 *         This parameter can be any value of @ref BasicPreambleLength.
 * @retval None.
 */
#define S2LPPktBasicSetPreambleLength(xPreambleLength)                S2LPSetPreambleLength(xPreambleLength)

/**
 * @brief  Returns the PREAMBLE field length mode for S2LP Basic packets.
 * @param  None.
 * @retval uint8_t Preamble field length in bytes.
 */
#define S2LPPktBasicGetPreambleLength()                               S2LPGetPreambleLength()

/**
 * @brief  Sets the SYNC field length for S2LP Basic packets.
 * @param  xSyncLength length of SYNC field in bytes.
 *         This parameter can be any value of @ref BasicSyncLength.
 * @retval None.
 */
#define S2LPPktBasicSetSyncLength(xSyncLength)                        S2LPSetSyncLength((PktSyncLength)xSyncLength)

/**
 * @brief  Returns the SYNC field length for S2LP Basic packets.
 * @param  None.
 * @retval uint8_t SYNC field length in bytes.
 */
#define S2LPPktBasicGetSyncLength()                                   S2LPGetSyncLength()

/**
 * @brief  Enables or Disables the CRC filtering.
 * @param  xNewState new state for CRC_CHECK.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define S2LPPktBasicFilterOnCrc(xNewState)                            S2LPPktCommonFilterOnCrc(xNewState)

/**
 * @brief  Enables or Disables WHITENING for S2LP packets.
 * @param  xNewState new state for WHITENING mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define S2LPPktBasicWhitening(xNewState)                               S2LPWhitening(xNewState)

/**
 * @brief  Enables or Disables FEC for S2LP Basic packets.
 * @param  xNewState new state for FEC mode.
 *         This parameter can be S_ENABLE or S_DISABLE.
 * @retval None.
 */
#define S2LPPktBasicFec(xNewState)                                     S2LPFec(xNewState)

/**
 * @brief  Sets multiple SYNC words for S2LP Basic packets.
 * @param  lSyncWords SYNC words to be set with format: 0x|SYNC1|SYNC2|SYNC3|SYNC4|.
 *         This parameter is a uint32_t.
 * @param  xSyncLength SYNC length in bytes. The 32bit word passed will be stored in the SYNCx registers from the MSb
 *         until the number of bytes in xSyncLength has been stored.
 *         This parameter is a @ref BasicSyncLength.
 * @retval None.
 */
#define S2LPPktBasicSetSyncWords(lSyncWords, xSyncLength)              S2LPSetSyncWords(lSyncWords, xSyncLength)

/* Typedef ------------------------------------------------------------------*/

/**
 * \enum BasicCrcMode
 * \brief CRC length in bytes enumeration.
 */
typedef PktCrcMode BasicCrcMode;

/**
 * \struct PktBasicInit
 * \brief S2LP Basic Packet Init structure definition.
 */
typedef struct {
  uint16_t           xPreambleLength;         /*!< Set the preamble length of packet. From 1 to 1024 chip sequence. */
  uint8_t            xSyncLength;             /*!< Set the sync word length of packet in bits. From 1 to 64 bits. */
  uint32_t           lSyncWords;              /*!< Set the sync words in MSB. */
  SFunctionalState   xFixVarLength;           /*!< Enable the variable length mode. */
  SFunctionalState   cExtendedPktLenField;    /*!< Extend the length field from 1 byte to 2 bytes. Variable length mode only. */
  BasicCrcMode       xCrcMode;                /*!< Set the CRC type. @ref StackCrcMode */
  SFunctionalState   xAddressField;           /*!< Enable the destination address field. */
  SFunctionalState   xFec;                    /*!< Enable the FEC/Viterbi. */
  SFunctionalState   xDataWhitening;          /*!< Enable the data whitening. */
} PktBasicInit;


/**
 * \struct PktBasicAddressesInit
 * \brief S2LP Basic packet address structure definition. This structure allows users to specify
 *         the node/multicast/broadcast addresses and the correspondent filtering options.
 */
typedef struct {
  SFunctionalState   xFilterOnMyAddress;         /*!< If set packet is received if its destination address matches with cMyAddress. */
  uint8_t            cMyAddress;                 /*!< Set the MyAddress. */
  SFunctionalState   xFilterOnMulticastAddress;  /*!< If set packet is received if its destination address matches with cMulticastAddress. */
  uint8_t            cMulticastAddress;          /*!< Set the Multicast address */
  SFunctionalState   xFilterOnBroadcastAddress;  /*!< If set packet is received if its destination address matches with cBroadcastAddress. */
  uint8_t            cBroadcastAddress;          /*!< Set the Broadcast address */
} PktBasicAddressesInit;

/*!******************************************************************
 * \fn void S2LPPktBasicInit(PktBasicInit* pxPktBasicInit)
 * \brief Initialize the S2LP Basic packet according to the specified parameters in the PktBasicInit struct.
 *         Notice that this function sets the autofiltering option on CRC if it is set to any value different from BASIC_NO_CRC.
 *
 * \param[in] PktBasicInit* pxPktBasicInit						Basic packet init structure.
 *         														This parameter is a pointer to @ref PktBasicInit.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicInit(PktBasicInit* pxPktBasicInit);

/*!******************************************************************
 * \fn void S2LPPktBasicGetInfo(PktBasicInit* pxPktBasicInit)
 * \brief Return the S2LP Basic packet structure according to the specified parameters in the registers.
 *
 * \param[out] PktBasicInit* pxPktBasicInit						Pointer returning basic packet init structure.
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicGetInfo(PktBasicInit* pxPktBasicInit);

/*!******************************************************************
 * \fn void S2LPPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses)
 * \brief Initialize the S2LP Basic packet addresses according to the specified
 * 			parameters in the PktBasicAddressesInit struct.
 *
 * \param[in] PktBasicAddressesInit* pxPktBasicAddresses		Basic packet addresses init structure.
 *        														This parameter is a pointer to @ref PktBasicAddresses.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicAddressesInit(PktBasicAddressesInit* pxPktBasicAddresses);

/*!******************************************************************
 * \fn void S2LPPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses)
 * \brief Return the S2LP Basic packet addresses structure according to the specified
 *         parameters in the registers.
 *
 * \param[out] PktBasicAddressesInit* pxPktBasicAddresses 		Basic packet addresses init structure.
 *         														This parameter is a pointer to @ref PktBasicAddresses.
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicGetAddressesInfo(PktBasicAddressesInit* pxPktBasicAddresses);

/*!******************************************************************
 * \fn void S2LPPktBasicSetFormat(void)
 * \brief Configure the Basic packet format as packet used by S2LP.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicSetFormat(void);

/*!******************************************************************
 * \fn void S2LPPktBasicAddressField(SFunctionalState xAddressField)
 * \brief Set the address length for S2LP Basic packets.
 *
 * \param[in] SFunctionalState xAddressField					Length of ADDRESS in bytes.
 * 																This parameter can be: S_ENABLE or S_DISABLE.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicAddressField(SFunctionalState xAddressField);

/*!******************************************************************
 * \fn SFunctionalState S2LPPktBasicGetAddressField(void)
 * \brief Specify if the Address field for S2LP Basic packets is enabled or disabled.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval SFunctionalState 									Notifies if the address field is enabled or disabled.
 *******************************************************************/
SFunctionalState S2LPPktBasicGetAddressField(void);

/*!******************************************************************
 * \fn void S2LPPktBasicSetPayloadLength(uint16_t nPayloadLength)
 * \brief Set the payload length for S2LP Basic packets. Since the packet length
 *         depends from the address and the control field size, this
 *         function reads the correspondent registers in order to determine
 *         the correct packet length to be written.
 *
 * \param[in] uint16_t nPayloadLength							 Payload length in bytes.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPPktBasicSetPayloadLength(uint16_t nPayloadLength);

/*!******************************************************************
 * \fn uint16_t S2LPPktBasicGetPayloadLength(void)
 * \brief Return the payload length for S2LP Basic packets. Since the
 *         packet length depends from the address and the control
 *         field size, this function reads the correspondent
 *         registers in order to determine the correct payload length
 *         to be returned.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval uint16_t 											Payload length in bytes.
 *******************************************************************/
uint16_t S2LPPktBasicGetPayloadLength(void);

/*!******************************************************************
 * \fn uint16_t S2LPPktBasicGetReceivedPktLength(void)
 * \brief Return the packet length field of the received packet.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval uint16_t 											Packet length.
 *******************************************************************/
uint16_t S2LPPktBasicGetReceivedPktLength(void);

#endif /* __HT_P2P_S2LP_PKTBASIC_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/

