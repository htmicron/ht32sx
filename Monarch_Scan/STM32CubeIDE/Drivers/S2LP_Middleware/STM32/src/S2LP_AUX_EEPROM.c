/**
 * @file    S2LP_EVAL_EEPROM_DMA.c
 * @author  LowPower RF BU - AMG
 * @version 1.0.0
 * @date    December, 2018
 * @brief   S2LP EVAL EEPROM management
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

/* Includes -----------------------------------------------------------------*/
#include "S2LP_Middleware_Config.h"
#include "S2LP_AUX_EEPROM.h"


/** @defgroup S2LP_EEPROM_Functions
 * @{
 */

uint8_t EepromRead(uint16_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer) {

#if EEPROM_YES
	uint8_t cmd[3];
	uint8_t dummy[255];
	cmd[0] = EEPROM_CMD_READ;

	for(uint8_t k=0; k<2; k++) {
		cmd[k+1] = (uint8_t)(nAddress>>((1-k)*8));
	}

	/* Wait the end of a previous write operation */
	EepromWaitEndWriteOperation();

	/* Put the SPI chip select low to start the transaction */
	EepromSPICSLow();

	/* Write the header bytes and read the status bytes */
	HAL_SPI_TransmitReceive(&EepromSpiHandle, cmd, dummy, 3, 1000);

	/* Read the registers according to the number of bytes */
	HAL_SPI_TransmitReceive(&EepromSpiHandle, dummy, pcBuffer, cNbBytes, 1000);

	/* Put the SPI chip select high to end the transaction */
	EepromSPICSHigh();
#endif
	return 0;
}


/**
 * @}
 */

/**
 * @}
 */

/******************* (C) COPYRIGHT 2018 STMicroelectronics *****END OF FILE****/
