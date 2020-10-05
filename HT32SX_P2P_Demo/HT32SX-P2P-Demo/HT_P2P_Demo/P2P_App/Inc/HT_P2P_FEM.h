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
 * \file HT_P2P_FEM.h
 * \brief S2LP commands.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_FEM_H__
#define __HT_P2P_FEM_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"

/* Defines  ------------------------------------------------------------------*/
#define S2LP_GPIO_DIG_OUT_TX_RX_MODE_VALUE	    0x90		/*!< TX or RX mode indicator (to enable an external range extender) */
#define S2LP_GPIO_DIG_OUT_RX_STATE_VALUE		0x50		/*!< RX state indication: "1" when demodulator is ON */
#define S2LP_GPIO_DIG_OUT_TX_STATE_VALUE		0x28		/*!< TX state indication: "1" when S2LP1 is passing in the TX state */

#define S2LP_GPIO_MODE_DIGITAL_INPUT_VALUE      0x01		/*!< Digital Input on GPIO */
#define S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP_VALUE	0x02		/*!< Digital Output on GPIO (low current) */
#define S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP_VALUE	0x03		/*!< Digital Output on GPIO (high current) */

#define HIGH_WORD                               0x9A		/*!< Sets GPIO to high logical level */
#define LOW_WORD                                0xA2		/*!< Sets GPIO to low logical level */

/* Typedef -----------------------------------------------------------*/

/**
 * \enum PA_OperationType
 * \brief Structure to manage External PA.
 */
typedef enum {
  PA_SHUTDOWN     = 0x00,									/*!< PA shutdown state */
  PA_TX_BYPASS    = 0x01,									/*!< PA TX Bypass state */
  PA_TX     	  = 0x02,									/*!< PA TX state */
  PA_RX     	  = 0x03,									/*!< PA RX state */
} PA_OperationType;

/*!******************************************************************
 * \fn void Config_RangeExt(PA_OperationType operation)
 * \brief Config external PA.
 *
 * \param[in]  PA_OperationType operation				PA state.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void Config_RangeExt(PA_OperationType operation);

/*!******************************************************************
 * \fn void FEM_Init(void)
 * \brief Init PA.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void FEM_Init(void);

#endif /* __HT_P2P_FEM_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
