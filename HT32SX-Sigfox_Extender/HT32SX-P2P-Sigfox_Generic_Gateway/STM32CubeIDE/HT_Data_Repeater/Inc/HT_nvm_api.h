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
 * \file HT_nvm_api.h
 * \brief NVM api for data repeater application.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date April 22, 2021
 */


#ifndef __HT_NVM_API_H__
#define __HT_NVM_API_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "HT_data_repeater.h"
#include "HT_retriever.h"
#include "SDK_UTILS_Flash.h"

/* Defines  ------------------------------------------------------------------*/
#define CONTEXT_ADDRESS			0x08080080							/**< EEPROM address of Sigfox context. */
#define CREDENTIALS_ADDRESS		0x08080000							/**< Sigfox Credentials address. */
#define SEQUENCE_N_ADDRESS		0x08080050							/**< Sequence number address. */
#define EEPROM_LAST_ADDRESS		0x080800F0							/**< Last context address. */
#define SFX_ENVIRONMENT_ADDR	0x0808003C							/**< Sigfox environment address. */
#define SFX_BKP_ADDRESS			0x08080110

#define CONTEXT_SIZE			128									/**< Context memory size. */
#define CONTEXT_DATA_SIZE		16									/**< Context data size. Only the current values.  */

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_NVM_StoreContext(void)
 * \brief Store Sigfox context.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_NVM_StoreContext(void);

/*!******************************************************************
 * \fn void HT_NVM_ConfigEEPROM(uint8_t *sfx_credentials, uint8_t *sfx_environment, uint8_t *sfx_context)
 * \brief Reconfigures NVM using the current data received from P2P network.
 *
 * \param[in] uint8_t *sfx_credentials					Sigfox credentials.
 * \param[in] uint8_t *sfx_environment					Sigfox Environment.
 * \param[in] uint8_t *sfx_context						Sigfox Context.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_NVM_ConfigEEPROM(uint8_t *sfx_credentials, uint8_t *sfx_environment, uint8_t *sfx_context);

/*!******************************************************************
 * \fn void HT_NVM_GetSigfoxContext(uint8_t *sfx_credentials, uint8_t *sfx_environment, uint8_t *sfx_context)
 * \brief Get the current Sigfox context.
 *
 * \param[in]  none
 * \param[out] uint8_t *sfx_credentials					Sigfox credentials.
 * \param[out] uint8_t *sfx_environment					Sigfox Environment.
 * \param[out] uint8_t *sfx_context						Sigfox Context.
 *
 * \retval none
 *******************************************************************/
void HT_NVM_GetSigfoxContext(uint8_t *sfx_credentials, uint8_t *sfx_environment, uint8_t *sfx_context);

void HT_NVM_InitBkpAddress(void);

#endif /* __HT_NVM_API_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
