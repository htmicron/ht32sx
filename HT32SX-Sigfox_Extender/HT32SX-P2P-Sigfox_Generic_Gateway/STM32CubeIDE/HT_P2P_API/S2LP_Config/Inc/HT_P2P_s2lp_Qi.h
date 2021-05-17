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
 * \file HT_P2P_s2lp_Qi.h
 * \brief Configuration and management of S2LP QI.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_QI_H__
#define __HT_P2P_S2LP_QI_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Defines  ------------------------------------------------------------------*/
#define IS_RSSI_MODE(MODE)            (MODE<=3)
#define IS_RSSI_DBM(VAL)              ((VAL>=-146) && (VAL<=-2))
#define IS_RSSI_FLT(VAL)              (VAL<=15)
#define IS_PQI_LVL_CHECK(VAL)         (VAL<=15)

#define S2LPRadioRegToRssidBm(VAL)    (VAL - 146)
#define S2LPRadioRssidBmToReg(VAL)    (uint8_t)(VAL+146)

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void S2LPRadioSetRssiThreshdBm(int32_t wRssiThrehsold)
 * \brief Set the RSSI threshold in dBm.
 *
 * \param[in] int32_t wRssiThrehsold 						Rssi threshold in dBm.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPRadioSetRssiThreshdBm(int32_t wRssiThrehsold);

/*!******************************************************************
 * \fn void S2LPRadioSetPqiCheck(uint8_t cPqiLevel)
 * \brief Set the PQI check.
 *
 * \param[in] uint8_t cPqiLevel								PQI_LEVEL.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPRadioSetPqiCheck(uint8_t cPqiLevel);

#endif /* __HT_P2P_S2LP_QI_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
