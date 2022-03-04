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
 * \file HT_mcu_api.h
 * \brief MCU API for HT32SX iMCP SiP Sigfox.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 17, 2020
 *
 * This file defines the MCU API made for the the AT Commands application.
 */

#ifndef _HT_MCU_API_H_
#define _HT_MCU_API_H_

/* Functions ------------------------------------------------------------------*/

/*!***********************************************************************************************************
 * \fn void HT_McuApi_enterStopMode(void)
 * \brief Starts MCU stop mode.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_enterStopMode(void);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_enableUsartWkp(void)
 * \brief Configures USART to wake up from Stop or Deep Sleep Mode .
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_enableUsartWkp(void);

/*!***********************************************************************************************************
 * \fn HT_McuApi_enterGpioLowPower(void)
 * \brief Set up all GPIOs to analog in order to reduce the current consumption.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_enterGpioLowPower(void);

/*!***********************************************************************************************************
 * \fn extern uint8_t HT_McuApi_getDeepSleepModeFlag(void)
 * \brief Get the deepSleepFlag state to control the state machine.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
extern uint8_t HT_McuApi_getDeepSleepModeFlag(void);

/*!***********************************************************************************************************
 * \fn extern void HT_McuApi_setDeepSleepModeFlag(uint8_t flag)
 * \brief Set the deepSleepFlag in order to controlling the state machine.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
extern void HT_McuApi_setDeepSleepModeFlag(uint8_t flag);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_configPeripherals(void)
 * \brief Reconfigure all peripherals again. It is called after a wake-up event.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_configPeripherals(void);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_softwareReset(void)
 * \brief Resets MCU.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_softwareReset(void);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_enterDeepSleepMode(void)
 * \brief Enters in deep sleep mode (calls stop mode and turn off most of peripherals, keeping on only the USART).
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_enterDeepSleepMode(void);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_switchPa(void)
 * \brief Enable or disable PA (1 enable, 0 disable).
 *
 * \param[in]  uint8_t state				PA state (on/off).
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_switchPa(uint8_t state);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_switchBoost(void)
 * \brief Enable or disable S2LP boost mode (1 enable, 0 disable).
 *
 * \param[in]  uint8_t state				S2LP boost mode state (on/off).
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_switchBoost(uint8_t state);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_reduceOutputPower(int16_t reduce_value)
 * \brief Set a new output power value and save it to flash memory.
 *
 * \param[in]  uint8_t reduce_value				S2LP boost mode state (on/off).
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_reduceOutputPower(int16_t reduce_value);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_setFreqOffset(int32_t offset)
 * \brief Calibration routine to set a new frequency offset value.
 *
 * \param[in]  uint8_t offset				Frequency offset value.
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_setFreqOffset(int32_t offset);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_setRssiOffset(int32_t offset)
 * \brief Calibration routine to set a new RSSI offset value.
 *
 * \param[in]  uint8_t offset				RSSI offset value.
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_setRssiOffset(int32_t offset);

/*!***********************************************************************************************************
 * \fn void HT_McuApi_setLbtOffset(int32_t offset)
 * \brief Calibration routine to set a new LBT offset value.
 *
 * \param[in]  uint8_t offset				LBT offset value.
 * \param[out] none
 *
 * \retval	none
 *
 *************************************************************************************************************/
void HT_McuApi_setLbtOffset(int32_t offset);

#endif /* _HT_MCU_API_H_ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
