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
 * \file HT_monarch_api.h
 * \brief Monarch API HT32SX iMCP SiP SigFox
 * \author HT Micron Advanced R&D 
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date July 17, 2020
 *
 * This file defines the HT Monarch API.
 */

#ifndef HT_MONARCH_APP_H
#define HT_MONARCH_APP_H

#include "main.h"

 /*!***********************************************************************************************************
 * \fn void HT_MonarchApi_monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer)
 * \brief Executes a scan of the air to detect a Sigfox Beacon. 
 *				It will return 0, if success and the RC enum value corresponding to 
 *				the beacon found and its RSSI level. The scan is executed during the 
 *				specific timer/unit time.
 *
 * \param[in]  sfx_u8 rc_capabilities_bit_mask Bit Mask of the RCx which the scan must to be executed.
 *				
 *							
 *              | Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
 *              |------|:----:|:----:|:----:|:----:|:----:|:----:|-----:|
 *              |   -  | RC7  | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *              
 * \param[in]	sfx_u16 timer									 Monarch Scan timeout (in minutes).
 * \param[out] none
 *
 *
 *************************************************************************************************************/
void HT_MonarchApi_monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer);

/*!***********************************************************************************************************
 * \fn sfx_u8 HT_MonarchApi_callback(sfx_u8 rc_bit_mask, sfx_s16 rssi)
 * \brief Monarch callback called after finding a Monarch Beacon.
 *				
 *				
 *				
 *
 * \param[in]  sfx_u8 rc_bit_mask Bit Mask of the RCx which the scan must to be executed.
 *				
 *							
 *              | Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
 *              |------|:----:|:----:|:----:|:----:|:----:|:----:|-----:|
 *              |   -  |  RC7 | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *              
 * \param[in]	rssi									 Signal rssi.
 * 
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                         No error
 * \retval MCU_ERR_API_FREE:                     Verify sigfox_api.h for more info
 *
 *************************************************************************************************************/
sfx_u8 HT_MonarchApi_callback(sfx_u8 rc_bit_mask, sfx_s16 rssi);

/*!***********************************************************************************************************
 * \fn void HT_MonarchApi_stopMonarchScan(void)
 * \brief Stops the monarch scan.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                         none
 *
 *************************************************************************************************************/
void HT_MonarchApi_stopMonarchScan(void);

/*!***********************************************************************************************************
* \fn uint8_t HT_MonarchApi_getRcBitMask(uint8_t rcz);
* \brief Get the corresponding rcz bit mask.
*
*
*
*
* \param[in]  uint8_t rcz						Integer corresponding to the region wanted.
*
*
*  | Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
*  |------|:----:|:----:|:----:|:----:|:----:|:----:|-----:|
*  |   -  |  RC7 | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
*
*
* \param[out] uint8_t bitMask				Return the bitmask wanted.
*
* \retval RCZ Bitmask
*
*************************************************************************************************************/
uint8_t HT_MonarchApi_getRcBitMask(uint8_t rcz);

#endif

/************************ (C) COPYRIGHT HT Micron Semicondutors S.A *****END OF FILE****/
