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

static uint8_t scan = 1;
static uint8_t rc_bit = 0; 

#ifdef USE_DOWNLINK_FLAG
static uint8_t downlink_request = 1;
#else
static uint8_t downlink_request = 0;
#endif

void configRegion(void) {
	sfx_error_t error = SFX_ERR_NONE;

	switch(rc_bit){
	case RCZ1:
	{
		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC1);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
		HT_switchPa(0);
		HT_setSmpsVoltageAction(7);

		sendFrameRCZ(RCZ1);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	case RCZ2:
	{
		sfx_u32 config_words[3]={1,0,0};

		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC2);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ2_OUTPUT_POWER);
		HT_switchPa(1);
		HT_setSmpsVoltageAction(2);

		sendFrameRCZ(RCZ2);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	case RCZ3:
	{
		sfx_u32 config_words[3] = RC3C_CONFIG;

		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC3C);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ3_OUTPUT_POWER);
		HT_switchPa(0);
		HT_setSmpsVoltageAction(7);

		sendFrameRCZ(RCZ3);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	case RCZ4:
	{
		sfx_u32 config_words[3]={0,0x40000000,0};

		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC4);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}


		ST_RF_API_reduce_output_power(RCZ4_OUTPUT_POWER);
		HT_switchPa(1);
		HT_setSmpsVoltageAction(2);

		sendFrameRCZ(RCZ4);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	case RCZ5:
	{
		sfx_u32 config_words[3]=RC5_CONFIG;

		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC5);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		error = SIGFOX_API_set_std_config(config_words,0);
		if(error) {
			printf("Set Std Config error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ5_OUTPUT_POWER);
		HT_switchPa(0);
		HT_setSmpsVoltageAction(7);

		sendFrameRCZ(RCZ5);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	case RCZ6:
	{
		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC6);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		HT_switchPa(0);
		HT_setSmpsVoltageAction(7);

		sendFrameRCZ(RCZ6);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	case RCZ7:
	{	
		SIGFOX_MONARCH_API_stop_rc_scan();

		error = SIGFOX_API_open(&(sfx_rc_t)RC7);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ7_OUTPUT_POWER);
		HT_switchPa(0);
		HT_setSmpsVoltageAction(7);

		sendFrameRCZ(RCZ7);
		closeSigfoxLib();
		scan = 1;

		break;
	}
	default:
		break;
	}

}

void HT_switchPa(uint8_t state) {
	ST_RF_API_set_pa(state);
	printf("Switch PA: %d\n", state);
}

void HT_setSmpsVoltageAction(sfx_u8 mode) {
	ST_RF_API_smps(mode);
	printf("Set_smps_voltage %d\n", mode);
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

void monarchScan(sfx_u8 rc_capabilities_bit_mask, sfx_u16 timer, sfx_timer_unit_enum_t unit){
	sfx_error_t err;

	err = SIGFOX_MONARCH_API_execute_rc_scan(rc_capabilities_bit_mask, timer, unit, callback);
	printf("\nScan Monarch error: %X \n", err);
}

sfx_u8 callback(sfx_u8 rc_bit_mask, sfx_s16 rssi) {
	printf("Return rssi %d\r\n", rssi);

	switch (rc_bit_mask) {
	case 0x01:  //RC1

		printf("Detected RC1!!!:\r\n");
		rc_bit = RCZ1;

		break;
	case 0x02: //RC2

		printf("Detected RC2!!!:\r\n");
		rc_bit = RCZ2;

		break;
	case 0x04:  //RC3a

		printf("Detected RC3!!!:\r\n");
		rc_bit = RCZ3;

		break;
	case 0x08:  //RC4

		printf("Detected RC4!!!:\r\n");
		rc_bit = RCZ4;

		break;
	case 0x10: //RC5

		printf("Detected RC5!!!:\r\n");
		rc_bit = RCZ5;

		break;
	case 0x20:  //RC6

		printf("Detected RC6!!!:\r\n");
		rc_bit = RCZ6;

		break;
	case 0x40:  //RC7

		printf("Detected RC7!!!:\r\n");
		rc_bit = RCZ7;

		break;

	default:
		scan = 1;
		break;
	}

	return 1;
}

uint8_t getScanStatus(void) {
	return scan;
}

void setScanStatus(uint8_t scanStatus) {
	scan = scanStatus;
}
