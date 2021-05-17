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
 * \file HT_P2P_s2lp_interface.h
 * \brief S2LP interface routines.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_INTERFACE_H__
#define __HT_P2P_S2LP_INTERFACE_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void S2LPInterfaceInit(void)
 * \brief Initialize the S2LP GPIOx according to the specified parameters in the pxGpioInitStruct.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPInterfaceInit(void);

/*!******************************************************************
 * \fn void S2LP_EnableTxIrq(void)
 * \brief Enable TX IRQn on S2LP side.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_EnableTxIrq(void);

/*!******************************************************************
 * \fn void S2LP_EnableRxIrq(void)
 * \brief Enable RX IRQn on S2LP side.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_EnableRxIrq(void);

/*!******************************************************************
 * \fn void S2LP_EnableSQI(void)
 * \brief Enable S2LP SQI.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_EnableSQI(void);

/*!******************************************************************
 * \fn void S2LP_SetRxTimeout(float cRxTimeOut)
 * \brief Set RX Timeout.
 *
 * \param[in] float cRxTimeOut						RX timeout.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_SetRxTimeout(float cRxTimeOut);

/*!******************************************************************
 * \fn void S2LP_SetPayloadlength(uint8_t length)
 * \brief Set payload length.
 *
 * \param[in] uint8_t length						Payload length.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_SetPayloadlength(uint8_t length);

/*!******************************************************************
 * \fn void S2LP_StartTx(uint8_t *buffer, uint8_t size)
 * \brief Start TX sending a frame to anther device.
 *
 * \param[in] uint8_t *buffer						TX buffer.
 * \param[in] uint8_t size							Buffer size.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_StartTx(uint8_t *buffer, uint8_t size);

/*!******************************************************************
 * \fn void S2LP_StartRx(void)
 * \brief Start RX. Put S2LP in RX mode.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_StartRx(void);

/*!******************************************************************
 * \fn void S2LP_PacketConfig(void)
 * \brief Config packet to basic protocol format.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_PacketConfig(void);

/*!******************************************************************
 * \fn void S2LP_GetRxPacket(uint8_t *buffer, uint8_t *cRxData)
 * \brief Get RX packet received from another device.
 *
 * \param[out] uint8_t *buffer					RX buffer.
 * \param[out] uint8_t *cRxData					RX data length.
 *
 * \retval none
 *******************************************************************/
void S2LP_GetRxPacket(uint8_t *buffer, uint8_t *cRxData);

/*!******************************************************************
 * \fn void S2LP_SetDestinationAddress(uint8_t address)
 * \brief Set destination address.
 *
 * \param[in] uint8_t address				Destination address.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LP_SetDestinationAddress(uint8_t address);

#endif /* __HT_P2P_S2LP_INTERFACE_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
