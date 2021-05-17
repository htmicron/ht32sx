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
 * \file HT_P2P_s2lp_config.h
 * \brief S2LP Configuration and useful defines .
 * \author HT Micron Advanced R&D
 * \link support_iot@htmicron.com.br
 * \version 2.2
 * \date September 29, 2020
 */

#ifndef __HT_P2P_S2LP_CONFIG_H__
#define __HT_P2P_S2LP_CONFIG_H__

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "spi.h"
#include "HT_P2P_s2lp_types.h"

/* Defines  ------------------------------------------------------------------*/

#define DIG_DOMAIN_XTAL_THRESH  	  30000000       		/*!< Digital domain logic threshold for XTAL in MHz */
#define HIGH_BAND_FACTOR      		  4     				/*!< Band select factor for high band. Factor B in the equation 2 */
#define MIDDLE_BAND_FACTOR    		  8      				/*!< Band select factor for middle band. Factor B in the equation 2 */
#define HIGH_BAND_LOWER_LIMIT         860000000   			/*!< Lower limit of the high band: 860 MHz */
#define HIGH_BAND_UPPER_LIMIT         940000000   			/*!< Upper limit of the high band: 940 MHz */
#define MIDDLE_BAND_LOWER_LIMIT       430000000   			/*!< Lower limit of the middle band: 430 MHz */
#define MIDDLE_BAND_UPPER_LIMIT       470000000   			/*!< Upper limit of the middle band: 470 MHz */
#define MINIMUM_DATARATE              100  					/*!< Minimum datarate supported by S2LP 100 bps */
#define MAXIMUM_DATARATE              250000 	 			/*!< Maximum datarate supported by S2LP 250 ksps */
#define VCO_CENTER_FREQ               3600000000           /*!< VCO center frequency in Hz */

#define IS_FREQUENCY_BAND_HIGH(FREQUENCY) ((FREQUENCY)>=HIGH_BAND_LOWER_LIMIT && \
                                           (FREQUENCY)<=HIGH_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND_MIDDLE(FREQUENCY) ((FREQUENCY)>=MIDDLE_BAND_LOWER_LIMIT && \
                                             (FREQUENCY)<=MIDDLE_BAND_UPPER_LIMIT)

#define IS_FREQUENCY_BAND(FREQUENCY) (IS_FREQUENCY_BAND_HIGH(FREQUENCY) || \
                                      IS_FREQUENCY_BAND_MIDDLE(FREQUENCY))

#define IS_CHANNEL_SPACE(CHANNELSPACE, F_Xo)    (CHANNELSPACE<=(F_Xo/32768*255))

#define IS_DATARATE(DATARATE, F_CLK)      (DATARATE>=MINIMUM_DATARATE && DATARATE<=((uint64_t)MAXIMUM_DATARATE*F_CLK/1000000)/26)

/* Typedef  ------------------------------------------------------------------*/

/**
 * \enum ModulationSelect
 * \brief Specifies digital modulation types available
 */
typedef enum {
	MOD_NO_MOD       = 0x70, /*!< CW modulation selected */
	MOD_2FSK         = 0x00, /*!< 2-FSK modulation selected */
	MOD_4FSK         = 0x10, /*!< 4-FSK modulation selected */
	MOD_2GFSK_BT05   = 0xA0, /*!< G2FSK modulation selected with BT = 0.5 */
	MOD_2GFSK_BT1    = 0x20, /*!< G2FSK modulation selected with BT = 1 */
	MOD_4GFSK_BT05   = 0xB0, /*!< G4FSK modulation selected with BT = 0.5 */
	MOD_4GFSK_BT1    = 0x30, /*!< G4FSK modulation selected with BT = 1 */
	MOD_ASK_OOK      = 0x50, /*!< OOK modulation selected. */
	MOD_POLAR        = 0x60, /*!< OOK modulation selected. */
} ModulationSelect;

/**
 * \struct SRadioInit
 * \brief Configuration values to init S2LP.
 */
typedef struct {
	uint32_t          lFrequencyBase;     /*!< Specifies the base carrier frequency (in Hz), i.e. the carrier frequency of channel #0.
                                        	This parameter can be in one of the following ranges:
                                        	High_Band: from 860 MHz to 940 MHz
                                        	Middle Band: from 430 MHz to 470 MHz */
	ModulationSelect  xModulationSelect;  /*!< Specifies the modulation @ref ModulationSelect */
	uint32_t          lDatarate;          /*!< Specifies the datarate expressed in bps.
                                        	This parameter can be in the range between 100 bps and 500 kbps */
	uint32_t          lFreqDev;           /*!< Specifies the frequency deviation expressed in Hz.
                                        	This parameter can be in the range: [F_Xo*8/2^18, F_Xo*7680/2^18] Hz */
	uint32_t          lBandwidth;         /*!< Specifies the channel filter bandwidth
											expressed in Hz. This parameter can be
                                        	in the range between 1100 and 800100 Hz */
} SRadioInit;

/* Functions ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn SFunctionalState S2LPRadioGetDigDiv(void)
 * \brief Get the digital divider.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval SFunctionalState S_DISABLE to disable the digital divider and S_ENABLE to enable it.
 *******************************************************************/
SFunctionalState S2LPRadioGetDigDiv(void);

/*!******************************************************************
 * \fn uint8_t S2LPRadioInit(SRadioInit* pxSRadioInitStruct)
 * \brief  Initializes the S2LP analog and digital radio part according to the specified
 * 		parameters in the pxSRadioInitStruct.
 *
 * \param[in] SRadioInit pxSRadioInitStruct 		pointer to a SRadioInit structure that
 * 													contains the configuration information for the
 * 													analog radio part of S2LP.
 * \param[out] none
 *
 * \retval uint8_t Error							Code: 0=no error, 1=error during calibration of VCO.
 *******************************************************************/
uint8_t S2LPRadioInit(SRadioInit* pxSRadioInitStruct);

/*!******************************************************************
 * \fn void S2LPRadioSetDigDiv(SFunctionalState xNewState)
 * \brief Set the digital divide.
 *
 * \param[in] SFunctionalState xNewState		S_DISABLE to disable the digital divider and S_ENABLE to enable it.
 * \param[out] none
 *
 * \retval AT_cmdStruct status					Return the error status and the command type which was received.
 *******************************************************************/
void S2LPRadioSetDigDiv(SFunctionalState xNewState);

/*!******************************************************************
 * \fn void S2LPRadioComputeIF(uint32_t nIF, uint8_t* pcAnaIf, uint8_t* pcDigIf)
 * \brief Computes the ANALOG_IF and DIGITAL_IF register values staring from a image frequency value in Hz.
 *
 * \param[in] uint32_t nIF 						Datarate expressed in bps. This parameter ranging between 100 and 500000.
 * \param[in] uint8_t *pcAnaIf					Pointer to the register of analog IF.
 * \param[out] uint8_t *pcDigIf					pcDigIf pointer to the returned of digital IF.
 *
 * \retval	none.
 *******************************************************************/
void S2LPRadioComputeIF(uint32_t nIF, uint8_t *pcAnaIf, uint8_t *pcDigIf);

/*!******************************************************************
 * \fn void S2LPRadioSearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE)
 * \brief Returns the mantissa and exponent, whose value used in the datarate formula will
 * 			give the datarate value closer to the given datarate.
 *
 * \param[in] uint32_t lDatarate				Datarate expressed in bps. This parameter ranging between 100 and 500000.
 * \param[out] uint16_t* pcM					Pointer to the returned mantissa value.
 * \param[out] uint8_t* pcE						Pointer to the returned exponent value.
 *
 * \retval none
 *******************************************************************/
void S2LPRadioSearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE);

/*!******************************************************************
 * \fn uint8_t S2LPRadioSetFrequencyBase(uint32_t lFBase)
 * \brief Set the Synth word and the Band Select register according to desired base carrier frequency.
 * 			In this API the Xtal configuration is read out from
 *
 * \param[in]  uint32_t lFBase					Base carrier frequency expressed in Hz as unsigned word.
 * \param[out] none
 *
 * \retval AT_cmdStruct status					Return the error status and the command type which was received.
 *******************************************************************/
uint8_t S2LPRadioSetFrequencyBase(uint32_t lFBase);

/*!******************************************************************
 * \fn uint32_t S2LPRadioComputeSynthWord(uint32_t frequency, uint8_t refdiv)
 * \brief Computes the synth word from a given frequency.
 *
 * \param[in] uint32_t frequency					Target frequency value expressed in Hz.
 * \param[in] uint8_t refdiv						Reference divider value value (it can be only 1 or 2).
 * \param[out] none
 *
 * \retval uint32_t 								SYNTH_WORD.
 *******************************************************************/
uint32_t S2LPRadioComputeSynthWord(uint32_t frequency, uint8_t refdiv);

/*!******************************************************************
 * \fn SFunctionalState S2LPRadioGetRefDiv(void)
 * \brief To know if the reference deivider is enabled or disabled.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval SFunctionalState 						SET (enabled) or RESET (disabled).
 *******************************************************************/
SFunctionalState S2LPRadioGetRefDiv(void);

/*!******************************************************************
 * \fn uint32_t S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE)
 * \brief Returns the mantissa and exponent, whose value used in the datarate formula
 * 			will give the datarate value closer to the given datarate.
 *
 * \param[in] uint16_t cM						Datarate expressed in bps. This parameter ranging between 100 and 500000.
 * \param[out] uint8_t cE						Pointer to the returned mantissa value.
 *
 * \retval none
 *******************************************************************/
uint32_t S2LPRadioComputeDatarate(uint16_t cM, uint8_t cE);

/*!******************************************************************
 * \fn void S2LPRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE)
 * \brief Returns the mantissa and exponent for a given bandwidth.
 *         Even if it is possible to pass as parameter any value in the below mentioned range,
 *         the API will search the closer value according to a fixed table of channel
 *         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet, returning the corresponding mantissa
 *         and exponent value.
 *
 * \param[in] uint32_t lBandwidth				Bandwidth expressed in Hz. This parameter ranging between 1100 and 800100.
 * \param[out] uint8_t* pcM						Pointer to the returned mantissa value.
 * \param[out] uint8_t* pcE						Pointer to the returned exponent value.
 *
 * \retval none
 *******************************************************************/
void S2LPRadioSearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE);

/*!******************************************************************
 * \fn void S2LPRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE)
 * \brief Returns the mantissa and exponent, whose value used in the frequency deviation formula
 *         will give a frequency deviation value most closer to the given frequency deviation.
 *
 * \param[in] uint32_t lFDev					Frequency deviation expressed in Hz.
 * 												This parameter can be a value in the range [F_Xo*8/2^18, F_Xo*7680/2^18].
 * \param[out] uint8_t* pcM						Pointer to the returned mantissa value.
 * \param[out] uint8_t* pcE						Pointer to the returned exponent value.
 *
 * \retval none
 *******************************************************************/
void S2LPRadioSearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE);

/*!******************************************************************
 * \fn uint32_t S2LPRadioComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs, uint8_t refdiv)
 * \brief
 *
 * \param[in] uint32_t lFDev
 * \param[in] uint8_t* pcM
 * \param[in] uint8_t* pcE
 *
 * \retval none
 *******************************************************************/
uint32_t S2LPRadioComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs, uint8_t refdiv);

/*!******************************************************************
 * \fn uint32_t S2LPRadioGetXtalFrequency(void)
 * \brief Return the XTAL frequency.
 *
 * \param[in] none
 * \param[out] none
 *
 * \retval uint32_t 							XTAL frequency.
 *******************************************************************/
uint32_t S2LPRadioGetXtalFrequency(void);

/*!******************************************************************
 * \fn void S2LPRadioSetMaxPALevel(SFunctionalState xNewState)
 * \brief  Set the MAX_DBM bit. This will allow to transmit at the maximum power.
 *
 * \param[in] SFunctionalState xNewState		Enable (S_ENABLE) to enable or disable (S_DISABLE) the max dBm.
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPRadioSetMaxPALevel(SFunctionalState xNewState);

/*!******************************************************************
 * \fn void S2LPEnterShutdown(void)
 * \brief Enter shutdown using SDN pin.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPEnterShutdown(void);

/*!******************************************************************
 * \fn void S2LPExitShutdown(void)
 * \brief Exit shutdown using SDN pin.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval none
 *******************************************************************/
void S2LPExitShutdown(void);

#endif /* __HT_P2P_S2LP_CONFIG_H__ */

/************************ HT Micron Semicondutors S.A *****END OF FILE****/
