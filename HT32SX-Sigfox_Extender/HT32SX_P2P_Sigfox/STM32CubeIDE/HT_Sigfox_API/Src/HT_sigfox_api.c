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
