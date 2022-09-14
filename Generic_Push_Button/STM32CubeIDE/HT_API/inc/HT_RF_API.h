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
 * \file HT_RF_API.h
 * \brief Implementation of RCZ configuration.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date August 17, 2022
 */

#ifndef __CONFIG_REGION_H__
#define __CONFIG_REGION_H__

#include <stdio.h>
#include "stm32l0xx_hal.h"
#include "sigfox_api.h"
#include "st_rf_api.h"
#include "ST_Sigfox_Init.h"
#ifdef FCC_AT_COMMANDS
#include "HT_hcfsm.h"
#include "HT_ATcmd.h"
#endif


#define RCZ1_OUTPUT_POWER 	-32
#define RCZ2_OUTPUT_POWER	-27
#define RCZ3_OUTPUT_POWER	-32
#define RCZ4_OUTPUT_POWER	-27
#define RCZ5_OUTPUT_POWER	-32
#define RCZ6_OUTPUT_POWER	-32
#define RCZ7_OUTPUT_POWER	-32

/*!******************************************************************
 * \typedef rc_mask
 * \brief Region configuration mask
 * \val RCZ1
 * 		Radio configuration for zone 1
 * \val RCZ2
 * 		Radio configuration for zone 2
 * \val RCZ3
 * 		Radio configuration for zone 3
 * \val RCZ4
 * 		Radio configuration for zone 4
 * \val RCZ5
 * 		Radio configuration for zone 5
 * \val RCZ6
 * 		Radio configuration for zone 6

 *******************************************************************/
typedef enum
{
	RCZ1 = 1,
	RCZ2,
	RCZ3,
	RCZ4,
	RCZ5,
	RCZ6,
	RCZ7
} rc_mask;


#ifdef GENERIC_PUSH_BUTTON
/*!******************************************************************
 * \fn void HT_API_ConfigRegion(rc_mask RCZ)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_API_ConfigRegion(rc_mask RCZ);
#endif /*GENERIC_PUSH_BUTTON*/

#ifdef FCC_AT_COMMANDS
/*!******************************************************************
 * \fn void HT_SigfoxApi_configRegion(rc_mask RCZ, uint32_t frequency)
 * \brief Configures the device with the region and frequency
 * specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[in]  uint32_t	freqeuncy            Desired frequency
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_SigfoxApi_configRegion(rc_mask RCZ, uint32_t frequency);

/*!******************************************************************
 * \fn void HT_SigfoxApi_SetTest(AT_Certification_Type certification)
 * \brief Configure the device to test according to certification type.
 *
 * \param[in] AT_Certification_Type certification  certification type
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_SigfoxApi_SetTest(AT_Certification_Type certification);

/*!******************************************************************
 * \fn void HT_RF_API_SetUplinkCenter(sfx_u32 uplinkCenter)
 * \brief Set uplink_center variable in HT_sigfox_api. Used just
 * in this lib.
 *
 * \param[in] sfx_u32 uplinkCenter              value
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_RF_API_SetUplinkCenter(sfx_u32 uplinkCenter);
#endif /*FCC_AT_COMMANDS*/

#ifdef MONARCH_MOVE_ME
/*!******************************************************************
 * \fn void configRegion(void)
 * \brief Configures the device using the RCZ found after Monarch Scan.
 *				Call the functions St_Sigfox_Open_RCZ, to execute these
 *				configurations and sendFrameRCZ.
 *
 * \param[in]  none
 * \param[out] none
 * \retval none
 *******************************************************************/
void configRegion(void);
#endif /*MONARCH_MOVE_ME*/

#ifdef MONARCH_SCAN
/*!******************************************************************
 * \fn void configRegion(void)
 * \brief Configures the device using the RCZ found after Monarch Scan.
 *				Call the functions St_Sigfox_Open_RCZ, to execute these
 *				configurations and sendFrameRCZ.
 *
 * \param[in]  none
 * \param[out] none
 * \retval none
 *******************************************************************/
void configRegion(void);
#endif /*MONARCH_SCAN*/

#ifdef P2P_SIGFOX_GENERIC
/*!******************************************************************
 * \fn void HT_API_ConfigRegion(void)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_API_ConfigRegion(rc_mask RCZ);
#endif /*P2P_SIGFOX_GENERIC*/

#ifdef PUSH_BUTTON_RC2/*!******************************************************************
 * \fn void HT_RF_API_set_rcz2(void)
 * \brief Configures the device with the RCZ2 region.
 *
 * \param[in]  none
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_RF_API_set_rcz2(void);
#endif /*PUSH_BUTTON_RC2*/

#ifdef RTC_WAKE_UP
/*!******************************************************************
 * \fn void HT_SigfoxApi_configRegion(rc_mask RCZ)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_SigfoxApi_configRegion(rc_mask RCZ);
#endif /*RTC_WAKE_UP*/

#ifdef P2P_SIGFOX
/*!******************************************************************
 * \fn void HT_API_ConfigRegion(rc_mask RCZ)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 * \retval none
 *******************************************************************/
void HT_API_ConfigRegion(rc_mask RCZ);
#endif /*P2P_SIGFOX*/

#ifdef AT_COMMANDS

#include "retriever_api.h"

/*!******************************************************************
 * \fn uint8_t HT_API_ConfigRegion(rc_mask RCZ)
 * \brief Configures the device with the region specified by the user.
 *
 * \param[in]  rc_mask 	RCZ                  RC of the desired region
 * \param[out] none
 * \retval uint8_t error					 0 no error, 1 if error
 *******************************************************************/
uint8_t HT_SigfoxApi_configRegion(rc_mask RCZ);

/*!******************************************************************
 * \fn HT_SigfoxApi_SetTestCredentials(uint8_t en)
 * \brief Set test credentials before start running ANATEL tests.
 *
 * \param[in]  uint8_t en					1 enable, 0 disable
 * \param[out] none
 * \retval uint8_t error					 0 no error, 1 if error
 *******************************************************************/
uint8_t HT_SigfoxApi_SetTestCredentials(uint8_t en);

#endif /*AT_COMMANDS*/

#endif /*__CONFIG_REGION_H__*/

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
