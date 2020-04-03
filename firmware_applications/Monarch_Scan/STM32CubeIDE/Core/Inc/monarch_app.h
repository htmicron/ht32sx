/*
             _       _
           _| |__  _| |_
          |_   _ \|_   _|
            | | | |_| |
  _   __    |_| \_____|
 |  \/  |_  ____ ___   ___  _ __
 | |\/| | |/ ___) _ \ / _ \| '_ \
 | |  | | | (___|   /( (_) | | | |
 |_|  |_|_|\____|_|_\ \___/|_| |_|
 ========== Advanced R&D =========

*/

/*!
 * \file monarch_app.h
 * \brief Monarch Application HT32SX iMCP SiP Sigfox
 * \author HT Micron Advanced R&D 
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date November 20, 2019
 *
 * This file defines the Monarch Scan Application.
 */

#ifndef MONARCH_APP_H
#define MONARCH_APP_H

#include "main.h"

/*!******************************************************************
 * \fn void configRegion(void)
 * \brief Configures the device using the RCZ found after Monarch Scan. 
 *				Call the functions St_Sigfox_Open_RCZ, to execute these
 *				configurations and sendFrameRCZ.
 *
 * \param[in]  none                        
 * \param[out] none
 *******************************************************************/
void configRegion(void);

/*!******************************************************************
 * \fn void closeSigfoxLib(void)
 * \brief This function closes the library (Free the allocated 
 *				memory of SIGFOX_API_open and close RF).
 *
 * \param[in]  none                        
 * \param[out] none
 *******************************************************************/
void closeSigfoxLib(void);

/*!******************************************************************
 * \fn void sendFrameRCZ(rc_mask RCZ)
 * \brief Send a frame to the Sigfox network using the RCZ found 
 *				after Monarch Scan.
 *
 * \param[in]  rc_mask RCZ											 Radio zone                        
 * \param[out] none
 *******************************************************************/
 void sendFrameRCZ(rc_mask RCZ); 
 
 /*!***********************************************************************************************************
 * \fn void monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit)
 * \brief Executes a scan of the air to detect a Sigfox Beacon. 
 *				It will return 0, if success and the RC enum value corresponding to 
 *				the beacon found and its RSSI level. The scan is executed during the 
 *				specific timer/unit time.
 *
 * \param[in]  sfx_u8 rc_capabilities_bit_mask Bit Mask of the RCx on which the scan has to be executed.    
 *				
 *							
 *              | Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
 *              |------|:----:|:----:|:----:|:----:|:----:|:----:|-----:|
 *              |   -  | RC7  | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *              
 * \param[in]	sfx_u16 timer									 Scan duration value (with the unit parameter information).
 * \param[in] sfx_timer_unit_enum_t unit 		 Unit to be considered for the scan time computation
 * \param[out] none
 *
 *
 *************************************************************************************************************/
void monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit);

 /*!***********************************************************************************************************
 * \fn sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi)
 * \brief Monarch callback called after finding a Monarch Beacon.
 *				
 *				
 *				
 *
 * \param[in]  sfx_u8 rc_bit_mask Bit Mask of the RCx on which the scan has to be executed.    
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
sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi);

/*!******************************************************************
 * \fn uint8_t getScanStatus(void)
 * \brief Get the Monarch Scan status (running or not running).
 *
 * \param[in]  none											                         
 * \param[out] none
 *
 * \retval Scan status (1 if it's running, 0 if it's not running)
 *******************************************************************/
extern uint8_t getScanStatus(void);

/*!******************************************************************
 * \fn void setScanStatus(uint8_t scanStatus);
 * \brief Set Monarch Scan status.
 *
 * \param[in]  scanStatus											 Scan status                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
extern void setScanStatus(uint8_t scanStatus);

#endif

