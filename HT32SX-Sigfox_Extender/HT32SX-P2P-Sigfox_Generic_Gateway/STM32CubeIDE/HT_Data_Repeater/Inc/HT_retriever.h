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
 * \file HT_retriever.h
 * \brief Retriever api for P2P+Sigfox application.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 1.0
 * \date April 22, 2021
 */

#ifndef __HT_RETRIEVER_H__
#define __HT_RETRIEVER_H__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void HT_RET_GetSeqNumber(uint32_t arg,  uint8_t *data)
 * \brief Get sequence number in a uint8_t array.
 *
 * \param[in]  uint32_t arg					Sequence number.
 * \param[out] uint8_t *data				Sequence number after increment.
 *
 * \retval none
 *******************************************************************/
void HT_RET_GetSeqNumber(uint32_t arg,  uint8_t *data);

/*!******************************************************************
 * \fn uint32_t HT_RET_ConvertSeqNumber(uint8_t *data)
 * \brief Converts sequence number from uint8_t array to uint32_t.
 *
 * \param[in]  uint8_t *data				Current sequence number.
 * \param[out] none
 *
 * \retval Sequence number converted to a uint32_t.
 *******************************************************************/
uint32_t HT_RET_ConvertSeqNumber(uint8_t *data);

#endif /* __HT_RETRIEVER_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
