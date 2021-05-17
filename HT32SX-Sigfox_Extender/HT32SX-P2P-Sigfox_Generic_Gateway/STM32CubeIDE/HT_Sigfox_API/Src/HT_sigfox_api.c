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
	ST_SFX_ERR open_err = ST_SFX_ERR_NONE;

	switch(RCZ){
	case RCZ1:
		ST_RF_API_reduce_output_power(RCZ1_OUTPUT_POWER);
		open_err = St_Sigfox_Open_RCZ(RCZ1);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
	case RCZ2:
		ST_RF_API_reduce_output_power(RCZ2_OUTPUT_POWER);
		open_err = St_Sigfox_Open_RCZ(RCZ2);
		HT_API_switchPa(1);
		HT_API_setSmpsVoltageAction(2);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
	case RCZ3:
		open_err = St_Sigfox_Open_RCZ(RCZ3);
		ST_RF_API_reduce_output_power(RCZ3_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
	case RCZ4:
		open_err = St_Sigfox_Open_RCZ(RCZ4);
		ST_RF_API_reduce_output_power(RCZ4_OUTPUT_POWER);
		HT_API_switchPa(1);
		HT_API_setSmpsVoltageAction(2);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
	case RCZ5:
		open_err = St_Sigfox_Open_RCZ(RCZ5);
		ST_RF_API_reduce_output_power(RCZ5_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
	case RCZ6:
		open_err = St_Sigfox_Open_RCZ(RCZ6);
		ST_RF_API_reduce_output_power(RCZ6_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
	case RCZ7:
		open_err = St_Sigfox_Open_RCZ(RCZ7);
		ST_RF_API_reduce_output_power(RCZ7_OUTPUT_POWER);
		HT_API_switchPa(0);
		HT_API_setSmpsVoltageAction(7);

		if(open_err != 0)
			printf("Open rcz error: %X\n", open_err);

		break;
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
