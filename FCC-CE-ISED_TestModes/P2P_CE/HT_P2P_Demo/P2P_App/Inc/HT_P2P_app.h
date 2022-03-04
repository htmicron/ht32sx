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
 * \file HT_P2P_app.h
 * \brief P2P Demo Application for HT32SX device.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */


#ifndef __HT_P2P_APP_H__
#define __HT_P2P_APP_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "HT_CE_App.h"

#include "HT_P2P_s2lp_types.h"
#include "HT_P2P_s2lp_config.h"
#include "HT_P2P_s2lp_pktBasic.h"
#include "HT_P2P_s2lp_gpio.h"
#include "HT_P2P_s2lp_Qi.h"
#include "HT_P2P_s2lp_packetHandler.h"
#include "HT_P2P_s2lp_timer.h"
#include "HT_P2P_FEM.h"
#include "HT_P2P_s2lp_interface.h"
#include "HT_P2P_s2lp_commands.h"

/* Defines  ------------------------------------------------------------------*/

#ifdef BASE_FREQ_433
#define BASE_FREQUENCY              433.0e6 		/**< P2P basic frequency. Change this define to work in another frequency. */
#else
#define BASE_FREQUENCY 				902200000		/**< P2P basic frequency. Change this define to work in another frequency. */
#endif

#define TX_DEVICE 		1

#define CHANNEL_SPACE               100e3
#define CHANNEL_NUMBER              0
#define DATARATE                    38400
#define FREQ_DEVIATION              20e3
#define BANDWIDTH                   100E3
#define POWER_INDEX                 7
#define RECEIVE_TIMEOUT             2000.0 			/** < change the value for required timeout period*/
#define RSSI_THRESHOLD              -120  			/* < Default RSSI at reception, more than noise floor */
#define CSMA_RSSI_THRESHOLD         -90   			/* < Higher RSSI to Transmit. If it's lower, the Channel will be seen as busy */

/*  Packet configuration parameters  */
#define SYNC_WORD                   0x88888888
#define LENGTH_WIDTH                7
#define CRC_MODE                    PKT_CRC_MODE_8BITS
#define EN_FEC                      S_DISABLE
#define EN_WHITENING                S_ENABLE
#define MODULATION_SELECT          	MOD_2FSK
#define POWER_DBM                   12.0
#define PREAMBLE_LENGTH             PREAMBLE_BYTE(4)
#define SYNC_LENGTH                 SYNC_BYTE(4)
#define CONTROL_LENGTH              0x00
#define VARIABLE_LENGTH             S_ENABLE
#define EXTENDED_LENGTH_FIELD       S_DISABLE

#define PREAMBLE_BYTE(v)        	(4*v)
#define SYNC_BYTE(v)           		(8*v)
#define VARIABLE_LENGTH             S_ENABLE

#define EN_ADDRESS                  S_ENABLE
#define EN_FILT_MY_ADDRESS          S_ENABLE
#define EN_FILT_MULTICAST_ADDRESS   S_ENABLE
#define EN_FILT_BROADCAST_ADDRESS   S_ENABLE
#define EN_FILT_SOURCE_ADDRESS      S_ENABLE

#define SOURCE_ADDR_MASK            0xf0
#define SOURCE_ADDR_REF             0x37
#define MULTICAST_ADDRESS           0xEE
#define BROADCAST_ADDRESS           0xFF

#ifdef TX_DEVICE

#define MY_ADDRESS                  0x44 /* < This device address */
#define DESTINATION_ADDRESS         0x45 /* < Change this to send messages to another device */

#else

#define MY_ADDRESS                  0x45 /* < This device address */
#define DESTINATION_ADDRESS         0x44 /* < Change this to send messages to another device */

#endif


#define EN_AUTOACK                      S_DISABLE
#define EN_PIGGYBACKING             	S_DISABLE
#define MAX_RETRANSMISSIONS         	PKT_DISABLE_RETX

#define PAYLOAD_LEN                     25 					/* < *20 bytes data+tag+cmd_type+cmd+cmdlen+datalen*/
#define APPLI_CMD                       0x11
#define NWK_CMD                         0x22
#define LED_TOGGLE                      0xff
#define ACK_OK                          0x01
#define MAX_BUFFER_LEN                  96
#define TIME_TO_EXIT_RX                 3000
#define DELAY_RX_LED_TOGGLE             100
#define DELAY_TX_LED_GLOW               200
#define LPM_WAKEUP_TIME                 100
#define DATA_SEND_TIME                  50

#define TX_BUFFER_SIZE   				20
#define RX_BUFFER_SIZE   				96

#define TIME_UP                         0x01

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_P2P_Init(void)
 * \brief Init S2LP, external PA and configures S2LP to use basic protocol.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_P2P_Init(void);

/*!******************************************************************
 * \fn void BasicProtocolInit(void)
 * \brief Set up S2LP basic protocol.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void BasicProtocolInit(void);

/*!******************************************************************
 * \fn void AppliReceiveBuff(void)
 * \brief RX routine. Sets up S2LP registers and PA to RX mode.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void AppliReceiveBuff(void);

/*!******************************************************************
 * \fn void AppliSendBuff(AppliFrame_t *xTxFrame, uint8_t cTxlen)
 * \brief TX routine. Send a buffer and sets up S2LP register and PA to TX mode.
 *
 * \param[in] AppliFrame_t *xTxFrame 					TX Packet.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void AppliSendBuff(HT_Payload *xTxFrame);

/*!******************************************************************
 * \fn void P2P_Process(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen)
 * \brief P2P finite state machine.
 *
 * \param[in] uint8_t *pTxBuff 							TX packet.
 * \param[in] uint8_t cTxlen							TX buffer length.
 * \param[in] uint8_t* pRxBuff							RX packet.
 * \param[in] uint8_t cRxlen							RX buffer length
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2P_Process(uint8_t *pTxBuff, uint8_t cTxlen, uint8_t* pRxBuff, uint8_t cRxlen);

/*!******************************************************************
 * \fn void HAL_Radio_Init(void)
 * \brief Restart S2LP radio.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HAL_Radio_Init(void);

/*!******************************************************************
 * \fn void P2PInterruptHandler(void)
 * \brief Interruption handler routine. In this application it is used to manage the S2LP IRQ
 * 			configured to be notified on the S2LP GPIO_3.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2PInterruptHandler(void);

/*!******************************************************************
 * \fn void Set_KeyStatus(FlagStatus val)
 * \brief Updates FSM after button pressed.
 *
 * \param[in] FlagStatus val						Flag status.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void Set_KeyStatus(FlagStatus val);

#endif /* __HT_P2P_APP_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
