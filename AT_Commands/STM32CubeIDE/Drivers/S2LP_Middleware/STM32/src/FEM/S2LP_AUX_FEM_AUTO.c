/**
 * @file    S2LP_EVAL_FEM_AUTO.c
 * @author  LowPower RF BU - AMG
 * @version V1.0.0
 * @date    Oct 1, 2018
 * @brief   Platform dependent file for external front end module (aka power amplifier)
 *          management. This file is only useful for ST eval kits.
 *          The evaluation kit supported are: STEVAL-FKI868V2 and STEVAL-FKI915V1
 * @details
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
 */  

/* Includes ------------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_AUX_FEM.h"
#include "S2LP_AUX_UTILS.h"
#include "sigfox_types.h"
#include "spi.h"
#include "st_rf_api.h"

#include "usart.h"

/** @addtogroup S2LP_EVAL_FEM_AUTO
 * @{
 */

/**
 * In order to correctly configure the PA there's need to set the PA Level
 * in terms of dBM and the MaxIndex for ramping.
 * You can define this functions in your own library 
 * or include ST's S2LP Library that defines this functions in S2LP_Radio.c
 */
__weak void S2LPRadioSetPALeveldBm(uint8_t cIndex, int32_t wPowerdBm){};
__weak void S2LPRadioSetPALevelMaxIndex(uint8_t cIndex){};

#define S2LP_GPIO_DIG_OUT_TX_RX_MODE	        0x90	/*!< TX or RX mode indicator (to enable an external range extender) */
#define S2LP_GPIO_DIG_OUT_RX_STATE		0x50	/*!< RX state indication: "1" when demodulator is ON */
#define S2LP_GPIO_DIG_OUT_TX_STATE		0x28	/*!< TX state indication: "1" when S2LP1 is passing in the TX state */

#define S2LP_GPIO_MODE_DIGITAL_INPUT            0x01	/*!< Digital Input on GPIO */
#define S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP	0x02	/*!< Digital Output on GPIO (low current) */
#define S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP	0x03	/*!< Digital Output on GPIO (high current) */

#define HIGH                                    0x9A
#define LOW                                     0xA2

uint8_t tmp[3] = {0x00,0x00,0x00};

/**
 * @brief  Front End Module initialization function.
 * This function automatically sets the FEM according to the information stored in the device EEPROM.
 * This function can be redefined for special needs.
 * @param  None
 * @retval None
 */
void FEM_Init()
{

	/* Configuration of S2LP GPIO to control external PA signal CSD, CPS, CTX */
	uint8_t tmp[]={
			(uint8_t)S2LP_GPIO_DIG_OUT_TX_RX_MODE | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
			(uint8_t)S2LP_GPIO_DIG_OUT_RX_STATE   | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP,
			(uint8_t)S2LP_GPIO_DIG_OUT_TX_STATE   | (uint8_t)S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP
	};

	S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);
}

/**
 * @brief  Front End Module Operation function.
 * This function configures the PA according to the desired status.
 * This function can be redefined for special needs.
 * @param  operation Specifies the operation to perform.
 *         This parameter can be one of following parameters:
 *         @arg FEM_SHUTDOWN: Shutdown PA
 *         @arg FEM_TX_BYPASS: Bypass the PA in TX
 *         @arg FEM_TX: TX mode
 *         @arg FEM_RX: RX mode
 * @retval None
 */
void FEM_Operation(FEM_OperationType operation)
{
	FEM_Init();

	switch (operation)
	{
	case FEM_SHUTDOWN:
		tmp[0]=LOW;
		tmp[1]=LOW;
		tmp[2]=LOW;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;
	case FEM_TX_BYPASS:
		printf("TX\n");

		tmp[0]=HIGH;
		tmp[1]=LOW;
		tmp[2]=HIGH;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;
	case FEM_TX:
		printf("TX\n");

		tmp[0]=HIGH;
		tmp[1]=HIGH;
		tmp[2]=HIGH;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;
	case FEM_RX:
		//printf("RX\n");

		tmp[0]=LOW;
		tmp[1]=LOW;
		tmp[2]=HIGH;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);

		break;
	default:
		/* !!!Error */

		tmp[0]=LOW;
		tmp[1]=LOW;
		tmp[2]=LOW;

		S2LPSpiWriteRegisters(0x00, sizeof(tmp), tmp);
		break;
	}
}

/**
 * @}
 */

/**
 * @}
 */


/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
