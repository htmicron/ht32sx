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
 * \file HT_data_repeater.h
 * \brief Sigfox functions for P2P+Sigfox application.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.0
 * \date April 28, 2021
 */


#ifndef __HT_SIGFOX_API_H__
#define __HT_SIGFOX_API_H__

#include "main.h"
#include "HT_data_repeater.h"

/*!******************************************************************
 * \fn sfx_error_t HT_API_SendFrame(void)
 * \brief Send a frame to the Sigfox network..
 *
 * \param[in]  none
 * \param[out] none
 *******************************************************************/
sfx_error_t HT_API_SendFrame(HT_DR_Payload *payload);

/*!******************************************************************
 * \fn void HT_API_ConfigRegion(void)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 *******************************************************************/
void HT_API_ConfigRegion(rc_mask RCZ);

/*!******************************************************************
 * \fn void HT_API_setSmpsVoltageAction(sfx_u8 mode)
 * \brief Enable S2LP boost mode.
 *
 * \param[in]  sfx_u8 mode                  Desired output voltage. Check UM2169 for more details.
 * \param[out] none
 *******************************************************************/
void HT_API_setSmpsVoltageAction(sfx_u8 mode);

#endif /* __HT_SIGFOX_API_H__ */
