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
 * \file HT_hcfsm.h
 * \brief Hierarchical State Machine HT32SX iMCP SiP Sigfox
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date July 17, 2020
 *
 * This file defines the Hierarchical State Machine made for modulate the AT Commands application.
 */

#ifndef _HT_HCFSM_H_
#define _HT_HCFSM_H_

#include "main.h"

/* Typedef -------------------------------------------------------------------------------------------------------*/

/**
 * \void state_func
 * \brief State type.
 */

typedef void *(*state_func)();

typedef unsigned char AT_flagu8;

/**
 * \enum AT_flagType
 * \brief Specifies the command control flags type.
 */
typedef enum {
	CMD_RECEIVING = 0,
	CMD_RECEIVED
} AT_flagType;

/**
 * \enum AT_execStatus
 * \brief Specifies the error status after running a command.
 */
typedef enum {
	AT_error = 0,
	AT_ok,
} AT_execStatus;

/**
 * \struct AT_Cmd_stateFlags
 * \brief Implement a type of tuple error for the state machine control.
 */
typedef struct AT_Cmd_stateFlags_{
	AT_flagu8 cmdReceived;
	AT_flagu8 cmdReceivingCmd;
	AT_cmdType cmdType;
	AT_execStatus returnStatus;
} AT_Cmd_stateFlags;


/*!******************************************************************
 * \fn extern void HT_hcfsm(void)
 * \brief HCFSM main function.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void HT_hcfsm(void);

/* -----------------------------------------------AT_Cmd_GENERAL-STATE---------------------------------------------*/

/*!******************************************************************
 * \fn void * AT_Cmd(void)
 * \brief CMD general state: This is the main state and will the one which will
 * 			call the others general states.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Cmd(void);

/*!******************************************************************
 * \fn void * AT_Cmd_waitingCmd(void)
 * \brief AT_Cmd_waitingCmd state: Wait until detect a command.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Cmd_waitingCmd(void);

/*!******************************************************************
 * \fn void * AT_Cmd_getCmd(void)
 * \brief AT_Cmd_waitingCmd state: Get command after detect the end of command string.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Cmd_getCmd(void);

/*!******************************************************************
 * \fn void * AT_cmd_returnStatus(void)
 * \brief AT_cmd_returnStatus state: Return the operation status after execute a command.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_cmd_returnStatus(void);

/*!******************************************************************
 * \fn void * AT_Mcu(void)
 * \brief MCU general state: This is the main state related with all MCU commands.
 * 		It will call every MCU state.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Mcu(void);

/*!******************************************************************
 * \fn void * AT_SigFox(void)
 * \brief SigFox general state: This is the main state related with all SigFox commands.
 * 		It will call every SigFox state.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_SigFox(void);

/* ----------------------------------------------AT_Error_Routine--------------------------------------------------*/

/*!******************************************************************
 * \fn void * AT_errorRoutine(void)
 * \brief AT_errorRoutine state: State that will handle all possible errors related with the AT commands.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_errorRoutine(void);

/* -----------------------------------------------AT_Mcu_SUB-STATE-------------------------------------------------*/

/*!******************************************************************
 * \fn void * AT_Mcu_getParameters(void)
 * \brief AT_Mcu_getParameters state: After detected that It's a MCU command, this state is called to
 * 		 get the command parameters.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Mcu_getParameters(void);

/*!******************************************************************
 * \fn void * AT_Mcu_executeCmd(void)
 * \brief AT_Mcu_executeCmd state: Run a MCU command.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Mcu_executeCmd(void);

/* ----------------------------------------------------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void * AT_Sigfox_getParameters(void)
 * \brief AT_Sigfox_getParameters state: After detected that It's a SigFox command, this state is called to
 * 		 get the command parameters.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_Sigfox_getParameters(void);

/*!******************************************************************
 * \fn void * AT_SigFox_executeCmd(void)
 * \brief AT_SigFox_executeCmd state: Run a SigFox command.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void * AT_SigFox_executeCmd(void);

/*---------------------------------------------- HCFSM methods ------------------------------------------------------*/

/*!******************************************************************
 * \fn extern void AT_setCmdFlag(AT_flagu8 flag)
 * \brief Set the command received flag in order to control the state machine.
 *
 * \param[in]  AT_flagu8 flag				Flag value that will be set.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_setCmdFlag(AT_flagu8 flag);

/*!******************************************************************
 * \fn extern void AT_updateFsm(AT_cmdStrError errStatus, AT_cmdType cmdType)
 * \brief Force an update on the state machine.
 *
 * \param[in]  AT_cmdStrError errStatus			 Error status.
 * \param[in]  AT_cmdType cmdType				 Discontinued
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_updateFsm(AT_cmdStrError errStatus, AT_cmdType cmdType);

/*!******************************************************************
 * \fn void AT_splitString(char *cmd, char *splitChar, char *ptr)
 * \brief Split the command string to get all parameters.
 *
 * \param[in]  char *cmd				Entire command.
 * \param[in]  char *splitChar			String that specifies where should it split.
 * \param[out] char *ptr				Return string.
 *
 * \retval none
 *******************************************************************/
void AT_splitString(char *cmd, char *splitChar, char *ptr);

/*!******************************************************************
 * \fn extern void AT_setFsmState(state_func state)
 * \brief Set a new fsg.
 *
 * \param[in]  state_func state				New state.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_setFsmState(state_func state);

/*!******************************************************************
 * \fn void AT_getHexValue(char *str, uint8_t *output)
 * \brief Convert ACSII data to hex.
 *
 * \param[in]  char *str			ASCII payload.
 * \param[out] uint8_t *output		Output as hex data
 *
 * \retval none
 *******************************************************************/
void AT_getHexValue(char *str, uint8_t *output);

#endif /* _HT_HCFSM_H_ */

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
