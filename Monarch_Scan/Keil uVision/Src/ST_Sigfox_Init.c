/*!
 * \file ST_Sigfox_Init.c
 * \brief Sigfox MCU functions
 * \author  Advanced R&D HT Micron, STMicroelectronics
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

ST_SFX_ERR St_Sigfox_Open_RCZ(uint8_t rcz)
{
	ST_SFX_ERR open_err = ST_SFX_ERR_NONE;

	switch(rcz)
	{
	case 1:
	{
		/* Turn PA off in RC1/3/5/6 */
		//ST_RF_API_set_pa(0);
		/* RCZ1 - open the SigFox library */
		if(SIGFOX_API_open(&(sfx_rc_t)RC1)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	case 2:
	{
		/* Turn PA off in RC2 and RC4 */
		//ST_RF_API_set_pa(1);
		/* RCZ2 - open the SigFox library */
		if(SIGFOX_API_open(&(sfx_rc_t)RC2)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}

		/* In FCC we can choose the macro channel to use by a 86 bits bitmask
        In this case we use the first 9 macro channels */
		sfx_u32 config_words[3]={1,0,0};

		/* Set the standard configuration with default channel to 1 */
		if(SIGFOX_API_set_std_config(config_words,0)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	case 3:
	{
		volatile uint8_t ret;
		/* Turn PA off in RC1/3/5/6 */
		//ST_RF_API_set_pa(0);
		ret=SIGFOX_API_open(&(sfx_rc_t)RC3C);
		/* RCZ4 - open the SigFox library */
		if(ret!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}

		/* In FCC we can choose the macro channel to use by a 86 bits bitmask
        In this case we use 9 consecutive macro channels starting from 63 (920.8MHz) */
		sfx_u32 config_words[3]=RC3C_CONFIG;

		/* Set the standard configuration with default channel to 63 */
		if(SIGFOX_API_set_std_config(config_words,0)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	case 4:
	{
		volatile uint8_t ret;
		/* Turn PA off in RC2 and RC4 */
		//ST_RF_API_set_pa(1);

		ret=SIGFOX_API_open(&(sfx_rc_t)RC4);
		/* RCZ4 - open the SigFox library */
		if(ret!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}

		/* In FCC we can choose the macro channel to use by a 86 bits bitmask
        In this case we use 9 consecutive macro channels starting from 63 (920.8MHz) */
		sfx_u32 config_words[3]={0,0x40000000,0};

		/* Set the standard configuration with default channel to 63 */
		if(SIGFOX_API_set_std_config(config_words,1)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	case 5:
	{
		volatile uint8_t ret;
		/* Turn PA off in RC1/3/5/6 */
		//ST_RF_API_set_pa(0);
		ret=SIGFOX_API_open(&(sfx_rc_t)RC5);
		/* RCZ5 - open the SigFox library */
		if(ret!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}

		/* In FCC we can choose the macro channel to use by a 86 bits bitmask
        In this case we use 9 consecutive macro channels starting from 63 (920.8MHz) */
		sfx_u32 config_words[3]=RC5_CONFIG;

		/* Set the standard configuration with default channel to 63 */
		if(SIGFOX_API_set_std_config(config_words,0)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	case 6:
	{
		/* Turn PA off in RC1/3/5/6 */
		//ST_RF_API_set_pa(0);
		/* RCZ6 - open the SigFox library */
		if(SIGFOX_API_open(&(sfx_rc_t)RC6)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	case 7:
	{
		/* Turn PA off in RC1/3/5/6/7 */
		//ST_RF_API_set_pa(0);
		/* RCZ7 - open the SigFox library */
		if(SIGFOX_API_open(&(sfx_rc_t)RC7)!=0)
		{
			/* Stuck in case of error */
			open_err = ST_SFX_ERR_OPEN;
		}
		break;
	}
	default:
	{
		/* Stuck the application for a out of range number */
		open_err = ST_SFX_ERR_RC_UNKNOWN;
		break;
	}
	}
	return open_err;
}

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
