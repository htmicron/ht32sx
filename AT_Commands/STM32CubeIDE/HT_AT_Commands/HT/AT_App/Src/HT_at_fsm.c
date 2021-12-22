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
#include "HT_at_fsm.h"
#include "HT_uart_api.h"
#include "HT_at_master.h"
#include "usart.h"

/* Global variables and constants --------------------------------------------------- */

HT_AT_State at_state = SM_WAIT_FOR_CMD;

volatile uint8_t cmd_received = 0;
static HT_AT_ErrorCode ret;

static HT_AT_Commands command;
static uint8_t cmd_param[MAX_PARAMETER_SIZE];

/* Function prototypes ------------------------------------------------------------- */

/*!******************************************************************
 * \fn static void HT_AT_ResetCtrlFlags(void)
 * \brief Resets all control flags before restarting the finite state machine.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_ResetCtrlFlags(void);

/*!******************************************************************
 * \fn static void HT_AT_TransmitErrorCode(void)
 * \brief Transmit the error code after a command being executed.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_TransmitErrorCode(void);

/*!******************************************************************
 * \fn static void HT_AT_WaitForCmdState(void)
 * \brief Wait for a command state implementation. Keeps waiting for the "received flag" raised by UART FSM.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_WaitForCmdState(void);

/*!******************************************************************
 * \fn static void HT_AT_DecodeCmdState(void)
 * \brief Decode command state implementation. After a command being received, starts decoding it before the execution.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_DecodeCmdState(void);

/*!******************************************************************
 * \fn static void HT_AT_ExecuteCmdState(void)
 * \brief Execute command state implementation. Executes the decoded command.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_ExecuteCmdState(void);

/*!******************************************************************
 * \fn static void HT_AT_ReturnResultState(void)
 * \brief Return Result state implementation. Returns the status of the command execution.
 *
 * \param[in] none.
 *
 * \retval none.
 *******************************************************************/
static void HT_AT_ReturnResultState(void);

void HT_AT_SetReceivedFlag(uint8_t val) {
    cmd_received = val;
}

uint8_t HT_AT_GetReceivedFlag(void) {
    return cmd_received;
}

static void HT_AT_ResetCtrlFlags(void) {
    cmd_received = 0;
    memset((uint8_t *)&ret, 0, sizeof(HT_AT_ErrorCode));
}

static void HT_AT_TransmitErrorCode(void) {
	uint8_t buffer[MAX_RESPONSE_SIZE] = {'\0'};

	sprintf((char *)buffer, "0x%02X%02X\n", ret.sigfox_error, ret.at_cmd_error);
	HAL_UART_Transmit(&huart1, buffer, strlen((char *)buffer), 0xFFFF);
}

static void HT_AT_WaitForCmdState(void) {

    if(!cmd_received) {
        HT_UART_Fsm();
        at_state = SM_WAIT_FOR_CMD;
    } else {
        at_state = SM_DECODE_CMD;
    }
}

static void HT_AT_DecodeCmdState(void) {
    uint8_t cmd_str[UART_MAX_SIZE];

    memset(cmd_str, 0, UART_MAX_SIZE);
    HT_UART_GetUartBuffer(cmd_str);

    ret.uart_error = HT_UART_GetErrorCode();
    
    if(ret.uart_error == UART_ERROR_NONE) {                               //check command string
        ret.at_cmd_error = HT_AT_ValidHeaderCommand(cmd_str);        
        if(ret.at_cmd_error == AT_ERROR_NONE) {                               //check command header
            ret.at_cmd_error = HT_AT_ValidCommand(&cmd_str[3], &command);     
            if(ret.at_cmd_error == AT_ERROR_NONE) {                           //check command parameter
                memset(cmd_param, 0, MAX_PARAMETER_SIZE);
                strcpy((char *)cmd_param, (char *)&cmd_str[HT_AT_GetCommandStrLen(command)+4]);
            }
        } 
    } else {
    	ret.at_cmd_error = AT_ERROR_TIMEOUT;
    }

    at_state = HT_AT_CheckErrorCode(ret) ? SM_EXECUTE_CMD : SM_RETURN_RESULT;
}

static void HT_AT_ExecuteCmdState(void) {

	HT_UART_EnableParalellIrqn();
    ret = HT_AT_ExecuteCommand(cmd_param, command);
    at_state = SM_RETURN_RESULT;
}

static void HT_AT_ReturnResultState(void) {

	HT_AT_TransmitErrorCode();

    HT_AT_ResetCtrlFlags();
    at_state = SM_WAIT_FOR_CMD;
}

void HT_AT_Fsm(void) {

    switch (at_state) {
    case SM_WAIT_FOR_CMD:
        HT_AT_WaitForCmdState();
        break;
    case SM_DECODE_CMD:
        HT_AT_DecodeCmdState();
        break;
    case SM_EXECUTE_CMD:
        HT_AT_ExecuteCmdState();
        break;
    case SM_RETURN_RESULT:
        HT_AT_ReturnResultState();
        break;
    default:
        break;
    }
}

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
