/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "adc.h"

/**
 * @brief  Absolute value macro.
 * @param  x: Value on which apply the abs function.
 * @retval None
 */
#define ABS(x)  (x>0?x:-x)

#define DECAY_LEVEL 34

volatile uint32_t lSystickCounter = 0;

static volatile uint8_t notify_end = 0;
static volatile uint8_t carrier_sense_tim_started = 0;
static volatile uint32_t n_intermediate_tim_irq = 0;

static TIM_HandleTypeDef  Tim2_Handler={.Instance=TIM2};
RTC_HandleTypeDef *rtcHandler;

uint32_t vref = 0;

/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim21;

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}
/* TIM21 init function */
void MX_TIM21_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim21.Instance = TIM21;
  htim21.Init.Prescaler = 16000-1;
  htim21.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim21.Init.Period = 50-1;
  htim21.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim21.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim21) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim21, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim21, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM21)
  {
  /* USER CODE BEGIN TIM21_MspInit 0 */

  /* USER CODE END TIM21_MspInit 0 */
    /* TIM21 clock enable */
    __HAL_RCC_TIM21_CLK_ENABLE();

    /* TIM21 interrupt Init */
    HAL_NVIC_SetPriority(TIM21_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM21_IRQn);
  /* USER CODE BEGIN TIM21_MspInit 1 */

  /* USER CODE END TIM21_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM21)
  {
  /* USER CODE BEGIN TIM21_MspDeInit 0 */

  /* USER CODE END TIM21_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM21_CLK_DISABLE();

    /* TIM21 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM21_IRQn);
  /* USER CODE BEGIN TIM21_MspDeInit 1 */

  /* USER CODE END TIM21_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

/**
 * @brief  Computes two integer value prescaler and period such that Cycles = prescaler * period.
 * @param  lCycles the specified cycles for the desired timer value.
 * @param  pnPrescaler prescaler factor.
 * @param  pnCounter period factor.
 * @retval None.
 */
void SdkEvalTimersFindFactors(uint32_t lCycles, uint16_t *pnPrescaler, uint16_t *pnCounter)
{
	uint16_t b0;
	uint16_t a0;
	long err, err_min=lCycles;

	*pnPrescaler = a0 = ((lCycles-1)/0xffff) + 1;
	*pnCounter = b0 = lCycles / *pnPrescaler;

	for (; *pnPrescaler < 0xffff-1; (*pnPrescaler)++) {
		*pnCounter = lCycles / *pnPrescaler;
		err = (long)*pnPrescaler * (long)*pnCounter - (long)lCycles;
		if (ABS(err) > (*pnPrescaler / 2)) {
			(*pnCounter)++;
			err = (long)*pnPrescaler * (long)*pnCounter - (long)lCycles;
		}
		if (ABS(err) < ABS(err_min)) {
			err_min = err;
			a0 = *pnPrescaler;
			b0 = *pnCounter;
			if (err == 0) break;
		}
	}

	*pnPrescaler = a0;
	*pnCounter = b0;
}

/**
 * @brief  Configures the specified timer to raise an interrupt every time the counter
 *         reaches the nPeriod value counting with a prescaler of nPrescaler.
 * @note   The specified timer is configured but not enabled.
 * @param  TIM_TimeBaseStructure Timer Handler of the timer to be set.
 *          This parameter can be a pointer to @ref TIM_HandleTypeDef .
 * @param  nPrescaler prescaler factor.
 * @param  nPeriod period factor.
 * @retval None.
 */
void SdkEvalTimersTimConfig(TIM_HandleTypeDef* TIM_TimeBaseStructure, uint16_t nPrescaler, uint16_t nPeriod)
{
	TIM_TypeDef *xTim=TIM_TimeBaseStructure->Instance;

	/* disable the timer */
	__HAL_TIM_DISABLE(TIM_TimeBaseStructure);

	/* Configure the timer in update mode */
	__HAL_TIM_DISABLE_IT(TIM_TimeBaseStructure, TIM_IT_UPDATE);

	/* put the timer clock on */
	if(xTim == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
	}

	/* Time base configuration */
	TIM_TimeBaseStructure->Init.Prescaler         = nPrescaler;
	TIM_TimeBaseStructure->Init.Period            = nPeriod;
	TIM_TimeBaseStructure->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
	TIM_TimeBaseStructure->Init.CounterMode       = TIM_COUNTERMODE_UP;

	/* Init the time base structure */
	HAL_TIM_Base_Init(TIM_TimeBaseStructure);


	/* NVIC configuration */
	if(xTim == TIM2) {
		HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}

	/* Clear the timer pending bit */
	__HAL_TIM_CLEAR_FLAG(TIM_TimeBaseStructure, TIM_FLAG_UPDATE);
}

void ST_MCU_API_TimerCalibration(uint16_t duration_ms)
{
	TIM_HandleTypeDef  Tim2_Handler={.Instance=TIM2};
	uint16_t c;
	uint16_t rtc_presc;
	Configure_RTC_Clock();

	setNotifyEndFlag(1);

	rtcHandler = getRtcHandler();
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(rtcHandler, RTC_FLAG_WUTF);
	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

	setNextRtcWakeUpFlag(0);

	SdkEvalTimersTimConfig(&Tim2_Handler,16000-1,65535-1);
	__HAL_TIM_DISABLE_IT(&Tim2_Handler, TIM_IT_UPDATE);
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	HAL_TIM_Base_Start(&Tim2_Handler);

	HAL_RTCEx_SetWakeUpTimer_IT(rtcHandler,((getRtcPrescFlag()*duration_ms)/1000),RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	while(!getRtcIrqFlag());
	c=Tim2_Handler.Instance->CNT;

	setRtcIrqFlag(0);
	HAL_TIM_Base_Stop(&Tim2_Handler);

	rtc_presc=(duration_ms*getRtcPrescFlag())/c;
	setRtcPrescFlag(rtc_presc);
}

sfx_u8 MCU_API_timer_start_carrier_sense(sfx_u16 time_duration_in_ms)
{
	uint32_t rtc_wup_tick, next_rtc_wakeup_tick;
	uint32_t next_rtc_wakeup = 0;

	carrier_sense_tim_started=1;

	rtcHandler = getRtcHandler(); 

	if(getRtcInUseFlag())
	{
		uint32_t n = ((uint32_t)time_duration_in_ms*16000);
		uint16_t a,b;
		SdkEvalTimersFindFactors(n,&a,&b);
		SdkEvalTimersTimConfig(&Tim2_Handler,a-1,b-1);
		SdkEvalTimersState(&Tim2_Handler, ENABLE);
	}
	else
	{
		Configure_RTC_Clock();
		setNotifyEndFlag(1);
		__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(rtcHandler, RTC_FLAG_WUTF);
		__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
		setNIntermediateTimIrqFlag(0);
		setRtcInUseFlag(1);
		setRtcInUseForCsFlag(1);
		rtc_wup_tick = ((time_duration_in_ms*getRtcPrescFlag())/1000);
		if(rtc_wup_tick>65535) /* Mapped register is 16bit */
		{
			next_rtc_wakeup_tick=rtc_wup_tick-65535;
			rtc_wup_tick=65535;
		}
		else
		{
			next_rtc_wakeup_tick=0;
		}

		next_rtc_wakeup = ((next_rtc_wakeup_tick*1000)/(getRtcPrescFlag()));
		setNextRtcWakeUpFlag(next_rtc_wakeup);

		HAL_RTCEx_SetWakeUpTimer_IT(rtcHandler, rtc_wup_tick, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
	}

	// printUart("MCU_API_timer_start_carrier_sense OUT\n\r");

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_timer_start(sfx_u32 time_duration_in_s)
{
	uint32_t rtc_wup_tick, next_rtc_wakeup_tick;
	uint32_t next_rtc_wakeup = 0;

	ST_RF_API_Timer_CB(TIMER_START); /* To notify the rf_api layer */
	setRtcIrqFlag(0);

	rtcHandler = getRtcHandler();

	if (time_duration_in_s == DECAY_LEVEL)
		time_duration_in_s += 2; /* In order to make RX-PROTOCOL End of Listening Window working */

	Configure_RTC_Clock();
	setNotifyEndFlag(1);
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(rtcHandler, RTC_FLAG_WUTF);
	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
	setNIntermediateTimIrqFlag(0);
	setRtcInUseFlag(1);
	rtc_wup_tick = (time_duration_in_s)*getRtcPrescFlag(); //(time_duration_in_s)*rtc_presc;

	if(rtc_wup_tick>65535)
	{
		next_rtc_wakeup_tick=(rtc_wup_tick)-65535;
		rtc_wup_tick=65535;
	}
	else
	{
		next_rtc_wakeup_tick=0;
	}

	HAL_RTCEx_SetWakeUpTimer_IT(rtcHandler,rtc_wup_tick,RTC_WAKEUPCLOCK_RTCCLK_DIV16);

	next_rtc_wakeup=(next_rtc_wakeup_tick/getRtcPrescFlag());
	setNextRtcWakeUpFlag(next_rtc_wakeup);

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_timer_stop(void)
{
	rtcHandler = getRtcHandler();

	HAL_RTCEx_DeactivateWakeUpTimer(rtcHandler);
	setRtcInUseFlag(0);
	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_timer_stop_carrier_sense(void)
{

	rtcHandler = getRtcHandler();

	if(getRtcInUseForCsFlag())
	{
		HAL_RTCEx_DeactivateWakeUpTimer(rtcHandler);
		setRtcInUseFlag(0);;
		setRtcInUseForCsFlag(0);
	}
	else
	{
		SdkEvalTimersState(&Tim2_Handler, DISABLE);
	}
	carrier_sense_tim_started = 0;

	return SFX_ERR_NONE;

}

sfx_u8 MCU_API_timer_wait_for_end(void)
{
	//printUart("MCU_API_timer_wait_for_end IN\n\r");

	while(!getRtcIrqFlag())//(!(next_rtc_wakeup==0 || rtc_irq==1))
	{
		ST_MCU_API_WaitForInterrupt();
	}
	//rtc_irq=0;
	setRtcIrqFlag(0);
	// printUart("MCU_API_timer_wait_for_end OUT\n\r");
	return SFX_ERR_NONE;
}

void ST_MCU_API_WaitForInterrupt(void)
{
#ifndef DEBUG
	if(getLowPowerFlag() && (!carrier_sense_tim_started))
	{
		//setGpioLowPower();

		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);

		SystemClock_Config();

		//setGpioRestore();
	}
#endif

	if(getS2lpIrqRaisedFlag())
	{
		ST_RF_API_S2LP_IRQ_CB();
		//s2lp_irq_raised = 0;
		setS2lpIrqRaisedFlag(0);
	}
}

/* Set flags */
void setNotifyEndFlag(uint8_t notifyEnd) {
	notify_end = notifyEnd;
}

void setNIntermediateTimIrqFlag(uint8_t nIntermediateTimeIrq) {
	n_intermediate_tim_irq = nIntermediateTimeIrq;
}

/* Get flags */
uint8_t getNotifyEndFlag(void) {
	return notify_end;
}

uint32_t getNIntermediateTimIrqFlag(void) {
	return n_intermediate_tim_irq;
}

#ifdef DOWNLINK_FLAG

uint32_t HT_getVddTx(void) {
	return vref;
}

void HT_setVddTx(uint32_t vdd) {
	vref = vdd;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	uint32_t aux;
	uint32_t ad;

	if(htim->Instance == TIM21) {
		aux = HT_getVrefData();
		vref = aux < vref || !vref ? aux : vref;
		ad = HT_getTemperatureAD();
	}
}

#endif

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
