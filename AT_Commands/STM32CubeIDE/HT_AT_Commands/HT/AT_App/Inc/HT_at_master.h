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
 * \file HT_at_master.h
 * \brief AT Commands implementation.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date December 16, 2021
 */

#ifndef __HT_AT_MASTER_H__
#define __HT_AT_MASTER_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Defines  ------------------------------------------------------------------*/

#define SIGFOX_MAX_PAYLOAD_SIZE   12					/**< Sigfox max payload size. */
#define MAX_PAYLOAD_STRING		  25					/**< Sigfox max payload string length. */
#define MAX_PARAMETER_SIZE       UART_MAX_SIZE			/**< Max parameter size. */

#define MAX_FLAG_STRING			2						/**< Max flag string length. */
#define MAX_RESPONSE_SIZE		8						/**< Max response string length. */

#define HEADER_DELIMITER        "+"						/**< Header delimiter string. */
#define COMMAND_DELIMITER       "="						/**< Command delimiter string. */
#define PARAMETER_DELIMITER     ":"						/**< Parameter delimiter string. */

#define HEADER_STR              "AT+"					/**< Header string. */

#define N_CMD(tab) ((int32_t)(sizeof(tab)/sizeof(tab)[0]))		/**< Macro to calculate the actual number of available commands */

/* Typedefs  ------------------------------------------------------------------*/

/**
 * \enum AT_Command
 * \brief AT Commands table.
 */
typedef char AT_Command;

/**
 * \enum HT_ATCommand_ErrorCode
 * \brief AT Commands error codes.
 */
typedef enum {
	AT_ERROR_NONE             				= 0x00,			/**< No erros found */
	AT_ERROR_PARAM_CMD            			= 0xA0,			/**< Parameter error */
	AT_ERROR_UNAVAILABLE_CMD				= 0xA1,			/**< Unavailable command error */
	AT_ERROR_HDR 							= 0xA2,			/**< Header command error */
	AT_ERROR_OVF 							= 0xA3,			/**< Overflow command error */
	AT_ERROR_INVALID_HEX_VALUE				= 0xA4,			/**< Invalid hex value. */
	AT_ERROR_INVALID_CRC					= 0xA5,			/**< Invalid CRC value. */
	AT_ERROR_TIMEOUT						= 0xA6			/**< Command timeout value. Usually occurs during monarch scan. */
} HT_ATCommand_ErrorCode;

/**
 * \enum HT_AT_Commands
 * \brief AT Commands list.
 */
typedef enum {
	AT_SIGFOX_IDPAC_CMD = 0,								/**< SigFox configure region command */
	AT_SIGFOX_CFGRCZ_CMD,									/**< SigFox configure region command */
	AT_SIGFOX_SEND_CMD,										/**< SigFox send frame command */
	AT_SIGFOX_SENDCRC_CMD,									/**< SigFox send frame with the CRC of the payload command */
	AT_SIGFOX_SENDBIT_CMD,									/**< SigFox send frame with a null payload command */
	AT_SIGFOX_MONARCH_CMD,									/**< SigFox monarch scan command */
	AT_SIGFOX_STPMONARCH_CMD,								/**< SigFox stop monarch command */
	AT_SIGFOX_CLOSE_CMD,									/**< SigFox close SigFox lib command */
	AT_SIGFOX_CW,											/**< SigFox start continuous transmission command. */
	AT_SIGFOX_STPCW_CMD,									/**< SigFox stop continuous transmission command. */
	AT_SIGFOX_CTMICRO,										/**< SigFox continuous micro-channel transmission command. */
	AT_SIGFOX_CTMACRO,										/**< SigFox continuous macro-channel transmission command. */
	AT_SIGFOX_FREQPHOPP,									/**< SigFox frequency hopping test command. */
	AT_SIGFOX_TESTCRED,										/**< SigFox set test credentials command. */
	AT_MCU_RESET_CMD,										/**< MCU software reset command. */
	AT_MCU_DEEPSLEEP_CMD,									/**< MCU start deep sleep state command. */
	AT_MCU_SWITCHPA_CMD,									/**< MCU switch PA (0/1) command. */
	AT_MCU_SWITCHBOOST_CMD,									/**< MCU switch boost mode (0/1) command. */
	AT_MCU_REDUCEPOWER_CMD,									/**< MCU reduce output power command. */
	AT_MCU_FREQOFFSET_CMD,									/**< MCU set frequency offset command. */
	AT_MCU_RSSIOFFSET_CMD,									/**< MCU set RSSI offset command. */
	AT_MCU_LBTOFFSET_CMD									/**< MCU set LBT offset command. */
} HT_AT_Commands;

/**
 * \struct HT_AT_ErrorCode
 * \brief AT Commands return buffer.
 */
typedef struct {
	uint8_t at_cmd_error;									/**< AT commands error byte. */
	uint8_t sigfox_error;									/**< Sigfox error byte. */
	uint8_t uart_error;										/**< UART error byte. */
} __attribute__((packed)) HT_AT_ErrorCode;

/**
 * \struct HT_AT_Parameter
 * \brief AT Commands parameter table.
 */
typedef struct {
	char *param;											/**< Command parameter pointer. */
} HT_AT_Parameter;

/* ------------------------- Functions --------------------------- */

/*!******************************************************************
 * \fn void HT_AT_SplitCommandData(uint8_t *cmdBuffer, char *delim, HT_AT_Parameter *ptr);
 * \brief Splits the parameter string to parse the data before executing a specific command.
 *
 * \param[in]  uint8_t *cmdBuffer			Command string to be splitted.
 * \param[in]  char *delim					Split delimiter.
 * \param[out] HT_AT_Parameter *ptr			Output pointer.
 *
 * \retval none
 *******************************************************************/
void HT_AT_SplitCommandData(uint8_t *cmdBuffer, char *delim, HT_AT_Parameter *ptr);

/*!******************************************************************
 * \fn void HT_AT_ParseCommandData(char *str, uint8_t len, uint8_t *output);
 * \brief Parse the command parameter, converting from ASCII to hex numbers.
 *
 * \param[in] char *str						String to be converted.
 * \param[in] uint8_t len 					String length.
 * \param[out] uint8_t *output				Output buffer.
 *
 * \retval none
 *******************************************************************/
void HT_AT_ParseCommandData(char *str, uint8_t len, uint8_t *output);

/*!******************************************************************
 * \fn HT_ATCommand_ErrorCode HT_AT_ValidHeaderCommand(uint8_t *str);
 * \brief Check if a the received command has a valid header. 
 *
 * \param[in] char *str						String to be checked.
 * \param[out] none.
 *
 * \retval Error code informing the result of verification. 
 *******************************************************************/
HT_ATCommand_ErrorCode HT_AT_ValidHeaderCommand(uint8_t *str);

/*!******************************************************************
 * \fn HT_ATCommand_ErrorCode HT_AT_ValidCommand(uint8_t *cmd, HT_AT_Commands *cmd_id);
 * \brief Check if a the received string is a valid command.
 *
 * \param[in] uint8_t *cmd						String to be checked.
 * \param[out] HT_AT_Commands *cmd_id			Command ID.
 *
 * \retval Error code informing the result of verification. 
 *******************************************************************/
HT_ATCommand_ErrorCode HT_AT_ValidCommand(uint8_t *cmd, HT_AT_Commands *cmd_id);

/*!******************************************************************
 * \fn HT_AT_ErrorCode HT_AT_ExecuteCommand(uint8_t *ptr, HT_AT_Commands cmd);
 * \brief Executes the received command.
 *
 * \param[in] uint8_t *ptr						Parameter string.
 * \param[in] HT_AT_Commands cmd				Command ID.
 * \param[out] none
 *
 * \retval Error code of the processed command.
 *******************************************************************/
HT_AT_ErrorCode HT_AT_ExecuteCommand(uint8_t *ptr, HT_AT_Commands cmd);

/*!******************************************************************
 * \fn uint8_t HT_AT_CheckErrorCode(HT_AT_ErrorCode code);
 * \brief Check the error code.
 *
 * \param[in] HT_AT_ErrorCode code				Error code variable to be checked.
 * \param[out] none
 *
 * \retval 1 if error none, else 0.
 *******************************************************************/
uint8_t HT_AT_CheckErrorCode(HT_AT_ErrorCode code);

/*!******************************************************************
 * \fn uint8_t HT_AT_GetCommandStrLen(HT_AT_Commands cmd);
 * \brief Gets the command string length.
 *
 * \param[in] HT_AT_Commands cmd			Command ID to be verified.
 * \param[out] none
 *
 * \retval Command string length.
 *******************************************************************/
uint8_t HT_AT_GetCommandStrLen(HT_AT_Commands cmd);

#endif /* __HT_AT_MASTER_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
