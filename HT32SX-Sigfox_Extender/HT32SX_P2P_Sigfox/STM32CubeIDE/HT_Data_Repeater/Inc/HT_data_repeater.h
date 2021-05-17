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
 * \file HT_data_repeater.h
 * \brief P2P+Sigfox Stack. Repeats data to a Sigfox gateway.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date March 29, 2021
 */


#ifndef __HT_DATA_REPEATER_H__
#define __HT_DATA_REPEATER_H__

#include "main.h"

/* Defines  ------------------------------------------------------------------*/

#define WAKE_UP_TIME			20 							/**< Wkp time in SECONDS */

#define MAX_N_DEVICES			4							/**< Max numbers of devices in the P2P network. The last one will be the gateway. */
#define FIRST_ADDRESS			(uint8_t)0x01				/**< First device address. */

#define GATEWAY_ID 				(uint8_t)0x04				/**< Gateway device P2P ID. */

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum HT_DR_States
 * \brief FSM states.
 */
typedef enum {
	SM_STATE_START_RX=0, 						/**< Start RX state */
	SM_STATE_WAIT_FOR_RX_DONE,					/**< Wait for RX done state */
	SM_STATE_DATA_RECEIVED,						/**< Data received state */
	SM_STATE_SEND_DATA,							/**< Send data state */
	SM_STATE_WAIT_FOR_TX_DONE,					/**< Wait for RX done state */
	SM_STATE_ACK_RECEIVED,						/**< Ack received state */
	SM_STATE_SEND_ACK,							/**< Send ack state */
	SM_STATE_DEEP_SLEEP,						/**< Deep sleep mode state */
	SM_SEND_SFX_DATA							/**< Send data to Sigfox network state */
} HT_DR_States;

/**
 * \enum HT_DR_PayloadType
 * \brief Payload data type. Specifies what kind of data it is being transmitted.
 */
typedef enum {
	ACK_DATA = 1,								/**< Ack payload type */
	SIGFOX_DATA									/**< Sigfox data type */
} HT_DR_PayloadType;

/**
 * \struct HT_DR_Payload
 * \brief P2P payload buffer.
 */
typedef struct {
	uint8_t payload_type;						/**< HT_DR_PayloadType type*/
	uint8_t data[12];							/**< Sigfox data */
} HT_DR_Payload;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_DR_Fsm(void)
 * \brief Data repeater finite state machine.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_Fsm(void);

/*!******************************************************************
 * \fn void HT_DR_DeepSleepState(void)
 * \brief Deep sleep mode state.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_DeepSleepState(void);

/*!******************************************************************
 * \fn void HT_DR_SendSfxDataState(void)
 * \brief Sends the repeated buffer to the Sigfox network, if it is a gateway.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_SendSfxDataState(void);

/*!******************************************************************
 * \fn void HT_DR_SendAckState(void)
 * \brief Send an ack packet to the source address.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_SendAckState(void);

/*!******************************************************************
 * \fn void HT_DR_AckReceivedState(void)
 * \brief Detects that an ack buffer was received by the destination address.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_AckReceivedState(void);

/*!******************************************************************
 * \fn void HT_DR_WaitForTxDoneState(void)
 * \brief Wait until the end of transmission process.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_WaitForTxDoneState(void);

/*!******************************************************************
 * \fn void HT_DR_SendDataState(void)
 * \brief Sends a payload to the next device in the P2P network.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_SendDataState(void);

/*!******************************************************************
 * \fn void HT_DR_DataReceivedState(void)
 * \brief Gets the received buffer.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_DataReceivedState(void);

/*!******************************************************************
 * \fn void HT_DR_WaitForRxDoneState(void)
 * \brief Wait until the end of RX process
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_WaitForRxDoneState(void);

/*!******************************************************************
 * \fn void HT_DR_StartRxState(void)
 * \brief Start the RX process.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_StartRxState(void);

/*!******************************************************************
 * \fn void HT_DR_StartRxState(void)
 * \brief Gets the data which will be transmitted to the Sigfox network. It could be a sensor data or anything else.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_DR_GetData(void);

#endif /* __HT_DATA_REPEATER_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
