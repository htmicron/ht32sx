/**
 *
 * Copyright (c) 2021 HT Micron Semicondutors S.A.
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

/* Includes------------------------------------------------------------------------- */

#include "HT_uart_api.h"
#include "HT_at_fsm.h"
#include "usart.h"
#include "tim.h"

/* Global variables and constants --------------------------------------------------- */

static HT_UART_State uart_state = SM_CONFIG_UART;
static HT_UART_State prev_uart_state = SM_RETURN_STRING;

HT_UART_ErrorCode uart_error = UART_ERROR_NONE;

static uint8_t rx_buffer = 0;
static uint8_t uart_buffer[UART_MAX_SIZE];
volatile uint8_t usart_callback = 0;

static volatile uint8_t len_cnt = 0;

static volatile uint8_t timeout_start = 0;

/* Function prototypes ------------------------------------------------------------- */

/*!******************************************************************
 * \fn static uint8_t HT_UART_EndCharacter(char ch)
 * \brief Check if the received character is an end line character (;).
 *
 * \param[in] none.
 *
 * \retval 1 if true, 0 if false.
 *******************************************************************/
static uint8_t HT_UART_EndCharacter(char ch);

/*!******************************************************************
 * \fn static HT_UART_ErrorCode HT_UART_AppendChar(void);
 * \brief Append the received character to the final string.
 *
 * \param[in] none.
 *
 * \retval Error code of the UART FSM execution.
 *******************************************************************/
static HT_UART_ErrorCode HT_UART_AppendChar(void);

/*!******************************************************************
 * \fn static void HT_UART_WaitForExecutionState(void);
 * \brief Wait for Execution State implementation. Keeps waiting for the end of the command execution to restart the FSM.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_UART_WaitForExecutionState(void);

/*!******************************************************************
 * \fn static void HT_UART_ReturnStringState(void);
 * \brief Return Received String State implementation. Sets the received flag to inform that a new command was received.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_UART_ReturnStringState(void);

/*!******************************************************************
 * \fn static void HT_UART_WaitForEndState(void);
 * \brief Wait for the End of Command state implementation. Keeps waiting for the end line character.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_UART_WaitForEndState(void);

/*!******************************************************************
 * \fn static void HT_UART_WaitForCharState(void);
 * \brief Wait for Character state implementation. Keeps waiting for first character of a new command be received.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_UART_WaitForCharState(void);

/*!******************************************************************
 * \fn static void HT_UART_ConfigUARTState(void);
 * \brief Configure UART State implementation. Resets the UART configuration.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_UART_ConfigUARTState(void);

/* Function  --------------------------------------------------------------------------- */

void HT_UART_GetUartBuffer(uint8_t *output) {
	memcpy(output, uart_buffer, UART_MAX_SIZE);
}

uint8_t HT_UART_GetCmdLen(void) {
	return strlen((char *)uart_buffer);
}

void HT_UART_SetUartState(HT_UART_State state) {
	uart_state = state;
}

HT_UART_ErrorCode HT_UART_GetErrorCode(void) {
	return uart_error;
}

void HT_UART_SetErrorCode(HT_UART_ErrorCode error) {
	uart_error = error;
}

static uint8_t HT_UART_EndCharacter(char ch) {
	return (ch == END_LINE_CHARACTER_1) ? 1 : 0;
}

static HT_UART_ErrorCode HT_UART_AppendChar(void) {

	if((len_cnt+1) < UART_MAX_SIZE) {
		uart_buffer[len_cnt] = rx_buffer;
		uart_error = UART_ERROR_NONE;
		len_cnt += 1;
	} else {
		uart_error = UART_ERROR_OVERFLOW;
	}

	return uart_error;
}

static void HT_UART_WaitForExecutionState(void) {

	prev_uart_state = SM_WAIT_FOR_EXECUTION;
	HAL_NVIC_DisableIRQ(USART1_IRQn);

	uart_state = !HT_AT_GetReceivedFlag() ? SM_CONFIG_UART : SM_WAIT_FOR_EXECUTION;
}

static void HT_UART_ReturnStringState(void) {
	HAL_TIM_Base_Stop_IT(&htim22);
	timeout_start = 0;

	HT_AT_SetReceivedFlag(1);

	prev_uart_state = SM_RETURN_STRING;
	uart_state = SM_WAIT_FOR_EXECUTION;
}

static void HT_UART_WaitForEndState(void) {

	if(usart_callback) {
		usart_callback = 0;

		if(!timeout_start) {
			HAL_TIM_Base_Start_IT(&htim22);
			timeout_start = 1;
			HT_UART_DisableParalellIrqn();
		}

		if(HT_UART_EndCharacter(rx_buffer)) {
			uart_state = SM_RETURN_STRING;
		} else if (HT_UART_AppendChar() != UART_ERROR_NONE) {
			uart_state = SM_RETURN_STRING;
			uart_error = UART_ERROR_OVERFLOW;
		} else {
			uart_state = SM_CONFIG_UART ;
		}
	} else if(uart_error == UART_ERROR_TIMEOUT) {
		uart_state = SM_RETURN_STRING;
	} else{
		uart_state = SM_WAIT_FOR_END;
	}

	prev_uart_state = SM_WAIT_FOR_END;
}

static void HT_UART_WaitForCharState(void) {

	if(usart_callback) {

		if(!timeout_start) {
			HAL_TIM_Base_Start_IT(&htim22);
			timeout_start = 1;
			HT_UART_DisableParalellIrqn();
		}

		usart_callback = 0;

		HT_UART_AppendChar();

		uart_state = SM_CONFIG_UART;

	} else if(uart_error == UART_ERROR_TIMEOUT) {
		uart_state = SM_RETURN_STRING;
	} else{
		uart_state = SM_WAIT_FOR_CHAR;
	}

	prev_uart_state = SM_WAIT_FOR_CHAR;
}

static void HT_UART_ConfigUARTState(void) {

	rx_buffer = 0;
	HAL_UART_Receive_IT(&huart1, &rx_buffer, UART_BUFFER_SIZE);

	if(prev_uart_state == SM_WAIT_FOR_EXECUTION) {

		uart_state = SM_WAIT_FOR_CHAR;
		uart_error = UART_ERROR_NONE;
		usart_callback = 0;

		HAL_NVIC_EnableIRQ(USART1_IRQn);
		rx_buffer = 0;

		memset(uart_buffer, 0, sizeof(uart_buffer));
		len_cnt = 0;
	} else{
		uart_state = SM_WAIT_FOR_END;
	}

	prev_uart_state = SM_CONFIG_UART;
}

void HT_UART_Fsm(void) {

	switch (uart_state) {
	case SM_CONFIG_UART:
		HT_UART_ConfigUARTState();
		break;
	case SM_WAIT_FOR_CHAR:
		HT_UART_WaitForCharState();
		break;
	case SM_WAIT_FOR_END:
		HT_UART_WaitForEndState();
		break;
	case SM_RETURN_STRING:
		HT_UART_ReturnStringState();
		break;
	case SM_WAIT_FOR_EXECUTION:
		HT_UART_WaitForExecutionState();
		break;
	}
}

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
