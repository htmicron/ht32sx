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
 * \file HT_P2P_s2lp_timer.h
 * \brief Configuration and management of S2-LP Timers.
 * \author HT Micron Advanced R&D and
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_TIMER_H__
#define __HT_P2P_TIMER_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Defines  ------------------------------------------------------------------*/

#define SET_INFINITE_RX_TIMEOUT()     S2LPTimerSetRxTimerCounter(0)

#define IS_RX_TIMEOUT_STOP_CONDITION(COND)  ( COND == NO_TIMEOUT_STOP || \
                                                COND == TIMEOUT_ALWAYS_STOPPED || \
                                                COND == RSSI_ABOVE_THRESHOLD || \
                                                COND == SQI_ABOVE_THRESHOLD || \
                                                COND == PQI_ABOVE_THRESHOLD || \
                                                COND == RSSI_AND_SQI_ABOVE_THRESHOLD || \
                                                COND == RSSI_AND_PQI_ABOVE_THRESHOLD || \
                                                COND == SQI_AND_PQI_ABOVE_THRESHOLD || \
                                                COND == ALL_ABOVE_THRESHOLD || \
                                                COND == RSSI_OR_SQI_ABOVE_THRESHOLD || \
                                                COND == RSSI_OR_PQI_ABOVE_THRESHOLD || \
                                                COND == SQI_OR_PQI_ABOVE_THRESHOLD || \
                                                COND == ANY_ABOVE_THRESHOLD )

/* Typedef  ------------------------------------------------------------------*/

/**
 * \enum RxTimeoutStopCondition
 * \brief All the possible RX timeout stop conditions enumeration.
 */
typedef enum {
     NO_TIMEOUT_STOP = 0x00,                /*!< Timeout never stopped */
     PQI_ABOVE_THRESHOLD = 0x01,            /*!< Timeout stopped on PQI above threshold */
     SQI_ABOVE_THRESHOLD = 0x02,            /*!< Timeout stopped on SQI above threshold */
     SQI_AND_PQI_ABOVE_THRESHOLD = 0x03,    /*!< Timeout stopped on both SQI and PQI above threshold */
     RSSI_ABOVE_THRESHOLD = 0x04,           /*!< Timeout stopped on RSSI above threshold */
     RSSI_AND_PQI_ABOVE_THRESHOLD = 0x05,   /*!< Timeout stopped on both RSSI and PQI above threshold */
     RSSI_AND_SQI_ABOVE_THRESHOLD = 0x06,   /*!< Timeout stopped on both RSSI and SQI above threshold */
     ALL_ABOVE_THRESHOLD = 0x07,            /*!< Timeout stopped only if RSSI, SQI and PQI are above threshold */
     TIMEOUT_ALWAYS_STOPPED = 0x08,         /*!< Timeout always stopped (default) */
     SQI_OR_PQI_ABOVE_THRESHOLD = 0x0B,     /*!< Timeout stopped if one between SQI or PQI are above threshold */
     RSSI_OR_PQI_ABOVE_THRESHOLD = 0x0D,    /*!< Timeout stopped if one between RSSI or PQI are above threshold */
     RSSI_OR_SQI_ABOVE_THRESHOLD = 0x0E,    /*!< Timeout stopped if one between RSSI or SQI are above threshold */
     ANY_ABOVE_THRESHOLD = 0x0F             /*!< Timeout stopped if one among RSSI, SQI or SQI are above threshold */
} RxTimeoutStopCondition;

/* Functions  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn void S2LPTimerSetRxTimerMs(float fDesiredMsec)
 * \brief Sets RX timer in ms.
 *
 * \param[in] float fDesiredMsec							Time in ms.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPTimerSetRxTimerMs(float fDesiredMsec);

/*!******************************************************************
 * \fn void S2LPTimerSetRxTimerStopCondition(RxTimeoutStopCondition xStopCondition)
 * \brief Set the RX timeout stop conditions.
 *
 * \param[in] RxTimeoutStopCondition xStopCondition			New stop condition.
 *         													This parameter can be any value of @ref RxTimeoutStopCondition.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPTimerSetRxTimerStopCondition(RxTimeoutStopCondition xStopCondition);

/*!******************************************************************
 * \fn void S2LPTimerSetRxTimerCounter(uint8_t cCounter)
 * \brief Set the RX timeout timer counter. If it is equal to 0 the timeout is infinite.
 *
 * \param[in] uint8_t cCounter								Value for the timer counter.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPTimerSetRxTimerCounter(uint8_t cCounter);

/*!******************************************************************
 * \fn void S2LPTimerSetRxTimerUs(uint32_t lDesiredUsec)
 * \brief Set the RX timeout timer counter and prescaler from the desired value in ms. it is possible to fix the RX_Timeout to
 *         a minimum value of 50.417us to a maximum value of about 3.28 s.
 *
 * \param[in] uint32_t lDesiredUsec							Desired timer value
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPTimerSetRxTimerUs(uint32_t lDesiredUsec);

/*!******************************************************************
 * \fn void S2LPTimerComputeRxTimerRegValues(uint32_t lDesiredUsec , uint8_t* pcCounter , uint8_t* pcPrescaler)
 * \brief Computes the values of the rx_timeout timer counter and prescaler from the user time expressed in millisecond.
 *         The prescaler and the counter values are computed maintaining the prescaler value as
 *         small as possible in order to obtain the best resolution, and in the meantime minimizing the error.
 *
 * \param[in] uint32_t lDesiredUsec							Rx_timeout in microsecs.
 * \param[in] uint8_t* pcCounter							Pointer to the variable in which the value for
 * 																the rx_timeout counter has to be stored.
 * \param[in] uint8_t* pcPrescaler							Pointer to the variable in which the value for the
 * 																rx_timeout prescaler has to be stored.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPTimerComputeRxTimerRegValues(uint32_t lDesiredUsec , uint8_t* pcCounter , uint8_t* pcPrescaler);

#endif /* __HT_P2P_S2LP_TIMER_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
