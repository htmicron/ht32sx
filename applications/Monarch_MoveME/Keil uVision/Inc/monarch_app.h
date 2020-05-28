/**
  * 
  * Copyright (c) 2019 HT Micron Semicondutors S.A.
	* Licensed under the Apache License, Version 2.0 (the "License");
	* you may not use this file except in compliance with the License.
	* You may obtain a copy of the License at
	* http://www.apache.org/licenses/LICENSE-2.0
	* Unless required by applicable law or agreed to in writing, software
	* distributed under the License is distributed on an "AS IS" BASIS,
	* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	* See the License for the specific language governing permissions and
	* limitations under the License.
  *
  */

/*
*	\file monarch_app.h
* 	\brief This is the header of the monarch custom API. This example had some modifications in order to ensure the desired custom application behavior.
*	\author  R&D team HT Micron Semicondutors.
*	\version 1.0 Accelerometer POC Alternative Version
*	\date November 21, 2019
*/

#ifndef MONARCH_APP_H
#define MONARCH_APP_H

#include "main.h"

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
	RCZ7,
} rc_mask;


/*!******************************************************************
 * \typedef scan_st_t
 * \brief Scan region status
 * \val SCAN_IDLE
 * 		System is waiting to perform a scan.
 * \val SCAN_DONE
 * 		System has performed a scan.
 *******************************************************************/

typedef enum {
   SCAN_IDLE,
   SCAN_DONE,
} scan_st_t;

/*!******************************************************************
 * \fn void configRegion(void)
 * \brief Configures the device using the RCZ value defined in 
 * 				RCZ_REGION found at main.h.
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval none	
 *******************************************************************/
 
void configRegion(void);

/*!******************************************************************
 * \fn void closeSigfoxLib(void)
 * \brief This function closes the library (Free the allocated 
 *				memory of SIGFOX_API_open and close RF).
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
 
void closeSigfoxLib(void);

/*!******************************************************************
 * \fn void sendFrameRCZ(rc_mask RCZ)
 * \brief Send a frame to the Sigfox network using the RCZ found 
 *				after Monarch Scan.
 *
 * \param[in]  rc_mask RCZ											 Radio zone                        
 * \param[out] none
 * 
 * \retval none	
 *******************************************************************/
 
 void sendFrameRCZ(rc_mask RCZ);
 
 /*!***********************************************************************************************************
 * \fn void MonarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit)
 * \brief Executes a network scan to detect a Sigfox Beacon. 
 *				It returns 0 if a valid SigFox network is found and the RC enum value corresponding to 
 *				the beacon found and its RSSI level. The scan is executed during the 
 *				specific timer/unit time.
 *
 * \param[in]  sfx_u8 rc_capabilities_bit_mask Bit Mask of the RCx on which the scan has to be executed.    
 *
 *						--------------------------------------------------------- 
 *          	| Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
 *          	---------------------------------------------------------
 *          	|  -   |  RC7 | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *          	---------------------------------------------------------  
 *						 sfx_u16 timer						 Scan duration value (with the unit parameter information).
 *						 sfx_timer_unit_enum_t unit 		 Unit to be considered for the scan time computation
 * \param[out] none
 * 
 * \retval none	
 *************************************************************************************************************/
 
void MonarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit);

/*!******************************************************************
 * \fn sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi)
 * \brief Monarch callback called after finding Monarch Beacon.
 *
 * \param[in]  sfx_u8 rc_capabilities_bit_mask Bit Mask of the RCx on which the scan has to be executed.    
 *
 *						--------------------------------------------------------- 
 *          	| Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
 *          	---------------------------------------------------------
 *          	|  -   |  RC7 | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *          	---------------------------------------------------------  
 *							
 *							sfx_s16 rssi											Signal rssi.
 *
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                         						No error
 *******************************************************************/

sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi);

/*!******************************************************************
 * \fn uint8_t getScanStatus(void)
 * \brief Get scan status variable value.
 *
 * \param[in]  none
 * \param[out] uint8_t Current Scan Status value
 *
 * \retval SCAN_IDLE:			         				System is waiting to perform a scan.
 * \retval SCAN_DONE:                    	Scan has been performed.
 *******************************************************************/

extern scan_st_t getScanStatus(void);

/*!******************************************************************
 * \fn void setScanStatus(uint8_t scanStatus)
 * \brief Set scan status variable value.
 *
 * \param[in]  none
 * \param[out] uint8_t Current Scan Status value
 *
 * \retval none								
 *******************************************************************/

extern void setScanStatus(scan_st_t scanStatus);

#endif

