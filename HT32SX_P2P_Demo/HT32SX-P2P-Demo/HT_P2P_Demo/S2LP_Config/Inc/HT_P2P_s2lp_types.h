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
 * \file HT_P2P_s2lp_types.h
 * \brief Header file for S2-LP types.
 * \author HT Micron Advanced R&D and
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_TYPES_H__
#define __HT_P2P_S2LP_TYPES_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "HT_P2P_s2lp_regs.h"

/* Typedef -----------------------------------------------------------*/

/**
 * \enum SFunctionalState
 * \brief S2LP Functional state. Used to enable or disable a specific option.
 */
typedef enum {
  S_DISABLE = 0,
  S_ENABLE = !S_DISABLE
} SFunctionalState;


/**
 * \enum SFlagStatus
 * \brief S2LP Flag status. Used to control the state of a flag.
 */
typedef enum {
  S_RESET = 0,
  S_SET = !S_RESET
} SFlagStatus;

/**
 * \enum SBool
 * \brief Boolean type enumeration.
 */
typedef enum {
  S_FALSE = 0,
  S_TRUE  = !S_FALSE
} SBool;

/**
 * \enum S2LPState
 * \brief S2LP States enumeration.
 */
typedef enum {
  MC_STATE_READY             =0x00,  /*!< READY */
  MC_STATE_SLEEP_NOFIFO      =0x01,  /*!< SLEEP NO FIFO RETENTION */
  MC_STATE_STANDBY           =0x02,  /*!< STANDBY */
  MC_STATE_SLEEP             =0x03,  /*!< SLEEP */
  MC_STATE_LOCKON            =0x0C,  /*!< LOCKON */
  MC_STATE_RX                =0x30,  /*!< RX */
  MC_STATE_LOCK_ST           =0x14,  /*!< LOCK_ST */
  MC_STATE_TX                =0x5C,  /*!< TX */
  MC_STATE_SYNTH_SETUP       =0x50   /*!< SYNTH_SETUP */
} S2LPState;

/**
 * \struct S2LPStatus
 * \brief S2LP Status. This definition represents the single field of the S2LP
 *        status returned on each SPI transaction, equal also to the MC_STATE registers.
 *        This field-oriented structure allows user to address in simple way the single
 *        field of the S2LP status.
 *        The user shall define a variable of S2LPStatus type to access on S2LP status fields.
 *
 *        The fields order in the structure depends on used endianness (little or big
 *        endian). The actual definition is valid ONLY for LITTLE ENDIAN mode. Be sure to
 *        change opportunely the fields order when use a different endianness.
 */
typedef struct {
  uint8_t XO_ON:1;           /*!< XO is operating state */
  S2LPState MC_STATE: 7;     /*!< The state of the Main Controller of S2LP @ref S2LPState */
  uint8_t ERROR_LOCK: 1;     /*!< RCO calibration error */
  uint8_t RX_FIFO_EMPTY: 1;  /*!< RX FIFO is empty */
  uint8_t TX_FIFO_FULL: 1;   /*!< TX FIFO is full */
  uint8_t ANT_SELECT: 1;     /*!< Currently selected antenna */
  uint8_t RCCAL_OK: 1;       /*!< RCO successfully terminated */
  uint8_t : 3;               /*!< This 3 bits field are reserved and equal to 2 */
}S2LPStatus;


/**
 * @defgroup Types_Exported_Variables   Types Exported Variables
 * @{
 */
extern volatile S2LPStatus g_xStatus;


/**
 * @defgroup Types_Exported_Macros              Types Exported Macros
 * @{
 */

#define IS_SFUNCTIONAL_STATE(STATE)   (STATE == S_DISABLE || STATE == S_ENABLE)
#define IS_SFLAG_STATUS(STATUS)   (STATUS == S_RESET || STATUS == S_SET)
#define IS_SBOOL(STATUS)   (STATUS == S_FALSE || STATUS == S_TRUE)

#define S_ABS(a) ((a)>0?(a):-(a))

#define s_assert_param(expr)        {}

/* Functions  -----------------------------------------------------------*/

/*!******************************************************************
 * \fn void S2LPRefreshStatus(void)
 * \brief S2LP Status global variable.
 *         This global variable of @ref S2LPStatus type is updated on every SPI transaction
 *         to maintain memory of S2LP Status.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPRefreshStatus(void);

#endif /* __HT_P2P_S2LP_TYPES_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
