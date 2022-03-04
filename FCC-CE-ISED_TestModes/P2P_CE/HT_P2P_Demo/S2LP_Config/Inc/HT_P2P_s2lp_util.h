/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 */

/*!
 * \file HT_P2P_s2lp_util.h
 * \briefIdentification functions for S2-LP DK.
 * \author HT Micron Advanced R&D and
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_UTIL_H__
#define __HT_P2P_S2LP_UTIL_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "HT_P2P_FEM.h"

/* Functions  -----------------------------------------------------------*/

/*!******************************************************************
 * \fn void S2LPManagementSetBand(uint8_t value)
 * \brief S2LP set band.
 *
 * \param[in] uint8_t value								Desired band value.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPManagementSetBand(uint8_t value);

/*!******************************************************************
 * \fn uint8_t S2LPManagementGetBand(void)
 * \brief S2LP get band.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
uint8_t S2LPManagementGetBand(void);

/*!******************************************************************
 * \fn void S2LPManagementSetOffset(int32_t value)
 * \brief Set module offset.
 *
 * \param[in] int32_t value								Offset value.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPManagementSetOffset(int32_t value);

/*!******************************************************************
 * \fn int32_t S2LPManagementGetOffset(void
 * \brief Get module offset.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
int32_t S2LPManagementGetOffset(void);

#endif /* __HT_P2P_S2LP_UTIL_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
