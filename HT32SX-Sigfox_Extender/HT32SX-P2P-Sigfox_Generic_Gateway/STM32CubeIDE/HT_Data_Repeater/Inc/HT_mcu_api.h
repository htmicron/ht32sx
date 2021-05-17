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
 * \file HT_mcu_api.h
 * \brief API for sleep mode routines.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date March 29, 2021
 */


#ifndef __HT_MCU_API_H__
#define __HT_MCU_API_H__

#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"
#include "rtc.h"

#include "HT_mcu_api.h"

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_McuApi_EnableRTCWkp(uint32_t seconds)
 * \brief Configures RTC to wake up from standby mode.
 *
 * \param[in]  uint32_t seconds					WKP period.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_McuApi_EnableRTCWkp(uint32_t seconds);

/*!******************************************************************
 * \fn void HT_McuApi_DeepSleepMode(void)
 * \brief Set deep sleep mode.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void HT_McuApi_DeepSleepMode(void);

#endif /* __HT_MCU_API_H__ */
