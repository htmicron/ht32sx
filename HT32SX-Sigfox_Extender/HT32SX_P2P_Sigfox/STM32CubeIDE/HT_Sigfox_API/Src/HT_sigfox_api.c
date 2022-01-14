/*
 * HT_sigfox_api.c
 *
 *  Created on: Mar 25, 2021
 *      Author: hendrick.goncalves
 */

#include "HT_sigfox_api.h"

#ifdef DOWNLINK_FLAG
static uint8_t downlink_request = 1;
#else
static uint8_t downlink_request = 0;
#endif

void HT_API_setSmpsVoltageAction(sfx_u8 mode) {
	ST_RF_API_smps(mode);
	printf("Set_smps_voltage %d\n", mode);
}

void HT_API_switchPa(uint8_t state) {

	ST_RF_API_set_pa(state);

	printf("Switch PA: %d\n", state);
}

void HT_API_ConfigRegion(rc_mask RCZ) {
	sfx_error_t error = SFX_ERR_NONE;

	switch(RCZ){
	case RCZ1:
	{
		error = SIGFOX_API_open(&(sfx_rc_t)RC1);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		break;
	}
	case RCZ2:
	{
		sfx_u32 config_words[3]={1,0,0};

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
		HT_API_switchPa(1);
		HT_API_setSmpsVoltageAction(2);


		break;
	}
	case RCZ3:
	{
		sfx_u32 config_words[3] = RC3C_CONFIG;

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
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		break;
	}
	case RCZ4:
	{
		sfx_u32 config_words[3]={0,0x40000000,0};

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
		HT_API_switchPa(1);
		HT_API_setSmpsVoltageAction(2);

		break;
	}
	case RCZ5:
	{
		sfx_u32 config_words[3]=RC5_CONFIG;

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
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		break;
	}
	case RCZ6:
	{
		error = SIGFOX_API_open(&(sfx_rc_t)RC6);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		break;
	}
	case RCZ7:
	{
		error = SIGFOX_API_open(&(sfx_rc_t)RC7);
		if(error) {
			printf("Open rcz error: %X\n", error);
			break;
		}

		ST_RF_API_reduce_output_power(RCZ7_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		break;
	}
	default:
		break;
	}

}

sfx_error_t HT_API_SendFrame(HT_DR_Payload *payload) {
	uint8_t customer_resp[8];
	sfx_error_t err;

	/********** FUNCTION PARAMETERS  ****************************/
	/********** THE LAST ONE IS TO REQUEST DOWNLINK ************/
	/********** 1 - YES, 0 - NO	 ******************************/
	err=SIGFOX_API_send_frame(payload->data, strlen((char *)payload->data), customer_resp, 3, downlink_request);

	if(downlink_request && !err) {
		printf("Customer resp: {");

		for(uint16_t i = 0; i < 7; i++)
			printf("0x%x,", customer_resp[i]);

		printf("0x%x}\n\r", customer_resp[7]);
	}

	printf("\nError Send Frame: %X\n", err);

	return err;
}
