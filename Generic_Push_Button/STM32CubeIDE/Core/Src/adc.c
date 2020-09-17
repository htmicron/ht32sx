/**
  ******************************************************************************
  * File Name          : ADC.c
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"

/* USER CODE BEGIN 0 */
uint32_t ADC_raw = 0;
uint32_t Vdd;
/* USER CODE END 0 */

ADC_HandleTypeDef hadc;

/* ADC init function */
void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_COMP_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

uint32_t HT_getVrefData(void) {
	uint32_t ad = 0;
	uint32_t vdd = 0;

	ADC_ChannelConfTypeDef sConfig;

	sConfig.Channel = ADC_CHANNEL_VREFINT;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	HAL_ADC_Start(&hadc);
	while(HAL_ADC_PollForConversion(&hadc,ADC_CHANNEL_VREFINT)!=0);
	ad = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	vdd = 3000*(*VREFINT_CAL_ADDR)/ad;

	return vdd;
}

uint32_t HT_getTemperatureAD(void) {
	uint32_t temp;

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE;

	if ((ADC1->CR & ADC_CR_ADEN) != 0)
		ADC1->CR &= (uint32_t)(~ADC_CR_ADEN);

	ADC1->CR |= ADC_CR_ADCAL;

	while ((ADC1->ISR & ADC_ISR_EOCAL) == 0);

	ADC1->ISR |= ADC_ISR_EOCAL;
	ADC1->ISR |= ADC_ISR_ADRDY;
	ADC1->CR |= ADC_CR_ADEN;

	if ((ADC1->CFGR1 & ADC_CFGR1_AUTOFF) == 0)
		while ((ADC1->ISR & ADC_ISR_ADRDY) == 0);

	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	ADC1->CHSELR = ADC_CHSELR_CHSEL18;
	ADC1->SMPR |= ADC_SMPR_SMP;
	ADC->CCR |= ADC_CCR_TSEN;

	HAL_ADC_Start(&hadc);
	while(HAL_ADC_PollForConversion(&hadc,ADC_CHANNEL_TEMPSENSOR)!=0);
	temp = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	return temp;
}

int32_t HT_computeTemperature(uint32_t measure, uint32_t vdd) {
	int32_t temperature;
	uint32_t vref = vdd/10;

	temperature = ((measure * vref / VDD_CALIB) - (int32_t) *TEMP30_CAL_ADDR );
	temperature = temperature * (int32_t)(130 - 30);
	temperature = temperature / (int32_t)(*TEMP130_CAL_ADDR - *TEMP30_CAL_ADDR);
	temperature += 30;

	return temperature;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
