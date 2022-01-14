/*!
 * \file mcu_api_stm32.c
 * \brief Sigfox MCU functions
 * \author  R&D HT Micron,
					H�ndrick Bataglin Gon�alves, 
					Vilson Petry, 
					STMicroelectronics
 * \version 1.0
 * \date Sept 2, 2019
 *
 */

#include "ST_Sigfox_Init.h"
#include "SDK_UTILS_Flash.h"
#include "tim.h"
#include "sigfox_types.h"
#include "sigfox_api.h"
#include "retriever_api.h"
#include "st_rf_api.h"
#include "st_mcu_api.h"

ST_SFX_ERR ST_Sigfox_Init(NVM_BoardDataType *sfxConfig, uint8_t openAfterInit)
{
	ST_SFX_ERR ret_err = ST_SFX_ERR_NONE;

	/* Macro that defines and initializes the nvmconfig structure */
	INIT_NVM_CONFIG(nvmConfig);

	/* Sigfox Credentials Management */
	nvmConfig.nvmType = NVM_TYPE_FLASH;
	nvmConfig.sfxDataAddress = (uint32_t)FLASH_USER_START_ADDR; /* Set here the address for 'NVM sigfox data' management */
	nvmConfig.boardDataAddress = (uint32_t)FLASH_BOARD_START_ADDR; /* Set here the address for 'NVM board data' management */

	/* Configure the NVM_API */
	SetNVMInitial(&nvmConfig);

	/* Retrieve Sigfox info from FLASH */
	if(enc_utils_retrieve_data_from_flash(sfxConfig) != 0)
		ret_err = ST_SFX_ERR_CREDENTIALS;
	return ret_err;
}

void ST_Sigfox_SetPublicKey(uint8_t en)
{
	enc_utils_set_public_key(en);
}
