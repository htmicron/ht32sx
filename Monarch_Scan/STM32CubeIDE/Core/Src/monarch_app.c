/*!
 * \file monarch_app.c
 * \brief Monarch Application HT32SX iMCP SiP Sigfox
 * \author HT Micron Advanced R&D
 * \version 1.0
 * \date November 20, 2019
 *
 * This file defines the Monarch Scan Application.
 */

#include "monarch_app.h"

volatile uint8_t scan = 1;
volatile uint8_t rc_bit = 0;

#ifdef DOWNLINK_FLAG
static uint8_t downlink_request = 1;
#else
static uint8_t downlink_request = 0;
#endif


uint8_t HT_RF_API_GETrcbit(void){
	return rc_bit;
}

void closeSigfoxLib(void) {
	sfx_error_t err;

	err = SIGFOX_API_close();
	printf("Close api error: %X", err);
}

void sendFrameRCZ(rc_mask RCZ) {
	uint8_t customer_data;
	uint8_t customer_resp[8];
	sfx_error_t err;

	rc_bit = 0;
	customer_data = RCZ;

	err=SIGFOX_API_send_frame(&customer_data, sizeof(customer_data), customer_resp, 2, downlink_request);
	printf("Send Frame error: %X\n", err);
}

sfx_u8 callback2(sfx_u8 rc_bit_mask, sfx_s16 rssi) {

	switch (rc_bit_mask) {
	case 0x01:  //RC1

		printf("Detected RC1!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ1;

		break;
	case 0x02: //RC2

		printf("Detected RC2!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ2;

		break;
	case 0x04:  //RC3a

		printf("Detected RC3!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ3;

		break;
	case 0x08:  //RC4

		printf("Detected RC4!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ4;

		break;
	case 0x10: //RC5

		printf("Detected RC5!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ5;

		break;
	case 0x20:  //RC6

		printf("Detected RC6!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ6;

		break;
	case 0x40:  //RC7

		printf("Detected RC7!!!\n");
		printf("RSSI %d\n", rssi);
		rc_bit = RCZ7;

		break;

	default:
		printf("No RCZ Detected!\n");
		scan = 1;
		break;
	}

	return 1;
}


void monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit){
	sfx_error_t err;

	err = SIGFOX_MONARCH_API_execute_rc_scan(rc_capabilities_bit_mask, timer, unit, callback2);
	printf("\nScan Monarch error: 0x%02X \n", err);
}

uint8_t getScanStatus(void) {
	return scan;
}

void setScanStatus(uint8_t scanStatus) {
	scan = scanStatus;
}
