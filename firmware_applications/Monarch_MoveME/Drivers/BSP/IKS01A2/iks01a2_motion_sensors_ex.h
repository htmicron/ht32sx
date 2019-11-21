/**
 ******************************************************************************
 * @file    iks01a2_motion_sensors_ex.h
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of extended functions needed to manage the motion sensors
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef IKS01A2_MOTION_SENSOR_EX_H
#define IKS01A2_MOTION_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks01a2_motion_sensors.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A2 IKS01A2
 * @{
 */

/** @addtogroup IKS01A2_MOTION_SENSOR_EX IKS01A2 MOTION SENSOR EX
 * @{
 */

/** @defgroup IKS01A2_MOTION_SENSOR_EX_Exported_Types IKS01A2 MOTION SENSOR EX Exported Types
 * @{
 */

typedef enum
{
  IKS01A2_MOTION_SENSOR_INT1_PIN = 0,
  IKS01A2_MOTION_SENSOR_INT2_PIN
} IKS01A2_MOTION_SENSOR_IntPin_t;

typedef struct
{
  unsigned int FreeFallStatus : 1;
  unsigned int TapStatus : 1;
  unsigned int DoubleTapStatus : 1;
  unsigned int WakeUpStatus : 1;
  unsigned int StepStatus : 1;
  unsigned int TiltStatus : 1;
  unsigned int D6DOrientationStatus : 1;
  unsigned int SleepStatus : 1;
} IKS01A2_MOTION_SENSOR_Event_Status_t;

/**
 * @}
 */

/** @addtogroup IKS01A2_MOTION_SENSOR_EX_Exported_Functions IKS01A2 MOTION SENSOR EX Exported Functions
 * @{
 */

int32_t IKS01A2_MOTION_SENSOR_Enable_HP_Filter(uint32_t Instance, uint8_t CutOff);
int32_t IKS01A2_MOTION_SENSOR_Set_INT1_DRDY(uint32_t Instance,uint8_t Status);
int32_t IKS01A2_MOTION_SENSOR_Set_INT2_DRDY(uint32_t Instance,uint8_t Status);
int32_t IKS01A2_MOTION_SENSOR_DRDY_Set_Mode(uint32_t Instance,uint8_t Status);
int32_t IKS01A2_MOTION_SENSOR_Clear_DRDY(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Enable_6D_Orientation(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_6D_Orientation(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Set_6D_Orientation_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS01A2_MOTION_SENSOR_Enable_Free_Fall_Detection(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_Free_Fall_Detection(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Set_Free_Fall_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS01A2_MOTION_SENSOR_Set_Free_Fall_Duration(uint32_t Instance, uint8_t Duration);
int32_t IKS01A2_MOTION_SENSOR_Enable_Pedometer(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Disable_Pedometer(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Set_Pedometer_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS01A2_MOTION_SENSOR_Reset_Step_Counter(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Enable_Single_Tap_Detection(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_Single_Tap_Detection(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Enable_Double_Tap_Detection(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_Double_Tap_Detection(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Set_Tap_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS01A2_MOTION_SENSOR_Set_Tap_Shock_Time(uint32_t Instance, uint8_t Time);
int32_t IKS01A2_MOTION_SENSOR_Set_Tap_Quiet_Time(uint32_t Instance, uint8_t Time);
int32_t IKS01A2_MOTION_SENSOR_Set_Tap_Duration_Time(uint32_t Instance, uint8_t Time);
int32_t IKS01A2_MOTION_SENSOR_Enable_Tilt_Detection(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_Tilt_Detection(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Enable_Wake_Up_Detection(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_Wake_Up_Detection(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Set_Wake_Up_Threshold(uint32_t Instance, uint8_t Threshold);
int32_t IKS01A2_MOTION_SENSOR_Set_Wake_Up_Duration(uint32_t Instance, uint8_t Duration);
int32_t IKS01A2_MOTION_SENSOR_Enable_Inactivity_Detection(uint32_t Instance, IKS01A2_MOTION_SENSOR_IntPin_t IntPin);
int32_t IKS01A2_MOTION_SENSOR_Disable_Inactivity_Detection(uint32_t Instance);
int32_t IKS01A2_MOTION_SENSOR_Set_Sleep_Duration(uint32_t Instance, uint8_t Duration);
int32_t IKS01A2_MOTION_SENSOR_Get_Event_Status(uint32_t Instance, IKS01A2_MOTION_SENSOR_Event_Status_t *Status);
int32_t IKS01A2_MOTION_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS01A2_MOTION_SENSOR_Get_6D_Orientation_XL(uint32_t Instance, uint8_t *xl);
int32_t IKS01A2_MOTION_SENSOR_Get_6D_Orientation_XH(uint32_t Instance, uint8_t *xh);
int32_t IKS01A2_MOTION_SENSOR_Get_6D_Orientation_YL(uint32_t Instance, uint8_t *yl);
int32_t IKS01A2_MOTION_SENSOR_Get_6D_Orientation_YH(uint32_t Instance, uint8_t *yh);
int32_t IKS01A2_MOTION_SENSOR_Get_6D_Orientation_ZL(uint32_t Instance, uint8_t *zl);
int32_t IKS01A2_MOTION_SENSOR_Get_6D_Orientation_ZH(uint32_t Instance, uint8_t *zh);
int32_t IKS01A2_MOTION_SENSOR_Get_Step_Count(uint32_t Instance, uint16_t *StepCount);
int32_t IKS01A2_MOTION_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS01A2_MOTION_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint16_t *NumSamples);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Get_Full_Status(uint32_t Instance, uint8_t *Status);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_Decimation(uint32_t Instance, uint32_t Function, uint8_t Decimation);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_ODR_Value(uint32_t Instance, float Odr);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_INT1_FIFO_Full(uint32_t Instance, uint8_t Status);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_INT2_FIFO_Full(uint32_t Instance, uint8_t Status);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint16_t Watermark);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_Stop_On_Fth(uint32_t Instance, uint8_t Status);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Get_Pattern(uint32_t Instance, uint16_t *Pattern);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Get_Axis(uint32_t Instance, uint32_t Function, int32_t *Data);
int32_t IKS01A2_MOTION_SENSOR_FIFO_Get_Data_Word(uint32_t Instance,uint32_t Function,int16_t *Data);
int32_t IKS01A2_MOTION_SENSOR_Set_SelfTest(uint32_t Instance, uint32_t Function, uint8_t Status);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* IKS01A2_MOTION_SENSOR_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
