/**
  ******************************************************************************
  * File Name          : ADC.h
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc;

/* USER CODE BEGIN Private defines */
#define TEMP130_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007E))
#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t) 0x1FF8007A))
#define VDD_CALIB ((uint16_t) (300))
#define VDD_APPLI ((uint16_t) (330))

#define VREFINT_CAL_ADDR ((uint16_t *)((uint32_t)0x1FF80078))
/* USER CODE END Private defines */

void MX_ADC_Init(void);

/* USER CODE BEGIN Prototypes */

/*!******************************************************************
 * \fn uint32_t HT_getTemperatureAD(void)
 * \brief Get the digital value converted by ADC.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval 	Digital temperature value.
 *******************************************************************/
uint32_t HT_getTemperatureAD(void);

/*!******************************************************************
 * \fn uint32_t HT_computeTemperature(void)
 * \brief Compute device temperature using the digital temperature value and the VREF value read by ADC.
 *
 * \param[in]  uint32_t measure 		Digital value read by ADC.
 * \param[in] uint32_t vdd				VREF value.
 * \param[out] none
 *
 * \retval Temperature absolute value.
 *******************************************************************/
int32_t HT_computeTemperature(uint32_t measure, uint32_t vdd);

/*!******************************************************************
 * \fn uint32_t HT_getVrefData(void)
 * \brief Read VREF value by ADC peripheral.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval Vref absolute value.
 *******************************************************************/
uint32_t HT_getVrefData(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
