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
 * \file HT_P2P_s2lp_gpio.h
 * \brief Configuration and management of S2-LP GPIO.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_GPIO_H__
#define __HT_P2P_S2LP_GPIO_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "HT_P2P_s2lp_types.h"
#include "HT_P2P_s2lp_regs.h"
#include "gpio.h"
#include "spi.h"

/* Defines ------------------------------------------------------------------*/

#define RADIO_GPIO_0_EXTI_IRQN                   EXTI0_1_IRQn
#define RADIO_GPIO_0_EXTI_IRQ_HANDLER            EXTI0_1_IRQHandler
#define RADIO_GPIO_1_EXTI_IRQN                   EXTI4_15_IRQn
#define RADIO_GPIO_1_EXTI_IRQ_HANDLER            EXTI0_1_IRQHandler
#define RADIO_GPIO_2_EXTI_IRQN                   EXTI0_1_IRQn
#define RADIO_GPIO_2_EXTI_IRQ_HANDLER            EXTI0_1_IRQHandler
#define RADIO_GPIO_3_EXTI_IRQN                   EXTI2_3_IRQn
#define RADIO_GPIO_3_EXTI_IRQ_HANDLER            EXTI2_3_IRQHandler

#define RADIO_GPIO_IRQ		RADIO_GPIO_3
#define RADIO_GPIO_NUMBER    ((uint8_t)5)

#define IS_S2LP_GPIO(PIN)  ((PIN == S2LP_GPIO_0) || \
                              (PIN == S2LP_GPIO_1) || \
                              (PIN == S2LP_GPIO_2) || \
                              (PIN == S2LP_GPIO_3))


#define IS_S2LP_GPIO_MODE(MODE)   ((MODE == S2LP_GPIO_MODE_DIGITAL_INPUT) || \
                                   (MODE == S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP) || \
                                   (MODE == S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP))

#define IS_S2LP_IRQ_LIST(VALUE)   ((VALUE == RX_DATA_READY) || \
                                     (VALUE == RX_DATA_DISC)  || \
                                     (VALUE == TX_DATA_SENT)  || \
                                     (VALUE == MAX_RE_TX_REACH)  || \
                                     (VALUE == CRC_ERROR)  || \
                                     (VALUE == TX_FIFO_ERROR)  || \
                                     (VALUE == RX_FIFO_ERROR)  || \
                                     (VALUE == TX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == TX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == RX_FIFO_ALMOST_FULL)  || \
                                     (VALUE == RX_FIFO_ALMOST_EMPTY)  || \
                                     (VALUE == MAX_BO_CCA_REACH)  || \
                                     (VALUE == VALID_PREAMBLE)  || \
                                     (VALUE == VALID_SYNC)  || \
                                     (VALUE == RSSI_ABOVE_TH)  || \
                                     (VALUE == WKUP_TOUT_LDC)  || \
                                     (VALUE == READY)  || \
                                     (VALUE == STANDBY_DELAYED)  || \
                                     (VALUE == LOW_BATT_LVL)  || \
                                     (VALUE == POR)  || \
                                     (VALUE == BOR)  || \
                                     (VALUE == LOCK)  || \
                                     (VALUE == VCO_CALIBRATION_END)  || \
                                     (VALUE == PA_CALIBRATION_END)  || \
                                     (VALUE == PM_COUNT_EXPIRED)  || \
                                     (VALUE == XO_COUNT_EXPIRED)  || \
                                     (VALUE == TX_START_TIME)  || \
                                     (VALUE == RX_START_TIME)  || \
                                     (VALUE == RX_TIMEOUT)  || \
                                     (VALUE == RX_SNIFF_TIMEOUT)   || \
				                              (VALUE == ALL_IRQ ))

/* Typedef ------------------------------------------------------------------*/

/**
 * \enum RadioGpioPin
 * \brief Specifies MCU GPIO pin enumeration for GPIO.
 */
typedef enum {
	RADIO_GPIO_0     = 0x00, /*!< GPIO_0 selected */
	RADIO_GPIO_1     = 0x01, /*!< GPIO_1 selected */
	RADIO_GPIO_2     = 0x02, /*!< GPIO_2 selected */
	RADIO_GPIO_3     = 0x03, /*!< GPIO_3 selected */
	RADIO_GPIO_SDN   = 0x04, /*!< GPIO_SDN selected */
} RadioGpioPin;

/**
 * \enum S2LPGpioPin
 * \brief S2LP GPIO pin enumeration.
 */
typedef enum {
	S2LP_GPIO_0  = GPIO0_CONF_ADDR, /*!< GPIO_0 selected */
	S2LP_GPIO_1  = GPIO1_CONF_ADDR, /*!< GPIO_1 selected */
	S2LP_GPIO_2  = GPIO2_CONF_ADDR, /*!< GPIO_2 selected */
	S2LP_GPIO_3  = GPIO3_CONF_ADDR  /*!< GPIO_3 selected */
} S2LPGpioPin;

/**
 * \enum S2LPGpioMode
 * \brief S2LP GPIO mode enumeration.
 */
typedef enum {
	S2LP_GPIO_MODE_DIGITAL_INPUT      = 0x01, /*!< Digital Input on GPIO */
	S2LP_GPIO_MODE_DIGITAL_OUTPUT_LP ,        /*!< Digital Output on GPIO (low current) */
	S2LP_GPIO_MODE_DIGITAL_OUTPUT_HP          /*!< Digital Output on GPIO (high current) */
} S2LPGpioMode;

/**
 * \enum S2LPGpioIO
 * \brief S2LP I/O selection enumeration.
 */
typedef enum {
	S2LP_GPIO_DIG_OUT_IRQ                               = 0x00, /*!< nIRQ (Interrupt Request, active low) , default configuration after POR */
	S2LP_GPIO_DIG_OUT_POR_INV                           = 0x08, /*!< POR inverted (active low) */
	S2LP_GPIO_DIG_OUT_WUT_EXP                           = 0x10, /*!< Wake-Up Timer expiration: "1" when WUT has expired */
	S2LP_GPIO_DIG_OUT_LBD                               = 0x18, /*!< Low battery detection: "1" when battery is below threshold setting */
	S2LP_GPIO_DIG_OUT_TX_DATA                           = 0x20, /*!< TX data internal clock output (TX data are sampled on the rising edge of it) */
	S2LP_GPIO_DIG_OUT_TX_STATE                          = 0x28, /*!< TX state indication: "1" when S2LP1 is passing in the TX state */
	S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_EMPTY            = 0x30, /*!< TX/RX FIFO Almost Empty Flag */
	S2LP_GPIO_DIG_OUT_TXRX_FIFO_ALMOST_FULL             = 0x38, /*!< TX/RX FIFO Almost Full Flag */
	S2LP_GPIO_DIG_OUT_RX_DATA                           = 0x40, /*!< RX data output */
	S2LP_GPIO_DIG_OUT_RX_CLOCK                          = 0x48, /*!< RX clock output (recovered from received data) */
	S2LP_GPIO_DIG_OUT_RX_STATE                          = 0x50, /*!< RX state indication: "1" when demodulator is ON */
	S2LP_GPIO_DIG_OUT_NOT_STANDBY_SLEEP                 = 0x58, /*!< VDD when the device is not in SLEEP or STANDBY */
	S2LP_GPIO_DIG_OUT_STANDBY                           = 0x60, /*!< VDD when device is in STANDBY */
	S2LP_GPIO_DIG_OUT_ANTENNA_SWITCH                    = 0x68, /*!< Antenna switch used for antenna diversity  */
	S2LP_GPIO_DIG_OUT_VALID_PREAMBLE                    = 0x70, /*!< Valid Preamble Detected Flag */
	S2LP_GPIO_DIG_OUT_SYNC_DETECTED                     = 0x78, /*!< Sync WordSync Word Detected Flag */
	S2LP_GPIO_DIG_OUT_RSSI_THRESHOLD                    = 0x80, /*!< RSSI above threshold */
	S2LP_GPIO_DIG_OUT_MCU_CLOCK                         = 0x88, /*!< MCU Clock */
	S2LP_GPIO_DIG_OUT_TX_RX_MODE                        = 0x90, /*!< TX or RX mode indicator (to enable an external range extender) */
	S2LP_GPIO_DIG_OUT_VDD                               = 0x98, /*!< VDD (to emulate an additional GPIO of the MCU, programmable by SPI) */
	S2LP_GPIO_DIG_OUT_GND                               = 0xA0, /*!< GND (to emulate an additional GPIO of the MCU, programmable by SPI) */
	S2LP_GPIO_DIG_OUT_SMPS_EXT                          = 0xA8, /*!< External SMPS enable signal (active high) */
	S2LP_GPIO_DIG_OUT_SLEEP                             = 0xB0, /*!< Device in SLEEP (active high) */
	S2LP_GPIO_DIG_OUT_READY                             = 0xB8, /*!< Device in READY (active high) */
	S2LP_GPIO_DIG_OUT_LOCK                              = 0xC0, /*!< Device in LOCK (active high) */
	S2LP_GPIO_DIG_OUT_WAIT_FOR_LOCK_SIG                 = 0xC8, /*!< Device waiting for LOCK (active high) */
	S2LP_GPIO_DIG_OUT_TX_DATA_OOK_SIGNAL                = 0xD0,
	S2LP_GPIO_DIG_OUT_WAIT_FOR_READY2_SIG               = 0xD8,
	S2LP_GPIO_DIG_OUT_WAIT_FOR_TIMER_FOR_PM_SET         = 0xE0,
	S2LP_GPIO_DIG_OUT_WAIT_VCO_CALIBRATION              = 0xE8,
	S2LP_GPIO_DIG_OUT_ENABLE_SYNTH_FULL_CIRCUIT         = 0xF0,

	S2LP_GPIO_DIG_IN_TX_COMMAND                         = 0x00,
	S2LP_GPIO_DIG_IN_RX_COMMAND                         = 0x08,
	S2LP_GPIO_DIG_IN_TX_DATA_INPUT_FOR_DIRECTRF         = 0x10,
	S2LP_GPIO_DIG_IN_DATA_WAKEUP                        = 0x18,
	S2LP_GPIO_DIG_IN_EXT_CLOCK_AT_34_7KHZ               = 0x20
} S2LPGpioIO;

/**
 * \enum IrqList
 * \brief S2LP IRQs enum types.
 */
typedef enum {
	RX_DATA_READY = 0x00000001,           /*!< IRQ: RX data ready */
	RX_DATA_DISC = 0x00000002,            /*!< IRQ: RX data discarded (upon filtering) */
	TX_DATA_SENT = 0x00000004,            /*!< IRQ: TX data sent */
	MAX_RE_TX_REACH = 0x00000008,         /*!< IRQ: Max re-TX reached */
	CRC_ERROR = 0x00000010,               /*!< IRQ: CRC error */
	TX_FIFO_ERROR = 0x00000020,           /*!< IRQ: TX FIFO underflow/overflow error */
	RX_FIFO_ERROR = 0x00000040,           /*!< IRQ: RX FIFO underflow/overflow error */
	TX_FIFO_ALMOST_FULL = 0x00000080,     /*!< IRQ: TX FIFO almost full */
	TX_FIFO_ALMOST_EMPTY = 0x00000100,    /*!< IRQ: TX FIFO almost empty */
	RX_FIFO_ALMOST_FULL = 0x00000200,     /*!< IRQ: RX FIFO almost full */
	RX_FIFO_ALMOST_EMPTY = 0x00000400,    /*!< IRQ: RX FIFO almost empty  */
	MAX_BO_CCA_REACH = 0x00000800,        /*!< IRQ: Max number of back-off during CCA */
	VALID_PREAMBLE = 0x00001000,          /*!< IRQ: Valid preamble detected */
	VALID_SYNC = 0x00002000,              /*!< IRQ: Sync word detected */
	RSSI_ABOVE_TH = 0x00004000,           /*!< IRQ: RSSI above threshold */
	WKUP_TOUT_LDC = 0x00008000,           /*!< IRQ: Wake-up timeout in LDC mode */
	READY = 0x00010000,                   /*!< IRQ: READY state */
	STANDBY_DELAYED = 0x00020000,         /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
	LOW_BATT_LVL = 0x00040000,            /*!< IRQ: Battery level below threshold*/
	POR = 0x00080000,                     /*!< IRQ: Power On Reset */
	BOR = 0x00100000,                     /*!< IRQ: Brown out event (both accurate and inaccurate)*/
	LOCK = 0x00200000,                    /*!< IRQ: LOCK state */
	VCO_CALIBRATION_END = 0x00400000,        /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
	PA_CALIBRATION_END = 0x00800000,        /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
	PM_COUNT_EXPIRED = 0x01000000,        /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
	XO_COUNT_EXPIRED = 0x02000000,        /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
	TX_START_TIME = 0x04000000,	        /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
	RX_START_TIME = 0x08000000,	        /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
	RX_TIMEOUT = 0x10000000,	        /*!< IRQ: RX operation timeout */
	RX_SNIFF_TIMEOUT = 0x20000000,                 /*!< IRQ: RX sniff operation timeout */
	ALL_IRQ = 0x7FFFFFFF			/*!< All the above mentioned IRQs */
} IrqList;

/**
 * \struct S2LPIrqs
 * \brief S2LP available IRQs.
 */
typedef struct {
	SFlagStatus  IRQ_RX_DATA_READY:1;            /*!< IRQ: RX data ready */
	SFlagStatus  IRQ_RX_DATA_DISC:1;             /*!< IRQ: RX data discarded (upon filtering) */
	SFlagStatus  IRQ_TX_DATA_SENT:1;             /*!< IRQ: TX data sent */
	SFlagStatus  IRQ_MAX_RE_TX_REACH:1;          /*!< IRQ: Max re-TX reached */
	SFlagStatus  IRQ_CRC_ERROR:1;                /*!< IRQ: CRC error */
	SFlagStatus  IRQ_TX_FIFO_ERROR:1;            /*!< IRQ: TX FIFO underflow/overflow error */
	SFlagStatus  IRQ_RX_FIFO_ERROR:1;            /*!< IRQ: RX FIFO underflow/overflow error */
	SFlagStatus  IRQ_TX_FIFO_ALMOST_FULL:1;      /*!< IRQ: TX FIFO almost full */

	SFlagStatus  IRQ_TX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: TX FIFO almost empty */
	SFlagStatus  IRQ_RX_FIFO_ALMOST_FULL:1;      /*!< IRQ: RX FIFO almost full */
	SFlagStatus  IRQ_RX_FIFO_ALMOST_EMPTY:1;     /*!< IRQ: RX FIFO almost empty  */
	SFlagStatus  IRQ_MAX_BO_CCA_REACH:1;         /*!< IRQ: Max number of back-off during CCA */
	SFlagStatus  IRQ_VALID_PREAMBLE:1;           /*!< IRQ: Valid preamble detected */
	SFlagStatus  IRQ_VALID_SYNC:1;               /*!< IRQ: Sync word detected */
	SFlagStatus  IRQ_RSSI_ABOVE_TH:1;            /*!< IRQ: RSSI above threshold */
	SFlagStatus  IRQ_WKUP_TOUT_LDC:1;            /*!< IRQ: Wake-up timeout in LDC mode */

	SFlagStatus  IRQ_READY:1;                    /*!< IRQ: READY state */
	SFlagStatus  IRQ_STANDBY_DELAYED:1;          /*!< IRQ: STANDBY state after MCU_CK_CONF_CLOCK_TAIL_X clock cycles */
	SFlagStatus  IRQ_LOW_BATT_LVL:1;             /*!< IRQ: Battery level below threshold*/
	SFlagStatus  IRQ_POR:1;                      /*!< IRQ: Power On Reset */
	SFlagStatus  IRQ_BOR:1;                      /*!< IRQ: Brown out event (both accurate and inaccurate)*/
	SFlagStatus  IRQ_LOCK:1;                     /*!< IRQ: LOCK state */
	SFlagStatus  IRQ_VCO_CALIBRATION_END:1;      /*!< IRQ: End of VCO calibration procedure */
	SFlagStatus  IRQ_PA_CALIBRATION_END:1;       /*!< IRQ: End of PA calibration procedure */

	SFlagStatus  IRQ_PM_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Power Management startup timer expiration (see reg PM_START_COUNTER, 0xB5) */
	SFlagStatus  IRQ_XO_COUNT_EXPIRED:1;         /*!< IRQ: only for debug; Crystal oscillator settling time counter expired */
	SFlagStatus  IRQ_TX_START_TIME:1;            /*!< IRQ: only for debug; TX circuitry startup time; see TX_START_COUNTER */
	SFlagStatus  IRQ_RX_START_TIME:1;            /*!< IRQ: only for debug; RX circuitry startup time; see TX_START_COUNTER */
	SFlagStatus  IRQ_RX_TIMEOUT:1;               /*!< IRQ: RX operation timeout */
	SFlagStatus  IRQ_RX_SNIFF_TIMEOUT:1;         /*!< IRQ: RX sniff opeartion timeout */
	SFlagStatus  :2;                             /*!< Reserved bit */
} S2LPIrqs;

/**
 * \enum OutputLevel
 * \brief S2LP OutputLevel enumeration.
 */
typedef enum {
	LOW = 0,
	HIGH = !LOW
} OutputLevel;

/**
 * \struct SGpioInit
 * \brief S2LP GPIO Init structure definition.
 */
typedef struct {
	S2LPGpioPin xS2LPGpioPin;    /*!< Select the GPIO pins to be configured. @ref S2LPGpioPin */
	S2LPGpioMode xS2LPGpioMode;  /*!< Set the pin operating mode. @ref S2LPGpioMode */
	S2LPGpioIO xS2LPGpioIO;      /*!< Set the I/O selection for the pin. @ref S2LPGpioIO */
} SGpioInit;

/*!******************************************************************
 * \fn void S2LPGpioInit(SGpioInit* pxGpioInitStruct)
 * \brief Initialize the S2LP GPIOx according to the specified parameters in the pxGpioInitStruct.
 *
 * \param[in]  SGpioInit* pxGpioInitStruct				Pointer to a SGpioInit structure that
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPGpioInit(SGpioInit* pxGpioInitStruct);

/*!******************************************************************
 * \fn void S2LPGpioIrqConfig(IrqList xIrq, SFunctionalState xNewState)
 * \brief Enable or disables a specific IRQ.
 *
 * \param[in] IrqList xIrq								IRQ to enable or disable. This parameter can be any value of @ref IrqList.
 * \param[in] SFunctionalState xNewState				new state for the IRQ. This parameter can be: S_ENABLE or S_DISABLE.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPGpioIrqConfig(IrqList xIrq, SFunctionalState xNewState);

/*!******************************************************************
 * \fn void S2LPGpioIrqDeInit(S2LPIrqs* pxIrqInit)
 * \brief Deinit the S2LPIrqs structure setting all the bitfield to 0.
 *         Moreover, it sets the IRQ mask registers to 0x00000000, disabling all IRQs
 *
 * \param[in]  S2LPIrqs* pxIrqInit				Pointer to a variable of type @ref S2LPIrqs, in which all the
 * 												bitfields will be settled to zero.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPGpioIrqDeInit(S2LPIrqs* pxIrqInit);

/*!******************************************************************
 * \fn void RadioGpioInterruptCmd(RadioGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState)
 * \brief Enables or disables the interrupt on GPIO.
 *
 * \param[in] RadioGpioPin xGpi						Specifies the GPIO whose priority shall be changed.
 * 													In app must be M2S_GPIO_3: GPIO_3.
 * \param[in] uint8_t nPreemption					Specifies Preemption Priority.
 * \param[in] uint8_t nSubpriority					Specifies Subgroup Priority.
 * \param[in] FunctionalState xNewState				Specifies the State.
 * 													This parameter can be one of following parameters:
 *											        @arg ENABLE: Interrupt is enabled
 *         											@arg DISABLE: Interrupt is disabled
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void RadioGpioInterruptCmd(RadioGpioPin xGpio, uint8_t nPreemption, uint8_t nSubpriority, FunctionalState xNewState);

/*!******************************************************************
 * \fn void S2LPGpioIrqClearStatus(void)
 * \brief Clear the IRQ status registers.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPGpioIrqClearStatus(void);

/*!******************************************************************
 * \fn void S2LPGpioIrqGetStatus(S2LPIrqs* pxIrqStatus)
 * \brief Fill a pointer to a structure of S2LPIrqs type reading the IRQ_STATUS registers.
 *
 * \param[in] S2LPIrqs* pxIrqStatus					Pointer to a variable of type @ref S2LPIrqs, through which the
 *													user can read the status of all the IRQs. All the bitfields equals to one correspond
 *        											to the raised interrupts. This parameter is a pointer to a S2LPIrqs.
 *         											For example suppose that the XO settling timeout is raised as well as the Sync word
 *         											detection.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPGpioIrqGetStatus(S2LPIrqs* pxIrqStatus);

#endif /* __HT_P2P_S2LP_GPIO_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
