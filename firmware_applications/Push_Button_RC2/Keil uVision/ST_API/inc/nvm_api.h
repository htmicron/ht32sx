/*!
 * \file nvm_api.h
 * \brief NVM handler
 * \author  AMG - RF Application team
 * \version 1.2.0
 * \date February, 2019
 * \copyright COPYRIGHT 2019 STMicroelectronics
 *
 * This file defines the functions to handle EEPROM or FLASH NVM
 */


#ifndef __NVM_API_H
#define __NVM_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/// \cond DO_NOT_DOCUMENT
#ifdef ERASE_VALUE_IS_FF
  #define NVM_BLOCK_INVALID	0xFFFFFFFCFFFFFF0E
  #define NVM_BLOCK_VALID	0xFFFFFF0CFFFFFF0E
  #define NVM_BLOCK_EMPTY	0xFFFFFFFFFFFFFFFF

  #define NVM_BLOCK_INVALID_1 0xFFFFFFFF
  #define NVM_BLOCK_INVALID_2 0xFFFFFF0E

  #define NVM_BLOCK_VALID_1 	0xFFFFFF0E
  #define NVM_BLOCK_VALID_2 	0xFFFFFF0C
#else
  #define NVM_BLOCK_INVALID	0x00000000000000FE
  #define NVM_BLOCK_VALID 	0x000000FC000000FE
  #define NVM_BLOCK_EMPTY	0x00

  #define NVM_BLOCK_INVALID_1 0x00000000
  #define NVM_BLOCK_INVALID_2 0x000000FE

  #define NVM_BLOCK_VALID_1 	0x000000FE
  #define NVM_BLOCK_VALID_2 	0x000000FC
#endif

#define BLOCK_HEADER_SIZE 8 /* To preserve data integrity, 2 words are reserved for record validation */
#define BLOCK_BODY_SIZE   8 /* NVM data size */

#define EEPROM_NVDATA_ADDRESS 0x600
#define RCZ_MASK 0x000000FF
/// \endcond

/**
* @brief Describes NVM Type. Possible values reference FLASH or EEPROM memory
*
*/
typedef enum
{
  NVM_TYPE_EEPROM = 0,
  NVM_TYPE_FLASH = 1
} NVM_TYPE;


/**
* @brief Describes write mode operation.
*
* @detail Flash Write can be be performed with or without erasing the page before write
*/
typedef enum
{
  NVM_WRITE_MODE_WRITEOVER = 0, /*!< Write data without erase the page */
  NVM_WRITE_MODE_ERASE = 1      /*!< Erase the page before write */
} NVM_WRITE_MODE;


/**
* @brief Return values for NVM Read/Write functions
*
*/
typedef enum
{
  NVM_RW_OK = 0,
  NVM_WRITE_ERROR,
  NVM_READ_ERROR,
  NVM_WRITE_RECORD_ERROR,
  NVM_READ_RECORD_ERROR,
  NVM_WRITE_HEADER_ERROR,
  NVM_NO_RECORDS
} NVM_RW_RESULTS;


/**
* @brief Board Data update modes
*
*/
typedef enum
{
  NVM_FREQ_OFFSET = 0,
  NVM_RSSI_OFFSET = 1,
  NVM_LBT_OFFSET = 2
} NVM_UPDATE_OFFSET;


/**
* @brief NVM API configuration structure
*
* @detail Information needed to properly configure the NVM layer for Sigfox functionalities. Default is NVM_TYPE_EEPROM
*/
typedef struct
{
  NVM_TYPE nvmType; /*!< Specify if NVM is EEPROM or FLASH */
  uint32_t boardDataAddress;/*!< Destination address where store NVM_BoardDataType (ID, PAC, RCZ, etc.) */
  uint32_t sfxDataAddress;/*!< Destination address where store NVM Sigfox data (message counter, etc.)*/
  uint8_t  messageSequenceNumber;/*!< Set this value to a specific number if Invalid message sequence error occurs */
} NVM_ConfigType;


/// \cond DO_NOT_DOCUMENT
#define INIT_NVM_CONFIG(X) NVM_ConfigType X = { .nvmType = NVM_TYPE_EEPROM, \
.boardDataAddress = 0, .sfxDataAddress = EEPROM_NVDATA_ADDRESS, .messageSequenceNumber = 0x00 }
/// \endcond


/**
* @brief Record for Sigfox recurrent data, like messages counter.
*
*/
typedef struct
{
  uint16_t pn;
  uint16_t seqNumber;
  uint16_t fh;
  uint16_t rl;
} NVM_RecurrentDataType;


/**
* @brief Record for board data
*
*/
typedef struct
{
  uint32_t id;
  uint8_t  rcz;
  uint8_t  pac[8];
  int32_t  freqOffset;
  int32_t  rssiOffset;
  int32_t  lbtOffset;
} NVM_BoardDataType;


/*-------------------------------------FUNCTION PROTOTYPES------------------------------------*/

/**
* @brief  Initialize NVM API
*
* @param  config configuration structure.
*/
void SetNVMInitial(NVM_ConfigType *config);


/**
* @brief  Returns the configured NVM Type
* @param  None
* @retval NVM_TYPE
*/
NVM_TYPE GetNVMType(void);


/**
* @brief  Returns the Board Data Address
* @param  None
* @retval Board Data Address
*/
uint32_t GetNVMBoardDataAddress(void);


/**
* @brief  Returns the Sigfox Data Address
* @param  None
* @retval Sigfox Data Address
*/
uint32_t GetNVMSigfoxDataAddress(void);


/**
* @brief  Reads data from NVM
*
* @param  nAddress Read start address
* @param  cNbBytes Number of bytes to read
* @param  pcBuffer Returned buffer
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_Read(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer);


/**
* @brief Writes data to NVM
*
* @param  nAddress Write start address
* @param  cNbBytes Number of bytes to write
* @param  pcBuffer Data to write
* @param  writeMode Write mode operation type @see #NVM_WRITE_MODE
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_Write(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer, NVM_WRITE_MODE writeMode);


/**
* @brief Reads last record stored at userSpaceAddress
*
* @param  nvmRecord Returned record
* @param  recordSize Size of record
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_ReadRecord(uint8_t* nvmRecord, uint32_t recordSize);


/**
* @brief Writes record in the last available slot in the userSpace page. If no space left, the page will be erased before write
*
* @param  nvmRecord Record to write
* @param  recordSize Size of record
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_WriteRecord(uint8_t* nvmRecord, uint32_t recordSize);


/**
* @brief Update Board Offsets (Frequency, RSSI and LBT offsets)
*
* @param  data Offset value
* @param  updateWhat Specifies what kind of offset to overwrite @see NVM_UPDATE_OFFSET
* @retval NVM_RW_RESULTS
*/
NVM_RW_RESULTS NVM_UpdateOffset(NVM_UPDATE_OFFSET updateWhat, int32_t data);

void eraseAllSector(void);

#ifdef __cplusplus
}
#endif

#endif

/******************* (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE*****/
