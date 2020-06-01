/*!
* \file ST_Sigfox_Init.h
* \brief Sigfox MCU functions
* \author  Advanced R&D HT Micron, STMicroelectronics
* \version 1.0
* \date Sept 2, 2019
*
*/

#include "S2LP_AUX_UTILS.h"
#include "S2LP_Middleware_Config.h"
#include "nvm_api.h"

#ifndef ST_SIGFOX_INIT_H
#define ST_SIGFOX_INIT_H

typedef enum {
  ST_SFX_ERR_NONE = 0,
  ST_SFX_ERR_OPEN = 1,
  ST_SFX_ERR_CREDENTIALS = 2,
  ST_SFX_ERR_OFFSET = 3,
  ST_SFX_ERR_RC_UNKNOWN = 99
} ST_SFX_ERR;


/**
 * @brief  Opens Sigfox Library according to the zone.
 * @param rcz The Radio Zone
 * @retval Returns 0 if ok.
 */
extern ST_SFX_ERR St_Sigfox_Open_RCZ(uint8_t rcz);


/**
 * @brief  Sigfox subsystem main initialization.
 * @param  openAfterInit if 1 opens the library after the initialization process
 * @retval error code
 */
extern ST_SFX_ERR ST_Sigfox_Init(NVM_BoardDataType *sfxConfig, uint8_t openAfterInit);


/**
 * @brief  Function to set or not the device in Public Key mode
 * @param  en Set this flag to 1 to enable Public Key mode. 0 otherwise.
 * @retval None
 */
extern void ST_Sigfox_SetPublicKey(uint8_t en);

#endif
