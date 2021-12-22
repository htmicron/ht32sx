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
 * \file HT_sigfox_api.h
 * \brief HT SigFox API HT32SX iMCP SiP SigFox
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date July 17, 2020
 *
 * This file defines a SigFox API for iMCP SiP SigFox.
 */

#ifndef HT_SIGFOX_API_H
#define HT_SIGFOX_API_H

#include "main.h"

#define MAX_PAYLOAD_SIZE 12
#define MAX_HEX_SIZE 24

/*!******************************************************************
 * \fn uint8_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_bool initiate_downlink_flag, uint8_t len)
 * \brief Send a frame to the Sigfox network.
 *
 * \param[in]  sfx_u8 *customer_data				Buffer that is going to be sent.
 * \param[in]  sfx_bool initiate_downlink_flag		Downlink flag. If it is 1, the device will wait for a downlink after send a payload.
 * \param[in]  uint8_t len							Payload data length.
 *
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                        	none
 *******************************************************************/
uint8_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_bool initiate_downlink_flag, uint8_t len);

/*!******************************************************************
 * \fn uint8_t HT_SigfoxApi_sendFrame(sfx_bool bit_value, sfx_bool initiate_downlink_flag)
 * \brief Send a single bit to the Sigfox network.
 *
 * \param[in]  sfx_bool bit_value					Bit vallue (0/1).
 * \param[in]  sfx_bool initiate_downlink_flag		Downlink flag. If it is 1, the device will wait for a downlink after send a payload.
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                        	none
 *******************************************************************/
uint8_t HT_SigfoxApi_sendBit(sfx_bool bit_value, sfx_bool initiate_downlink_flag);

/*!******************************************************************
 * \fn extern void HT_SigfoxApi_configRegion(rc_mask RCZ)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 *******************************************************************/
uint8_t HT_SigfoxApi_configRegion(rc_mask RCZ);

/*!******************************************************************
 * \fn void HT_MonarchApi_closeSigfoxLib(void)
 * \brief This function closes the library (Free the allocated
 *				memory of SIGFOX_API_open and close RF).
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/
uint8_t HT_SigfoxApi_closeSigfoxLib(void);

/*!******************************************************************
 * \fn void HT_SigfoxApi_switchPa(uint8_t state)
 * \brief Set PA flag.
 *
 * \param[in]  sfx_u8 state                 PA state.
 * \param[out] none
 *******************************************************************/
void HT_SigfoxApi_switchPa(uint8_t state);

/*!******************************************************************
 * \fn void HT_SigfoxApi_setSmpsVoltageAction(sfx_u8 mode)
 * \brief Enable S2LP boost mode.
 *
 * \param[in]  sfx_u8 mode                  Desired output voltage. Check UM2169 for more details.
 * \param[out] none
 *******************************************************************/
void HT_SigfoxApi_setSmpsVoltageAction(sfx_u8 mode);

uint8_t HT_SigfoxApi_GetCredentials(void);
uint8_t HT_SigfoxApi_ContinuousWave(uint32_t frequency);
uint8_t HT_SigfoxApi_StopCW(void);

#endif

/************************ HT Micron Semiconductors S.A *****END OF FILE****/
