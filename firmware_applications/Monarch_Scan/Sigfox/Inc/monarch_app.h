
#ifndef MONARCH_APP_H
#define MONARCH_APP_H

#include "main.h"

/*!******************************************************************
 * \fn void configRegion(void)
 * \brief Configures the device using the RCZ found after Monarch Scan. 
					Call the functions St_Sigfox_Open_RCZ, to execute these
					configurations and sendFrameRCZ.
 *
 * \param[in]  none                        
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                         No error
 * \retval MCU_ERR_API_FREE:                     Free error
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
 * \retval SFX_ERR_NONE:                         No error
 * \retval MCU_ERR_API_FREE:                     Free error
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
 * \retval SFX_ERR_NONE:                         No error
 * \retval MCU_ERR_API_FREE:                     Free error
 *******************************************************************/
 
 void sendFrameRCZ(rc_mask RCZ);
 
 /*!***********************************************************************************************************
 * \fn void MonarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit)
 * \brief Executes a scan of the air to detect a Sigfox Beacon. 
 *				It will return 0, if success and the RC enum value corresponding to 
 *				the beacon found and its RSSI level. The scan is executed during the 
 *				pecific timer/unit time.
 *
 * \param[in]  sfx_u8 rc_capabilities_bit_mask Bit Mask of the RCx on which the scan has to be executed.    
 *
 *						--------------------------------------------------------- 
 *          	| Bit7 | Bit6 | Bit5 | Bit4 | Bit3 | Bit2 | Bit1 | Bit0 |
 *          	---------------------------------------------------------
 *          	|  -   |  -   | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *          	---------------------------------------------------------  
 *						 sfx_u16 timer									 Scan duration value (with the unit parameter information).
 *						 sfx_timer_unit_enum_t unit 		 Unit to be considered for the scan time computation
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                         No error
 * \retval MCU_ERR_API_FREE:                     Free error
 *
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
 *          	|  -   |  -   | RC6  | RC5  | RC4  | RC3  | RC2  |  RC1 |
 *          	---------------------------------------------------------  
 *							
 *							sfx_s16 rssi											Signal rssi.
 *
 * \param[out] none
 *
 * \retval SFX_ERR_NONE:                         No error
 * \retval MCU_ERR_API_FREE:                     Free error
 *******************************************************************/

sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi);

extern uint8_t getScanStatus(void);

extern void setScanStatus(uint8_t scanStatus);

#endif

