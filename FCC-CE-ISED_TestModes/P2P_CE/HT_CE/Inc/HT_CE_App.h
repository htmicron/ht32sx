

#ifndef __HT_CE_APP_H__
#define __HT_CE_APP_H__

#include "main.h"

#define PAYLOAD_SIZE	12
#define PAYLOAD_DATA	{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}

typedef enum {
	SM_STATE_START_RX = 0, 						/**< Start RX state */
	SM_STATE_WAIT_FOR_RX_DONE,					/**< Wait for RX done state */
	SM_STATE_DATA_RECEIVED,						/**< Data received state */
	SM_STATE_SEND_DATA,							/**< Send data state */
	SM_STATE_WAIT_FOR_TX_DONE					/**< Wait for RX done state */
} HT_FSM_State;

typedef struct {
	uint8_t payload_type;
	uint8_t data[PAYLOAD_SIZE];
} __attribute__((packed)) HT_Payload;

/*!******************************************************************
 * \fn void P2P_StartRx(void)
 * \brief Start RX state routine.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2P_StartRx(void);

/*!******************************************************************
 * \fn void P2P_WaitForTxDone(void)
 * \brief Wait for TX Done state routine.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2P_WaitForTxDone(void);

/*!******************************************************************
 * \fn void P2P_SendData(uint8_t *pTxBuff, uint8_t cTxlen)
 * \brief Send Data state routine.
 *
 * \param[in] uint8_t *pTxBuff					TX packet.
 * \param[in] uint8_t cTxlen					TX buffer length.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2P_SendData(HT_Payload *pTxBuff);


/*!******************************************************************
 * \fn void P2P_DataReceived(HT_Payload *pRxBuff)
 * \brief Data Received state routine.
 *
 * \param[in] uint8_t *pRxBuff					RX packet.
 * \param[in] uint8_t cRxlen					RX buffer length.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2P_DataReceived(HT_Payload *pRxBuff);

/*!******************************************************************
 * \fn void P2P_WaitForRxDone(void)
 * \brief Wait for RX Done state routine.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void P2P_WaitForRxDone(void);

void HT_CE_Fsm(void);

#endif /* __HT_CE_APP_H__ */
