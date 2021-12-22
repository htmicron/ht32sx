/**
  ******************************************************************************
  * @file    crc.h
  * @brief   This file contains all the function prototypes for
  *          the crc.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC_H__
#define __CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CRC_HandleTypeDef hcrc;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_CRC_Init(void);

/* USER CODE BEGIN Prototypes */

/*!******************************************************************
 * \fn uint32_t HT_CRC_CalculateCRC(uint8_t *input, uint32_t len)
 * \brief Calculates CRC32 of an array.
 *
 * \param[in]  uint8_t *input				Input array.
 * \param[in]  uint32_t len					Array length.
 * \param[out] none.
 *
 * \retval CRC calculated.
 *******************************************************************/
uint32_t HT_CRC_CalculateCRC(uint8_t *input, uint32_t len);

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __CRC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
