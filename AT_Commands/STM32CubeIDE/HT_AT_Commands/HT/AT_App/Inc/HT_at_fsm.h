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
 * \file HT_at_fsm.h
 * \brief AT Commands finite state machine implementation. 
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date December 16, 2021
 */

#ifndef __HT_AT_FSM_H__
#define __HT_AT_FSM_H__

#include "main.h"

/* Defines  ------------------------------------------------------------------*/

#define AT_CMD_ERROR_NONE   (uint16_t)0x0000  /**</ AT Commands error none constant. */

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum HT_AT_State
 * \brief AT commands FSM states.
 */
typedef enum {
    SM_WAIT_FOR_CMD = 0,                        /**</ Wait for a command state. */
    SM_DECODE_CMD,                              /**</ Decode command state. */
    SM_EXECUTE_CMD,                             /**</ Execute command state. */
    SM_RETURN_RESULT                            /**</ Return result state. */
} HT_AT_State;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_AT_Fsm(void)
 * \brief AT Commands finite state machine.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_AT_Fsm(void);

/*!******************************************************************
 * \fn extern void HT_AT_SetReceivedFlag(uint8_t val);
 * \brief Sets a flag informing that a new commands was received.
 *
 * \param[in]  uint8_t val                 Flag value.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void HT_AT_SetReceivedFlag(uint8_t val);

/*!******************************************************************
 * \fn extern uint8_t HT_AT_GetReceivedFlag(void);
 * \brief Gets the received flag status.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval 1 if received and 0 if not.
 *******************************************************************/
extern uint8_t HT_AT_GetReceivedFlag(void);

#endif /* __HT_AT_FSM_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
