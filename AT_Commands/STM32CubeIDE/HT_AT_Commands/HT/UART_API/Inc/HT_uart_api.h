
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
 * \file HT_uart_api.h
 * \brief UART APi for AT Commands implementation.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date December 16, 2021
 */

#ifndef __HT_UART_API_H__
#define __HT_UART_API_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Defines -------------------------------------------------------------------*/

#define UART_BUFFER_SIZE 1
#define UART_MAX_SIZE  60

#define END_LINE_CHARACTER_1 ';'

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum HT_UART_ErrorCode
 * \brief UART API error list.
 */
typedef enum {
    UART_ERROR_NONE = 0x00,								/** </ UART API error none. */
    UART_ERROR_OVERFLOW = 0x01,							/** </ UART API string overflow error. */
    UART_ERROR_UNDERFLOW = 0x02,							/** </ UART API string underflow error. */
	UART_ERROR_TIMEOUT = 0x03
} HT_UART_ErrorCode;

/**
 * \enum HT_UART_State
 * \brief UART API FSM states.
 */
typedef enum {
    SM_WAIT_FOR_CHAR = 0,
    SM_CONFIG_UART,
    SM_WAIT_FOR_END,
    SM_RETURN_STRING,
    SM_WAIT_FOR_EXECUTION
} HT_UART_State;

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_UART_Fsm(void)
 * \brief UART APi finite state machine.
 *
 * \param[in] none
 * \param[out] none.
 *
 * \retval none
 *******************************************************************/
void HT_UART_Fsm(void);

/*!******************************************************************
 * \fn HT_UART_ErrorCode HT_UART_GetErrorCode(void)
 * \brief Gets the error code of UART API execution.
 *
 * \param[in] none
 * \param[out] none.
 *
 * \retval none
 *******************************************************************/
HT_UART_ErrorCode HT_UART_GetErrorCode(void);
void HT_UART_SetErrorCode(HT_UART_ErrorCode error);
void HT_UART_SetUartState(HT_UART_State state);

/*!******************************************************************
 * \fn void HT_UART_GetUartBuffer(uint8_t *output)
 * \brief Gets the UART buffer received.
 *
 * \param[in] uint8_t *output					UART output buffer.
 * \param[out] none.
 *
 * \retval none
 *******************************************************************/
void HT_UART_GetUartBuffer(uint8_t *output);

/*!******************************************************************
 * \fn uint8_t HT_UART_GetCmdLen(void);
 * \brief Gets the command string length.
 *
 * \param[in] none.
 * \param[out] none.
 *
 * \retval none
 *******************************************************************/
uint8_t HT_UART_GetCmdLen(void);

#endif /* __HT_UART_API_H__ */
