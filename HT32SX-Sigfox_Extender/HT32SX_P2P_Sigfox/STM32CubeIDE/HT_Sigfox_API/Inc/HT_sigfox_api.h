/*
 * HT_sigfox_api.h
 *
 *  Created on: Mar 25, 2021
 *      Author: hendrick.goncalves
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

#endif /* __HT_SIGFOX_API_H__ */
