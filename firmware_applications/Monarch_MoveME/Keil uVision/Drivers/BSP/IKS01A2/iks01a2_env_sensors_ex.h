/**
 ******************************************************************************
 * @file    iks01a2_env_sensors_ex.h
 * @author  MEMS Software Solutions Team
 * @brief   This file provides a set of extended functions needed to manage the environmental sensors
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
#ifndef IKS01A2_ENV_SENSOR_EX_H
#define IKS01A2_ENV_SENSOR_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iks01a2_env_sensors.h"

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup IKS01A2 IKS01A2
 * @{
 */

/** @addtogroup IKS01A2_ENV_SENSOR_EX IKS01A2_ENV_SENSOR_EX
 * @{
 */

/** @addtogroup IKS01A2_ENV_SENSOR_EX_Exported_Functions IKS01A2_ENV_SENSOR_EX Exported Functions
 * @{
 */

int32_t IKS01A2_ENV_SENSOR_FIFO_Get_Data(uint32_t Instance, float *Press, float *Temp);
int32_t IKS01A2_ENV_SENSOR_FIFO_Get_Fth_Status(uint32_t Instance, uint8_t *Status);
int32_t IKS01A2_ENV_SENSOR_FIFO_Get_Full_Status(uint32_t Instance, uint8_t *Status);
int32_t IKS01A2_ENV_SENSOR_FIFO_Get_Num_Samples(uint32_t Instance, uint8_t *NumSamples);
int32_t IKS01A2_ENV_SENSOR_FIFO_Get_Ovr_Status(uint32_t Instance, uint8_t *Status);
int32_t IKS01A2_ENV_SENSOR_FIFO_Reset_Interrupt(uint32_t Instance, uint8_t interrupt);
int32_t IKS01A2_ENV_SENSOR_FIFO_Set_Interrupt(uint32_t Instance, uint8_t Interrupt);
int32_t IKS01A2_ENV_SENSOR_FIFO_Set_Mode(uint32_t Instance, uint8_t Mode);
int32_t IKS01A2_ENV_SENSOR_FIFO_Set_Watermark_Level(uint32_t Instance, uint8_t Watermark);
int32_t IKS01A2_ENV_SENSOR_FIFO_Usage(uint32_t Instance, uint8_t Status);
int32_t IKS01A2_ENV_SENSOR_Get_DRDY_Status(uint32_t Instance, uint32_t Function, uint8_t *Status);
int32_t IKS01A2_ENV_SENSOR_Read_Register(uint32_t Instance, uint8_t Reg, uint8_t *Data);
int32_t IKS01A2_ENV_SENSOR_Write_Register(uint32_t Instance, uint8_t Reg, uint8_t Data);
int32_t IKS01A2_ENV_SENSOR_Set_One_Shot(uint32_t Instance);
int32_t IKS01A2_ENV_SENSOR_Get_One_Shot_Status(uint32_t Instance, uint8_t *Status);

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

#endif /* IKS01A2_ENV_SENSOR_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
