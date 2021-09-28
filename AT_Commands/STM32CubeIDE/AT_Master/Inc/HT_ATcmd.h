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
 * \version 2.2
 * \date July 17, 2020
 *
 * This file defines the AT Commands for iMCP SiP SigFox.
 */


#ifndef __HT_ATCMD__
#define __HT_ATCMD__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32l0xx_hal.h"

/* Defines  ------------------------------------------------------------------*/

#define AT_ERR_NONE					(uint8_t)0x00
#define AT_ERR_PARAM_CMD			(uint8_t)0xA0
#define AT_ERR_UNAVAILABLE_CMD		(uint8_t)0xA1
#define AT_ERR_HDR					(uint8_t)0xA2
#define AT_ERR_OVF					(uint8_t)0xA3

#define MAX_RESPONSE_SIZE			7

#define END_LINE_CH_1				';'
#define END_LINE_CH_2				'\n'
#define END_LINE_CH_3				'\r'

/* Typedef -----------------------------------------------------------*/

/**
 * \enum AT_cmdStrError
 * \brief Specifies all the error types related with the AT commands.
 */

typedef enum{
	ERR_NONE = 				0x00,						/**< No erros found */
	ERR_PARAM_CMD =			0xA0,						/**< Parameter error */
	ERR_UNAVAILABLE_CMD = 	0xA1,						/**< Unavailable command error */
	ERR_HDR = 				0xA2,						/**< Header command error */
	ERR_OVF = 				0xA3,						/**< Overflow command error */
	DUMMY =					0xA4,						/**< Function not finished. */
	ERR_INVALID_HEX_VALUE =	0xA5
} AT_cmdStrError;

typedef enum {
	AT_False = 0,
	AT_True
} AT_TxFlag;

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
	AT_cfgrcz,						/**< SigFox configure region command */
	AT_id_pac,
	AT_cw,
	AT_stop_cw
} AT_sigfoxCmdType;

/**
 * \enum AT_sigfoxCmdType
 * \brief Specifies all SigFox command available.
 */
typedef enum {
	AT_stop = 0,					/**< MCU stop mode command */
	AT_deepSleep,					/**< MCU deep sleep mode command */
	AT_reset,						/**< MCU reset command */
	AT_switch_pa,					/**< MCU switch pa command */
	AT_switch_boost,				/**< MCU switch boost command */
	AT_reduce_power,				/**< MCU reduce power command */
	AT_freq_offset,					/**< MCU frequency offset command */
	AT_rssi_offset,					/**< MCU rssi offset command */
	AT_lbt_offset,					/**< MCU lbt thr offset command */
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

typedef struct {
	uint8_t sfx_error;
	uint8_t at_error;
} __attribute__((packed)) AT_ErrorCode;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn extern void AT_setCmdStr(uint8_t *ptr)
 * \brief Fill a buffer with all bytes received by DMA.
 *
 * \param[in]  uint8_t *ptr						Pointer received by USART DMA.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void AT_setCmdStr(uint8_t *ptr);

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

#endif /* __HT_ATCMD__ */

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
