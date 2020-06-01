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
 * \file HT_ATcmd.h
 * \brief AT Commands API HT32SX iMCP SiP Sigfox
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.1
 * \date May 06, 2020
 *
 * This file defines the AT Commands for iMCP SiP SigFox.
 */


#ifndef __HT_ATCMD__
#define __HT_ATCMD__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32l0xx_hal.h"

/* Defines  ------------------------------------------------------------------*/

#define DATA_RX_MAX_BUFF_SIZE    23       /*Max size of the received buffer*/

#define DATA_TX_MAX_BUFF_SIZE    23       /*Max size of the transmit buffer*/

/* Typedef -----------------------------------------------------------*/

/**
 * \enum AT_cmdStrError
 * \brief Specifies all the error types related with the AT commands.
 */

typedef enum{
	DUMMY = 0,						/**< Function not finished. */
	ERR_NONE,						/**< No erros found */
	ERR_PARAM_CMD,					/**< Parameter error */
	ERR_UNAVAILABLE_CMD,			/**< Unavailable command error */
	ERR_HDR,						/**< Header command error */
	ERR_OVF,						/**< Overflow command error */
} AT_cmdStrError;

/**
 * \enum AT_cmdType
 * \brief Specifies if the command is a SigFox or MCU command or even if the hfsm should reset its state.
 */
typedef enum {
	AT_sigfox = 0,					/**< SigFox command type */
	AT_mcu,							/**< MCU command type */
	AT_cmd,							/**< CMD command type. */
} AT_cmdType;

/**
 * \enum AT_sigfoxCmdType
 * \brief Specifies all SigFox command available.
 */
typedef enum {
	AT_sendFrame = 0,				/**< SigFox send frame command */
	AT_monarchScan,					/**< SigFox monarch scan command */
	AT_stpMonarch,					/**< SigFox stop monarch command */
	AT_close,						/**< SigFox close SigFox lib command */
	AT_cfgrcz						/**< SigFox configure region command */
} AT_sigfoxCmdType;

/**
 * \enum AT_sigfoxCmdType
 * \brief Specifies all SigFox command available.
 */
typedef enum {
	AT_stop = 0,					/**< MCU stop mode command */
	AT_deepSleep,					/**< MCU deep sleep mode command */
	AT_reset,						/**< MCU reset command */
	AT_wkp							/**< MCU wake up command */
} AT_mcuCmdType;

/**
 * \struct AT_cmdStruct
 * \brief Tuple error implementation.
 */
typedef struct AT_cmdStruct {
	AT_cmdType AT_type;					/**< Command type (SigFox, MCU or CMD) */
	AT_sigfoxCmdType AT_sigfoxCmd;		/**< SigFox command */
	AT_mcuCmdType AT_mcuCmd;			/**< MCU command */
	AT_cmdStrError AT_err;				/**< Error status */
} AT_cmdStruct;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn extern void AT_setCmdStr(uint8_t *ptr, uint8_t Write, uint8_t tocopy)
 * \brief Fill a buffer with all bytes received by DMA.
 *
 * \param[in]  uint8_t *ptr						Pointer received by USART DMA.
 * \param[in]  uint8_t Write					Buffer ID.
 * \param[in]  uint8_t tocopy					Ptr length that will be copy.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_setCmdStr(uint8_t *ptr, uint8_t Write, uint8_t tocopy);

/*!******************************************************************
 * \fn extern void AT_getCmdData(uint8_t *cmd)
 * \brief Copy the buffer received by DMA to *cmd.
 *
 * \param[in]  uint8_t *cmd						Buffer that will receive the AT command.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_getCmdData(uint8_t *cmd);

/*!******************************************************************
 * \fn extern AT_cmdStruct AT_checkCmdString(char *cmdStr)
 * \brief Check if the second parameter is an available command.
 *
 * \param[in]  char *cmdStr						Buffer received by DMA.
 * \param[out] none
 *
 * \retval AT_cmdStruct status					Return the error status and the command type which was received.
 *******************************************************************/
extern AT_cmdStruct AT_checkCmdString(char *cmdStr);

/*!******************************************************************
 * \fn extern void AT_cleanAuxBuffer(void)
 * \brief Reset the DMA buffer.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_cleanAuxBuffer(void);

/*!******************************************************************
 * \fn AT_cmdStruct AT_checkCmdHdr(char *cmd)
 * \brief Check if the command header is an available one.
 *
 * \param[in]  char *cmd			The whole AT command.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
AT_cmdStruct AT_checkCmdHdr(char *cmd);

#endif

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
