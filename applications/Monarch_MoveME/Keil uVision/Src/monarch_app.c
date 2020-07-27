/**
  * 
  * Legal note: Copyright (c) 2019 HT Micron Semicondutors S.A.
  * 
  * All rights reserved. 
  * 
  * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
  * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
  * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  */

/*!
* \file moarch_app.c
* \brief Monarch user API
* \author  R&D HT Micron
* \version 1.0 POC Accelerometer Alternative Version
* \date November 21, 2019
* \note Please see monarch_app.h header for more details
*
*/

#include "monarch_app.h"

static scan_st_t scan = SCAN_IDLE;
static uint8_t rc_bit = RCZ_REGION; // SigFox region zone configuration.  
static uint8_t rc_found = 0;

void configRegion(void) {
	ST_SFX_ERR open_err = ST_SFX_ERR_NONE;
	
	switch(rc_bit){
		case RCZ1:
			SIGFOX_MONARCH_API_stop_rc_scan();	
			open_err = St_Sigfox_Open_RCZ(RCZ1);
			ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
			
			if(open_err != 0) 
				printf("Open rcz error: %X\n", open_err);
		
			sendFrameRCZ(RCZ1);
			closeSigfoxLib();
			scan = SCAN_IDLE;
		
			break;
		case RCZ2:
			SIGFOX_MONARCH_API_stop_rc_scan();
			open_err = St_Sigfox_Open_RCZ(RCZ2);
			ST_RF_API_reduce_output_power(RCZ2_OUTPUT_POWER);
			
			if(open_err != 0) 
				printf("Open rcz error: %X\n", open_err);
			
			sendFrameRCZ(RCZ2);
			closeSigfoxLib();
			scan = SCAN_IDLE;
			
			break;
		case RCZ3:
			SIGFOX_MONARCH_API_stop_rc_scan();
			open_err = St_Sigfox_Open_RCZ(RCZ3);
			ST_RF_API_reduce_output_power(RCZ3_OUTPUT_POWER);
			
			if(open_err != 0) 
				printf("Open rcz error: %X\n", open_err);
			
			sendFrameRCZ(RCZ3);
			closeSigfoxLib();
			scan = SCAN_IDLE;
		
			break;
		case RCZ4:
			SIGFOX_MONARCH_API_stop_rc_scan();
			open_err = St_Sigfox_Open_RCZ(RCZ4);
			ST_RF_API_reduce_output_power(RCZ4_OUTPUT_POWER);
			
			if(open_err != 0) 
				printf("Open rcz error: %X\n", open_err);
			
			sendFrameRCZ(RCZ4);
			closeSigfoxLib();
			scan = SCAN_IDLE;
		
			break;
		case RCZ5:
			SIGFOX_MONARCH_API_stop_rc_scan();
			open_err = St_Sigfox_Open_RCZ(RCZ5);
			ST_RF_API_reduce_output_power(RCZ5_OUTPUT_POWER);
			
			if(open_err != 0) 
				printf("Open rcz error: %X\n", open_err);
			
			sendFrameRCZ(RCZ5);
			closeSigfoxLib();
			scan = SCAN_IDLE;
		
			break;
		case RCZ6:
			SIGFOX_MONARCH_API_stop_rc_scan();
			open_err = St_Sigfox_Open_RCZ(RCZ6);
			ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		
			if(open_err != 0) 
				printf("Open rcz error: %X\n", open_err);
			
			sendFrameRCZ(RCZ6);
			closeSigfoxLib();
			scan = SCAN_IDLE;
		
			break;
		default:
			break;
	}
	
}

void closeSigfoxLib(void) {
	sfx_error_t err;
	
	err = SIGFOX_API_close();
	if (err != 0){
		printf("Close api error: %X", err);
	}
}

void sendFrameRCZ(rc_mask RCZ) {
	uint8_t customer_data[12];
	uint8_t customer_resp[8];
	sfx_error_t err;
	
	customer_data[0] = RCZ;
	
  err=SIGFOX_API_send_frame(customer_data, 1,customer_resp, 3, 0);
	printf("Send Frame error: %X\n", err);
}

void MonarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit){
  sfx_error_t err;

	err = SIGFOX_MONARCH_API_execute_rc_scan(rc_capabilities_bit_mask, timer, unit, callback);
	if (err != 0){
		printf("\nScan Monarch error: %X \n", err);
	}
}

sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi)
{
	printf("Return rssi %d\r\n", rssi);
	
	for (int i = 0; i < 20; i++)
	{
		printf("\n\r");
	}
	
	printf("d88888b  .d88b.  db    db d8b   db d8888b.    d8888b. d88888b  d888b  d888888b  .d88b.  d8b   db 	db \r");
	printf("88'     .8P  Y8. 88    88 888o  88 88  `8D    88  `8D 88'     88' Y8b   `88'   .8P  Y8. 888o  88 	88 \r");
	printf("88ooo   88    88 88    88 88V8o 88 88   88    88oobY' 88ooooo 88         88    88    88 88V8o 88 	YP \r");
	printf("88~~~   88    88 88    88 88 V8o88 88   88    88`8b   88~~~~~ 88  ooo    88    88    88 88 V8o88 	db \r");
	printf("88      `8b  d8' 88b  d88 88  V888 88  .8D    88 `88. 88.     88. ~8~   .88.   `8b  d8' 88  V888      \r");
	printf("YP       `Y88P'  ~Y8888P' VP   V8P Y8888D'    88   YD Y88888P  Y888P  Y888888P  `Y88P'  VP   V8P 	YP \r");
	
	for (int i = 0; i < 4; i++)
	{
		printf("\n\r");
	}
	
	switch (rc_bit_mask){
		case 0x01:  //RC1
		{
			printf("d8888b.  .o88b.       db \r");
			printf("88  `8D d8P  Y8      o88 \r");
			printf("88oobY' 8P            88 \r");
			printf("88`8b   8b            88 \r");
			printf("88 `88. Y8b  d8       88 \r");
			printf("88   YD  `Y88P'       VP \r");

			rc_found = RCZ1;
					
		}
		break;
		case 0x02: //RC2
		
			printf("d8888b.  .o88b.      .d888b.  \r");
			printf("88  `8D d8P  Y8      VP  `8D \r");
			printf("88oobY' 8P              odD' \r");
			printf("88`8b   8b            .88'   \r");
			printf("88 `88. Y8b  d8      j88.    \r");
			printf("88   YD  `Y88P'      888888D \r");
		
			rc_found = RCZ2;

		break;
		
		case 0x04:  //RC3a
		{
			printf("d8888b.  .o88b.      d8888b. \r");
			printf("88  `8D d8P  Y8      VP  `8D \r");
			printf("88oobY' 8P             oooY'  \r");
			printf("88`8b   8b             ~~~b.  \r");
			printf("88 `88. Y8b  d8      db   8D \r");
			printf("88   YD  `Y88P'      Y8888P' \r");

			rc_found = RCZ3;

		}
		break;
		case 0x08:  //RC4
		{
			printf("d8888b.  .o88b.        j88D  \r");
			printf("88  `8D d8P  Y8       j8~88  \r");
			printf("88oobY' 8P           j8' 88  \r");
			printf("88`8b   8b           V88888D \r");
			printf("88 `88. Y8b  d8          88  \r");
			printf("88   YD  `Y88P'          VP  \r");
			
			rc_found = RCZ4;
			
		}
		break;
		case 0x10: //RC5
		{
			printf("d8888b.  .o88b.        ooooo  \r");
			printf("88  `8D d8P  Y8       8P~~~~ \r ");
			printf("88oobY' 8P           dP      \r ");
			printf("88`8b   8b           V8888b. \r ");
			printf("88 `88. Y8b  d8          `8D  \r ");
			printf("88   YD  `Y88P'      88oobY'  \r ");

			rc_found = RCZ5;
			
		}
		break;
		case 0x20:  //RC6
		{
			printf("d8888b.  .o88b.         dD   \r");
			printf("88  `8D d8P  Y8        d8'   \r");
			printf("88oobY' 8P            d8'    \r");
			printf("88`8b   8b           d8888b. \r");
			printf("88 `88. Y8b  d8      88' `8D \r");
			printf("88   YD  `Y88P'      `8888P  \r");

			rc_found = RCZ6;
	
		}
		break;
		
		default:
			rc_found = 0;
			scan = SCAN_IDLE;
			break;
	}
	
	for (int i = 0; i < 10; i++){
		printf("\n\r");
	}
	
	return 0;
}

scan_st_t getScanStatus(void){
	return scan;
}

void setScanStatus(scan_st_t scanStatus) {
	scan = scanStatus;
}
