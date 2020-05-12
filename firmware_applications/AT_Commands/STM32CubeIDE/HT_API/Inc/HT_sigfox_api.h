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
 * \version 2.1
 * \date April 30, 2020
 *
 * This file defines a SigFox API for iMCP SiP SigFox.
 */

#ifndef HT_SIGFOX_API_H
#define HT_SIGFOX_API_H

#include "main.h"

/*!******************************************************************
 * \fn extern sfx_error_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_u8 *customer_response, sfx_bool initiate_downlink_flag)
 * \brief Send a frame to the Sigfox network.
 *
 * \param[in]  sfx_u8 *customer_data				Buffer that is going to be sent.
 * \param[in]  sfx_u8 *customer_response			Buffer that will receive the downlink.
 * \param[in]  sfx_bool initiate_downlink_flag		Downlink flag. If it is 1, the device will wait for a downlink after send a payload.
 *
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                        	none
 *******************************************************************/
extern sfx_error_t HT_SigfoxApi_sendFrame(sfx_u8 *customer_data, sfx_u8 *customer_response, sfx_bool initiate_downlink_flag);

/*!******************************************************************
 * \fn extern void HT_SigfoxApi_configRegion(rc_mask RCZ)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 *******************************************************************/
extern void HT_SigfoxApi_configRegion(rc_mask RCZ);

/*!******************************************************************
 * \fn void HT_MonarchApi_closeSigfoxLib(void)
 * \brief This function closes the library (Free the allocated
 *				memory of SIGFOX_API_open and close RF).
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/
void HT_SigfoxApi_closeSigfoxLib(void);

#endif

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
