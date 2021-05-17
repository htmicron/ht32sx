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
 * \file HT_P2P_api.h
 * \brief P2P API for HT32SX.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.3
 * \date March 29, 2021
 */


#ifndef __HT_P2P_API_H__
#define __HT_P2P_API_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "HT_data_repeater.h"

#include "HT_P2P_s2lp_types.h"
#include "HT_P2P_s2lp_config.h"
#include "HT_P2P_s2lp_pktBasic.h"
#include "HT_P2P_s2lp_gpio.h"
#include "HT_P2P_s2lp_Qi.h"
#include "HT_P2P_s2lp_packetHandler.h"
#include "HT_P2P_s2lp_timer.h"
#include "S2LP_AUX_FEM.h"
#include "HT_P2P_s2lp_interface.h"
#include "HT_P2P_s2lp_commands.h"

/* Defines  ------------------------------------------------------------------*/

#ifdef BASE_FREQ_433
#define BASE_FREQUENCY              433.0e6 		/**< P2P basic frequency. Change this define to work in another frequency. */
#else
#define BASE_FREQUENCY 				902200000		/**< P2P basic frequency. Change this define to work in another frequency. */
#endif

#define CHANNEL_SPACE               100e3
#define CHANNEL_NUMBER              0
#define DATARATE                    38400
#define FREQ_DEVIATION              20e3
#define BANDWIDTH                   100E3
#define POWER_INDEX                 7

#define RSSI_THRESHOLD              -120  			/* < Default RSSI at reception, more than noise floor */
#define CSMA_RSSI_THRESHOLD         -90   			/* < Higher RSSI to Transmit. If it's lower, the Channel will be seen as busy */

/*  Packet configuration parameters  */
#define SYNC_WORD                   0x88888888
#define LENGTH_WIDTH                7
#define CRC_MODE                    PKT_CRC_MODE_8BITS
#define EN_FEC                      S_DISABLE
#define EN_WHITENING                S_ENABLE
#define MODULATION_SELECT          	MOD_2FSK

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
#define MULTICAST_ADDRESS           0xFE
#define BROADCAST_ADDRESS           0xFF

#define MY_ADDRESS                  0x01						/*< This device address. */

#define TIME_TO_EXIT_RX                 1000

#define TIME_UP                         0x02

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
 * \fn void HT_P2P_BasicProtocolInit(void)
 * \brief Set up S2LP basic protocol.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_P2P_BasicProtocolInit(void);

/*!******************************************************************
 * \fn void HT_P2P_ReceivePayload(uint8_t src_addr);
 * \brief RX routine. Sets up S2LP registers and PA to RX mode.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_P2P_ReceivePayload(uint8_t src_addr);

/*!******************************************************************
 * \fn void HT_P2P_SendPayload(HT_DR_Payload *xTxFrame, uint8_t dst_addr);
 * \brief TX routine. Send a buffer and sets up S2LP register and PA to TX mode.
 *
 * \param[in] HT_DR_Payload *xTxFrame					TX Packet.
 * \param[in] uint8_t dst_addr							Destination address.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_P2P_SendPayload(HT_DR_Payload *xTxFrame, uint8_t dst_addr);

/*!******************************************************************
 * \fn void HT_P2P_Radio_Init(void)
 * \brief Restart S2LP radio.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_P2P_Radio_Init(void);

/*!******************************************************************
 * \fn void HT_P2P_InterruptHandler(void)
 * \brief Interruption handler routine. In this application it is used to manage the S2LP IRQ
 * 			configured to be notified on the S2LP GPIO_3.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_P2P_InterruptHandler(void);

#endif /* __HT_P2P_API_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
