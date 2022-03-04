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
 * \file HT_P2P_s2lp_commands.h
 * \brief Management of S2-LP Commands.
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_COMMANDS_H__
#define __HT_P2P_S2LP_COMMANDS_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "spi.h"
#include "HT_P2P_s2lp_regs.h"
#include "HT_P2P_FEM.h"
#include "HT_P2P_s2lp_util.h"

/* Defines ------------------------------------------------------------------*/

#define IS_S2LP_CMD(CMD)  (CMD == CMD_TX || \
                             CMD == CMD_RX || \
                             CMD == CMD_READY || \
                             CMD == CMD_STANDBY || \
                             CMD == CMD_SLEEP || \
                             CMD == CMD_LOCKRX || \
                             CMD == CMD_LOCKTX || \
                             CMD == CMD_SABORT || \
                             CMD == CMD_LDC_RELOAD || \
                             CMD == CMD_SEQUENCE_UPDATE || \
                             CMD == CMD_SRES || \
                             CMD == CMD_FLUSHRXFIFO || \
                             CMD == CMD_FLUSHTXFIFO \
                            )

/**
 * @brief  Sends the TX command to S2-LP. Start to transmit.
 * @param  None.
 * @retval None.
 * @note: this macro sets the SMPS switching frequency to 5.46MHz about for ETSI regulation compliancy.
 */

#ifndef BASE_FREQ_433
#define S2LPCmdStrobeTx()         {uint8_t tmp=0x9C;\
                                    Config_RangeExt(PA_TX);\
                                     S2LPSpiWriteRegisters(0x76,1,&tmp);\
                                     S2LPCmdStrobeCommand(CMD_TX);}
#else
#define S2LPCmdStrobeTx()         {uint8_t tmp=0x9C;\
                                    Config_RangeExt(PA_TX_BYPASS);\
                                     S2LPSpiWriteRegisters(0x76,1,&tmp);\
                                     S2LPCmdStrobeCommand(CMD_TX);}
#endif

/**
 * @brief  Sends the RX command to S2-LP. Start to receive.
 * @param  None.
 * @retval None.
 * @note: this macro sets the SMPS switching frequency to 3.12MHz.
 */
#define S2LPCmdStrobeRx()         {uint8_t tmp=0x90;\
                                    Config_RangeExt(PA_RX);\
                                    S2LPSpiWriteRegisters(0x76,1,&tmp);\
                                    S2LPCmdStrobeCommand(CMD_RX);}


#define S2LPCmdStrobeReady()      S2LPCmdStrobeCommand(CMD_READY)
#define S2LPCmdStrobeStandby()    S2LPCmdStrobeCommand(CMD_STANDBY)
#define S2LPCmdStrobeSleep()      S2LPCmdStrobeCommand(CMD_SLEEP)
#define S2LPCmdStrobeLockRx()     S2LPCmdStrobeCommand(CMD_LOCKRX)
#define S2LPCmdStrobeLockTx()     S2LPCmdStrobeCommand(CMD_LOCKTX)
#define S2LPCmdStrobeSabort()     S2LPCmdStrobeCommand(CMD_SABORT)
#define S2LPCmdStrobeLdcReload()  S2LPCmdStrobeCommand(CMD_LDC_RELOAD)
#define S2LPCmdStrobeSequenceUpdate() S2LPCmdStrobeCommand(CMD_SEQUENCE_UPDATE)
#define S2LPCmdStrobeSres()          S2LPCmdStrobeCommand(CMD_SRES)
#define S2LPCmdStrobeFlushRxFifo()    S2LPCmdStrobeCommand(CMD_FLUSHRXFIFO)
#define S2LPCmdStrobeFlushTxFifo()    S2LPCmdStrobeCommand(CMD_FLUSHTXFIFO)

/* Typedef -----------------------------------------------------------*/

/**
 * \enum S2LPCmd
 * \brief S2LP Commands codes enumeration
 */
typedef enum {
  CMD_TX =  ((uint8_t)(0x60)),                    /*!< Start to transmit; valid only from READY */
  CMD_RX =  ((uint8_t)(0x61)),                    /*!< Start to receive; valid only from READY */
  CMD_READY =  ((uint8_t)(0x62)),                 /*!< Go to READY; valid only from STANDBY or SLEEP or LOCK */
  CMD_STANDBY =  ((uint8_t)(0x63)),               /*!< Go to STANDBY; valid only from READY */
  CMD_SLEEP = ((uint8_t)(0x64)),                  /*!< Go to SLEEP; valid only from READY */
  CMD_LOCKRX = ((uint8_t)(0x65)),                 /*!< Go to LOCK state by using the RX configuration of the synth; valid only from READY */
  CMD_LOCKTX = ((uint8_t)(0x66)),                 /*!< Go to LOCK state by using the TX configuration of the synth; valid only from READY */
  CMD_SABORT = ((uint8_t)(0x67)),                 /*!< Force exit form TX or RX states and go to READY state; valid only from TX or RX */
  CMD_LDC_RELOAD = ((uint8_t)(0x68)),             /*!< LDC Mode: Reload the LDC timer with the value stored in the  LDC_PRESCALER / COUNTER  registers; valid from all states  */
  CMD_RCO_CALIB =  ((uint8_t)(0x69)),             /*!< Start (or re-start) the RCO calibration */
  CMD_SRES = ((uint8_t)(0x70)),                   /*!< Reset of all digital part, except SPI registers */
  CMD_FLUSHRXFIFO = ((uint8_t)(0x71)),            /*!< Clean the RX FIFO; valid from all states */
  CMD_FLUSHTXFIFO = ((uint8_t)(0x72)),            /*!< Clean the TX FIFO; valid from all states */
  CMD_SEQUENCE_UPDATE =  ((uint8_t)(0x73)),       /*!< Autoretransmission: Reload the Packet sequence counter with the value stored in the PROTOCOL[2] register valid from all states */
} S2LPCmd;

/*!******************************************************************
 * \fn void S2LPCmdStrobeCommand(S2LPCmd xCommandCode)
 * \brief Send commands to S2LP using SPI.
 *
 * \param[in]  S2LPCmd xCommandCode					S2LP cmd.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPCmdStrobeCommand(S2LPCmd xCommandCode);

#endif /* __HT_P2P_S2LP_COMMANDS_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
