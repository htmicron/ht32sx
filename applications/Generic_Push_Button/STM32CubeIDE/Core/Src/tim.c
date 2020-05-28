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

/* USER CODE END 0 */

TIM_HandleTypeDef htim6;

/* TIM6 init function */
void MX_TIM6_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 64-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspInit 0 */

  /* USER CODE END TIM6_MspInit 0 */
    /* TIM6 clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* TIM6 interrupt Init */
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN TIM6_MspInit 1 */

  /* USER CODE END TIM6_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM6)
  {
  /* USER CODE BEGIN TIM6_MspDeInit 0 */

  /* USER CODE END TIM6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM6_CLK_DISABLE();

    /* TIM6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  /* USER CODE BEGIN TIM6_MspDeInit 1 */

  /* USER CODE END TIM6_MspDeInit 1 */
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
  } else if(xTim==TIM6) {
    __HAL_RCC_TIM6_CLK_ENABLE();
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
  } else if(xTim == TIM6) {
    HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);
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
 // notify_end=1;
	setNotifyEndFlag(1);
	
	rtcHandler = getRtcHandler();
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(rtcHandler, RTC_FLAG_WUTF);
  __HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

  //next_rtc_wakeup=0;
	setNextRtcWakeUpFlag(0);
	
  SdkEvalTimersTimConfig(&Tim2_Handler,16000-1,65535-1);
  __HAL_TIM_DISABLE_IT(&Tim2_Handler, TIM_IT_UPDATE);
  HAL_NVIC_DisableIRQ(TIM2_IRQn);
  HAL_TIM_Base_Start(&Tim2_Handler);

  HAL_RTCEx_SetWakeUpTimer_IT(rtcHandler,((getRtcPrescFlag()*duration_ms)/1000),RTC_WAKEUPCLOCK_RTCCLK_DIV16);
  while(!getRtcIrqFlag());
  c=Tim2_Handler.Instance->CNT;
  //rtc_irq=0;
  setRtcIrqFlag(0);
	HAL_TIM_Base_Stop(&Tim2_Handler);

  rtc_presc=(duration_ms*getRtcPrescFlag())/c;
	setRtcPrescFlag(rtc_presc);
}

sfx_u8 MCU_API_timer_start_carrier_sense(sfx_u16 time_duration_in_ms)
{
  uint32_t rtc_wup_tick, next_rtc_wakeup_tick;
	uint32_t next_rtc_wakeup = 0;
	
  //sprintf(debug, "MCU_API_timer_start_carrier_sense IN (rtc_in_use=%d)\n\r",getRtcInUseFlag());
	//printUart(debug);
	
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
    //notify_end = 1;
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(rtcHandler, RTC_FLAG_WUTF);
    __HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
    //n_intermediate_tim_irq=0;
    setNIntermediateTimIrqFlag(0);
		//rtc_in_use=1;
    setRtcInUseFlag(1);
		//rtc_in_use_for_cs=1;
    setRtcInUseForCsFlag(1);
		//rtc_wup_tick = time_duration_in_ms/1000*rtc_presc;
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
  //rtc_irq=0;
	setRtcIrqFlag(0);

	rtcHandler = getRtcHandler();
	
  if (time_duration_in_s == DECAY_LEVEL)
    time_duration_in_s += 2; /* In order to make RX-PROTOCOL End of Listening Window working */

  //sprintf(debug, "MCU_API_timer_start IN %d\n\r", (uint32_t)time_duration_in_s);
	//printUart(debug);
	
  Configure_RTC_Clock();
	setNotifyEndFlag(1);
  //notify_end = 1;
  __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(rtcHandler, RTC_FLAG_WUTF);
  __HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
  //n_intermediate_tim_irq = 0;
  setNIntermediateTimIrqFlag(0);
	//rtc_in_use=1;
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

  //next_rtc_wakeup=next_rtc_wakeup_tick/rtc_presc;
	next_rtc_wakeup=(next_rtc_wakeup_tick/getRtcPrescFlag());
	setNextRtcWakeUpFlag(next_rtc_wakeup);
	
  //sprintf(debug, "MCU_API_timer_start OUT %d\n\r", next_rtc_wakeup);
  //printUart(debug);
	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_timer_stop(void)
{
  //printUart("MCU_API_timer_stop IN\n\r");
	rtcHandler = getRtcHandler();
	
  HAL_RTCEx_DeactivateWakeUpTimer(rtcHandler);
  //rtc_in_use=0;
  setRtcInUseFlag(0);
	//printUart("MCU_API_timer_stop OUT\n\r");
  return SFX_ERR_NONE;
}

sfx_u8 MCU_API_timer_stop_carrier_sense(void)
{
  //printUart("MCU_API_timer_stop_carrier_sense IN\n\r");

	rtcHandler = getRtcHandler();
	
  if(getRtcInUseForCsFlag())
  {
    HAL_RTCEx_DeactivateWakeUpTimer(rtcHandler);
    //rtc_in_use=0;
		setRtcInUseFlag(0);
    //rtc_in_use_for_cs=0;
		setRtcInUseForCsFlag(0);
  }
  else
  {
    SdkEvalTimersState(&Tim2_Handler, DISABLE);
  }
  carrier_sense_tim_started = 0;

  //printUart("MCU_API_timer_stop_carrier_sense OUT\n\r");

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

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
