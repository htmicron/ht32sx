/*!
 \if SIGFOX PATTERN
 ----------------------------------------------

   _____   _   _____   _____   _____  __    __      
  /  ___/ | | /  ___| |  ___| /  _  \ \ \  / /      
  | |___  | | | |     | |__   | | | |  \ \/ /       
  \___  \ | | | |  _  |  __|  | | | |   }  {        
   ___| | | | | |_| | | |     | |_| |  / /\ \
  /_____/ |_| \_____/ |_|     \_____/ /_/  \_\

  ----------------------------------------------

    !!!!  DO NOT MODIFY THIS FILE !!!!

  ----------------------------------------------
 \endif
  ----------------------------------------------*/
/*!
    \mainpage Sigfox RF and Protocol API documentation

    The picture below describes the architecture of a Sigfox Device organized as software Layers.<br>
    <br>
    The Addon Sigfox RF and Protocol is one application firmware - as described below.
    <br> 
    \htmlonly <style>div.image img[src="sw_architecture.png"]{width:100%;height=100%}</style> \endhtmlonly
    @image html sw_architecture.png 
*/

/*!
 * \file addon_sigfox_rf_protocol_api.h
 * \brief Sigfox RF and Protocol test application functions
 * \author $(SIGFOX_LIB_AUTHOR)
 * \version $(SIGFOX_LIB_VERSION)
 * \date $(SIGFOX_LIB_DATE)
 * \copyright Copyright (c) 2011-2017 SIGFOX, All Rights Reserved. This is unpublished proprietary source code of SIGFOX.
 *
 * This file includes the Sigfox RF and Protocol test modes functions to be used for the Sigfox RF and Protocol application 
 */

/* ######################################################################################### */

/* ------------------------------------------------------------------------------------------------------------------------------------- 
                                            IMPORTANT NOTE on ERROR CODES                             
   ------------------------------------------------------------------------------------------------------------------------------------- 

   ALL ADDON_SIGFOX_RF_PROTOCOL_API_xxx functions returns an error type sfx_error_t

   The sfx_error_t is composed of the following :
  \verbatim
     MSB_____________________________________________LSB
     15                      8|7                     0
      |                       |                      |
      |   MANUF_ERROR_CODE    |  SIGFOX_ERROR_CODE   |
      |_______________________|______________________|
 
  \endverbatim
    MANUF_ERROR_CODE :
      - Represents the error codes for MCU_API_xx, RF_API_xx, SE_API_xx, REPEATER_API_xx
      - All MCU_API_xx, RF_API_xx, SE_API_xx, REPEATER_API_xx have to return SFX_ERR_NONE when no error
      - Minimum set of error codes are defined into the mcu_api.h, rf_api.h, se_api.h and repeater_api.h : Manufacturer can define more error codes !
 
   SIGFOX_ERROR_CODE :
      - Represents the error codes of the SIGFOX API, internal functions or Sigfox RF and Protocol Application functions.
      - All the SIGFOX API and internal functions error codes can be found in the sigfox_api.h. 
      - All the SIGFOX RF and PROTOCOL application error codes can be found in this file. 

   ------------------------------------------------------------------------------------------------------------------------------------- */

#include "sigfox_types.h"

/* ---------------------------------------------------------------- */
/* Bytes reserved for SIGFOX ERROR CODES : From 0x10 to 0xBF        */
/* ---------------------------------------------------------------- */

/* They are defined in sigfox_api.h */

/* ----------------------------------------------------------------------------- */
/* Bytes reserved for SIGFOX RF and PROTOCOL APP ERROR CODES : From 0xC0 to 0xDF        */
/* ----------------------------------------------------------------------------- */
#define SFX_RF_PROTOCOL_ERR_API_TIMER_START                      (sfx_u8)(0xC0)
#define SFX_RF_PROTOCOL_ERR_API_REPORT_TEST                      (sfx_u8)(0xC1)
#define SFX_RF_PROTOCOL_ERR_API_SLEEP_SEND_BIT_1                 (sfx_u8)(0xC2)
#define SFX_RF_PROTOCOL_ERR_API_SLEEP_SEND_BIT_2                 (sfx_u8)(0xC3)
#define SFX_RF_PROTOCOL_ERR_API_SLEEP_SEND_OUTOFBAND             (sfx_u8)(0xC4)
#define SFX_RF_PROTOCOL_ERR_API_SLEEP_SEND_REPEATER_STATUS       (sfx_u8)(0xC5)
#define SFX_RF_PROTOCOL_ERR_API_SLEEP_SEND_FRAME                 (sfx_u8)(0xC6)
#define SFX_RF_PROTOCOL_ERR_API_TX_DBPSK                         (sfx_u8)(0xC7)
#define SFX_RF_PROTOCOL_ERR_API_DELAY_SENSI                      (sfx_u8)(0xC8)
#define SFX_RF_PROTOCOL_ERR_API_DELAY_TX_SYNTH                   (sfx_u8)(0xC9)
#define SFX_RF_PROTOCOL_ERR_API_WRONG_RC_ENUM                    (sfx_u8)(0xCA)
#define SFX_RF_PROTOCOL_ERR_API_GET_PAYLOAD_ENCRYPTION_FLAG      (sfx_u8)(0xCB)
#define SFX_RF_PROTOCOL_ERR_API_TEST_MODE_UNKNOWN                (sfx_u8)(0xCC)
#define SFX_RF_PROTOCOL_ERR_API_SLEEP_SEND_RC_SYNC               (sfx_u8)(0xCD)
#define SFX_RF_PROTOCOL_ERR_API_MONARCH                          (sfx_u8)(0xCE)
#define SFX_RF_PROTOCOL_ERR_API_NOT_A_MONARCH_TEST_MODE          (sfx_u8)(0xCF)
#define SFX_RF_PROTOCOL_ERR_API_NVM                              (sfx_u8)(0xD0)


typedef enum
{
     SFX_RC1   = 0, /*!< Radio Configuration 1 */
     SFX_RC2   = 1, /*!< Radio Configuration 2 */
     SFX_RC3A  = 2, /*!< Radio Configuration 3A */
     SFX_RC3C  = 3, /*!< Radio Configuration 3C */
     SFX_RC4   = 4, /*!< Radio Configuration 4 */
     SFX_RC5   = 5, /*!< Radio Configuration 5 */
     SFX_RC6   = 6, /*!< Radio Configuration 6 */
     SFX_RC101 = 7, /*!< Radio Configuration 101 */

     /* Keep this enum at the end so that is counts the number of supported RC */
     SFX_RC_LIST_MAX_SIZE,  /*!< Max Radio Configuration size */
} sfx_rc_enum_t;


typedef enum
{
     SFX_TEST_MODE_TX_BPSK              = 0,   /*!< Only BPSK with Synchro Bit + Synchro frame + PN sequence : no hopping centered on the TX_frequency */
     SFX_TEST_MODE_TX_PROTOCOL          = 1,   /*!< with full protocol with AES key defined at SIGFOX_API_open call: send all SIGFOX protocol frames available with hopping     */
     SFX_TEST_MODE_RX_PROTOCOL          = 2,   /*!< with full protocol with AES key defined at SIGFOX_API_open call: send SIGFOX protocol frames w/ initiate_downlink_flag     = SFX_TRUE */
     SFX_TEST_MODE_RX_GFSK              = 3,   /*!< with known pattern with SB + SF + Pattern on RX_Frequency defined at SIGFOX_API_open function : od internaly compare re    ceived frame <=> known pattern and call sfx_test_report() */
     SFX_TEST_MODE_RX_SENSI             = 4,   /*!< Do uplink +  downlink frame with AES key defined at SIGFOX_API_open call but specific shorter timings */
     SFX_TEST_MODE_TX_SYNTH             = 5,   /*!< Do 9 uplink frames to measure frequency synthesis step */
     SFX_TEST_MODE_TX_FREQ_DISTRIBUTION = 6,   /*!< Call all Sigfox frames of all types and size on all the Sigfox Band  */
     SFX_TEST_MODE_RX_MONARCH_PATTERN_LISTENING_SWEEP  = 7,  /*!< Scan all supported RC-pattern@freq and return the detected pattern with its RSSI  */
     SFX_TEST_MODE_RX_MONARCH_PATTERN_LISTENING_WINDOW = 8,  /*!< Scan 1 RC-pattern@freq during a specific time and return the detected pattern with its RSSI  */
     SFX_TEST_MODE_RX_MONARCH_BEACON                   = 9,  /*!< Keep listening on the RC beacon */
     SFX_TEST_MODE_RX_MONARCH_SENSI                    = 10, /*!< Execute Uplink frame (with counter) and scan for all supported RC-pattern@freq, return the result and loop again */     
     SFX_TEST_MODE_TX_BIT               = 11, /*!< Call twice the Sigfox frames (payload 1 bit only) */
     SFX_TEST_MODE_PUBLIC_KEY           = 12, /*!< Execute the public key test - all the frames of the protocol needs to be sent */
     SFX_TEST_MODE_NVM                  = 13, /*!< Execute the nvm test */
} sfx_test_mode_t;

/*!******************************************************************
 * \fn sfx_error_t ADDON_SIGFOX_RF_PROTOCOL_API_get_version ( sfx_u8 **version, sfx_u8 *size )
 * \brief This function allows to get the version of the Addon 
 *
 * \param[out] sfx_u8 **version            Pointer to Byte array (ASCII format) containing library version
 * \param[out] sfx_u8 *size                Size of the byte array pointed by *version
 *
 * \retval  The sfx_error_t is composed of the following :
 *
 * \verbatim
 *    MSB_____________________________________________LSB
 *     |                       |                      |
 *     |   MANUF_ERROR_CODE    |  SIGFOX_ERROR_CODE   |
 *     |_______________________|______________________|
 * \endverbatim 
 *
 *******************************************************************/
sfx_error_t ADDON_SIGFOX_RF_PROTOCOL_API_get_version ( sfx_u8 **version, sfx_u8 *size );

/*!******************************************************************
 * \fn sfx_error_t ADDON_SIGFOX_RF_PROTOCOL_API_test_mode(sfx_rc_enum_t rc_enum, sfx_test_mode_t test_mode)
 * \brief This function executes the test modes needed for the Sigfox RF and Protocol Tests. 
 * 
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_TX_BPSK (TEST_MODE 0).</B><BR>
 *  This test consists in sending 3 Sigfox Frames of 26 bytes with specific data @ central frequency.<BR>
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_TX_PROTOCOL (TEST_MODE 1).</B><BR>
 *  This test consists in calling SIGFOX_API_send_xxx functions to test the complete protocol in Uplink only.
 *  for all size of frames w/ Data = 0x40 to 0x4B
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_RX_PROTOCOL (TEST_MODE 2).</B><BR>
 *  This test consists in calling SIGFOX_API_send_xxx functions to test the
 *  complete protocol in Downlink only, w/ Uplink Data = 0x40 to 0x4B
 *  At the end of the test, a send bit message is sent to indicate if the downlink payload decoded by the device
 *  matches with the payload sent by the test tool.<BR>
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_RX_GFSK (TEST_MODE 3).</B><BR>
 *  This test consists in receiving constant GFSK frames @ constant frequency.<BR>
 *  The pattern used for test is : <B>AA AA B2 27 1F 20 41 84 32 68 C5 BA 53 AE 79 E7 F6 DD 9B</B>
 *  with <B>AA AA B2 27</B> configured in RF chip<BR>.
 *  The test is executed during 30s.
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_RX_SENSI (TEST_MODE 4).</B><BR>
 *  This test is specific to SIGFOX's test equipments & softwares.<BR>
 *  It is mandatory to measure the real sensitivity of device.<BR>
 *  This test execute a kind of "ping-pong" loop that starts with a frame sent by the device 
 *  and then a response sent by the equipment ( downlink frame ) that has to be received by the device.
 *  Internal counters are incremented to count the number of received frames.
 *  Report of the current status ( received or not ) is done through the MCU_API_report_test_result function
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_TX_SYNTH (TEST_MODE 5).</B><BR>
 *  This test consists in sending 9 SIGFOX frames with 4Bytes payload ( containing the programmed frequency )
 *  starting with the central frequency and then spaced with 100 Hz. 
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_TX_FREQ_DISTRIBUTION (TEST_MODE 6).</B><BR>
 *  This test consists in calling SIGFOX_API_send_xxx functions to test the
 *  complete protocol in Uplink only, w/ Uplink Data = 0x40 to 0x4B<BR>.
 *  There are several occurences of the loop to ensure the full Sigfox band is covered
 *  ______________________________________________________________________________________________ 
 *
 *  - <B>If test_mode = SFX_TEST_MODE_TX_BIT (TEST_MODE 11).</B><BR>
 *  This test consists in calling SIGFOX_API_send_bit function twice to test
 *  part of the protocol in Uplink only and LBT .
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_PUBLIC_KEY (TEST_MODE 12).</B><BR>
 *  This test consists in calling SIGFOX_API_send_frame function with the PUBLIC KEY activated<BR>.
 *  The device returns to PRIVATE KEY at the end of this test. 
 *  ______________________________________________________________________________________________
 *
 *  - <B>If test_mode = SFX_TEST_MODE_NVM (TEST_MODE 13).</B><BR>
 *  This test consists in calling SIGFOX_API_send_frame functions once with the PN of the NVM data
 *  as the payload.
 *  ______________________________________________________________________________________________
 * \param[in]  sfx_rc_t                    Radio Configuration pointer
 * \param[in]  sfx_test_mode_t test_mode   Test mode to be executed
 *
 * \retval  The sfx_error_t is composed of the following :
 *
 * \verbatim
 *    MSB_____________________________________________LSB
 *     |                       |                      |
 *     |   MANUF_ERROR_CODE    |  SIGFOX_ERROR_CODE   |
 *     |_______________________|______________________|
 * \endverbatim 
 *
 *******************************************************************/
sfx_error_t ADDON_SIGFOX_RF_PROTOCOL_API_test_mode(sfx_rc_enum_t rc_enum, sfx_test_mode_t test_mode);


/*!******************************************************************
 * \fn sfx_error_t ADDON_SIGFOX_RF_PROTOCOL_API_monarch_test_mode(sfx_rc_enum_t rc_enum, sfx_test_mode_t test_mode, sfx_u8 rc_capabilities)
 * \brief This function executes the \htmlonly <font color=red> MONARCH  </font> \endhtmlonly test modes needed for the Sigfox RF and Protocol Tests.
 * 
 *  ______________________________________________________________________________________________
 *
 *  - <B> \htmlonly <font color=red>MONARCH FEATURE ONLY </font> \endhtmlonly
 *  If test_mode = SFX_TEST_MODE_RX_MONARCH_PATTERN_LISTENING_SWEEP (TEST_MODE 7).</B><BR>
 *  This test consists in setting the device in pattern scan for 30s in LISTENING_SWEEP mode
 *  and report status TRUE or FALSE depending on the pattern found vs expected pattern. 
 *  ______________________________________________________________________________________________
 *
 *  - <B> \htmlonly <font color=red>MONARCH FEATURE ONLY </font> \endhtmlonly
 *  If test_mode = SFX_TEST_MODE_RX_MONARCH_PATTERN_LISTENING_WINDOW (TEST_MODE 8).</B><BR>
 *  This test consists in setting the device in pattern scan for 30s in LISTENING_WINDOW mode
 *  and report status TRUE or FALSE depending on the pattern found vs expected pattern.
 *  ______________________________________________________________________________________________
 *
 *  - <B> \htmlonly <font color=red>MONARCH FEATURE ONLY </font> \endhtmlonly
 *  If test_mode = SFX_TEST_MODE_RX_MONARCH_BEACON (TEST_MODE 9).</B><BR>
 *  This test is specific to SIGFOX's test equipments & softwares.<BR>
 *  This test execute a kind of "ping-pong" loop that starts with a frame sent by the device 
 *  to the equipment. At the reception of the frame, the equipment sends the beacon that 
 *  will be received by the device. The addon computes the RC found counters which are 
 *  reported in the Uplink Sigfox Frame to the equipment.
 *  This test is executed for 1000 loops ( 1000 beacons sent )
 *  ______________________________________________________________________________________________
 *
 *  - <B> \htmlonly <font color=red>MONARCH FEATURE ONLY </font> \endhtmlonly
 *  If test_mode = SFX_TEST_MODE_RX_MONARCH_SENSI (TEST_MODE 10).</B><BR>
 *  This test is specific to SIGFOX's test equipments & softwares.<BR>
 *  This test execute a kind of "ping-pong" loop that starts with a frame sent by the device 
 *  to the equipment. At the reception of the frame, the equipment sends the beacon that 
 *  will be received by the device. The addon computes the RC found counters which are 
 *  reported in the Uplink Sigfox Frame to the equipment.
 *  This test is executed for 10 loops ( 10 beacons sent )
 *  ______________________________________________________________________________________________
 * 
 * 
 * \param[in]  sfx_rc_t                    Radio Configuration pointer
 * \param[in]  sfx_test_mode_t test_mode   Test mode to be executed
 * \param[in]  sfx_u8 rc_capabilities      Radio Capabilities in bitmask format
 *                                         as defined in SIGFOX_MONARCH_API_executed_rc_scan
 *
 * \retval  The sfx_error_t is composed of the following :
 *
 * \verbatim
 *    MSB_____________________________________________LSB
 *     |                       |                      |
 *     |   MANUF_ERROR_CODE    |  SIGFOX_ERROR_CODE   |
 *     |_______________________|______________________|
 * \endverbatim 
 *
 *******************************************************************/
sfx_error_t ADDON_SIGFOX_RF_PROTOCOL_API_monarch_test_mode(sfx_rc_enum_t rc_enum, sfx_test_mode_t test_mode, sfx_u8 rc_capabilities);



