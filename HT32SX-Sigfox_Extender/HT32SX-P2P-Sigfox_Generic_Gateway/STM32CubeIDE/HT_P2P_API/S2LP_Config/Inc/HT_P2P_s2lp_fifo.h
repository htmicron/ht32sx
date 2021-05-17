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
 * \file HT_P2P_s2lp_fifo.h
 * \brief Configuration and management of S2-LP Fifo.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_FIFO_H__
#define __HT_P2P_S2LP_FIFO_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "spi.h"

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn uint8_t S2LPFifoReadNumberBytesRxFifo(void)
 * \brief Return the number of elements in the RX FIFO
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval uint8_t					Number of elements in the RX FIFO.
 *******************************************************************/
uint8_t S2LPFifoReadNumberBytesRxFifo(void);

/*!******************************************************************
 * \fn uint8_t S2LPFifoReadNumberBytesTxFifo(void)
 * \brief Return the number of elements in the TX FIFO
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval uint8_t					Number of elements in the TX FIFO.
 *******************************************************************/
uint8_t S2LPFifoReadNumberBytesTxFifo(void);

#endif /* __HT_P2P_S2LP_FIFO_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
