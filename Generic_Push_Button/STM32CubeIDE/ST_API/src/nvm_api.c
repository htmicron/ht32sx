/*!
 * \file mcu_api_stm32.c
 * \brief Sigfox MCU functions
 * \author  R&D HT Micron
 * \version 1.0
 * \date Sept 2, 2019
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sigfox_types.h"
#include "sigfox_api.h"

#include "mcu_api.h"
#include "nvm_api.h"
#include "SDK_UTILS_Flash.h"

static volatile NVM_TYPE _nvmType = NVM_TYPE_EEPROM;
static volatile uint8_t  _msgSq = 0x00;
static volatile uint32_t _blockPointer;
static volatile uint32_t _sfxDataAddress;
static volatile uint32_t _boardDataAddress;

#ifdef USE_FLASH
#define	_nvmReadOperation(addr, nbytes, buff) FlashRead(addr, nbytes, buff)
#define	_nvmWriteOperation(addr, nbytes, buff, mode) FlashWrite(addr, nbytes, buff, mode)
#else
#include "S2LP_AUX_EEPROM.h"

#define	_nvmReadOperation(addr, nbytes, buff) EepromRead(addr, nbytes, buff)
#define	_nvmWriteOperation(addr, nbytes, buff, mode) EepromWrite(addr, nbytes, buff)
#endif

static NVM_RW_RESULTS _setBlockState(uint32_t addr, uint64_t state)
{
	NVM_RW_RESULTS tRet = NVM_RW_OK;
	uint64_t wrState;

	if(state == NVM_BLOCK_INVALID)
	{
		wrState = NVM_BLOCK_INVALID_2;
		tRet = NVM_Write(addr, BLOCK_HEADER_SIZE/2, (uint8_t*)&wrState, NVM_WRITE_MODE_ERASE);
	}
	else if(state == NVM_BLOCK_VALID)
	{
		wrState = NVM_BLOCK_VALID_2;
		tRet = NVM_Write(addr+BLOCK_HEADER_SIZE/2, BLOCK_HEADER_SIZE/2, (uint8_t*)&wrState, NVM_WRITE_MODE_WRITEOVER);
	}
	else
		tRet = NVM_WRITE_HEADER_ERROR;

	return tRet;
}

static NVM_RW_RESULTS _writeRecordInternal(uint32_t addr, uint8_t* nvmRecord, uint32_t recordSize)
{
	NVM_RW_RESULTS tRet = NVM_RW_OK;

	tRet = _setBlockState(addr, NVM_BLOCK_INVALID);

	if(tRet == NVM_RW_OK)
		tRet = NVM_Write(addr+BLOCK_HEADER_SIZE, recordSize, (uint8_t *)nvmRecord, NVM_WRITE_MODE_WRITEOVER);

	if(tRet == NVM_RW_OK)
		tRet = _setBlockState(addr, NVM_BLOCK_VALID);

	return tRet;
}

void SetNVMInitial(NVM_ConfigType *config)
{
	_nvmType = config->nvmType;
	_sfxDataAddress = config->sfxDataAddress ;
	_boardDataAddress = config->boardDataAddress;
	_msgSq = config->messageSequenceNumber;

	/* Initialize FLASH user data for first boot */
	if(_nvmType == NVM_TYPE_FLASH)
	{
		uint8_t cleanBuf[BLOCK_BODY_SIZE];

		if (NVM_ReadRecord(cleanBuf, BLOCK_BODY_SIZE) == NVM_NO_RECORDS)
		{
			memset(cleanBuf, FLASH_ERASE_VALUE, BLOCK_BODY_SIZE);
			//FlashErase(_sfxDataAddress, 1);
			eraseAllSector();
			NVM_WriteRecord(&cleanBuf[0], BLOCK_BODY_SIZE);
		}
	}
}

NVM_TYPE GetNVMType()
{
	return _nvmType;
}

uint32_t GetNVMBoardDataAddress()
{
	return _boardDataAddress;
}

uint32_t GetNVMSigfoxDataAddress()
{
	return _sfxDataAddress;
}

NVM_RW_RESULTS NVM_Read(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer)
{
	NVM_RW_RESULTS tRet = NVM_RW_OK;
	FLS_RW_StatusTypeDef flRet = FLS_RW_OK;

	flRet = (FLS_RW_StatusTypeDef)_nvmReadOperation(nAddress, cNbBytes, pcBuffer);

	if(flRet != FLS_RW_OK)
		tRet = NVM_READ_ERROR;

	return tRet;
}

NVM_RW_RESULTS NVM_Write(uint32_t nAddress, uint8_t cNbBytes, uint8_t* pcBuffer, NVM_WRITE_MODE writeMode)
{
	NVM_RW_RESULTS tRet = NVM_RW_OK;
	FLS_RW_StatusTypeDef flRet = FLS_RW_OK;

	flRet = (FLS_RW_StatusTypeDef)_nvmWriteOperation(nAddress, cNbBytes, pcBuffer, writeMode);

	if(flRet != FLS_RW_OK)
		tRet = NVM_WRITE_ERROR;

	return tRet;
}

NVM_RW_RESULTS NVM_ReadRecord(uint8_t* nvmRecord, uint32_t recordSize)
{
	NVM_RW_RESULTS tRet = NVM_RW_OK;

	uint64_t currentBlockState;
	currentBlockState = NVM_BLOCK_INVALID;

	_blockPointer = _sfxDataAddress;

	if (_nvmType == NVM_TYPE_FLASH)
	{
		while(currentBlockState != NVM_BLOCK_EMPTY)
		{
			if(NVM_Read(_blockPointer, BLOCK_HEADER_SIZE, (uint8_t*)&currentBlockState) == NVM_RW_OK)
			{
				if(_blockPointer + (BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE) == (uint32_t)FLASH_USER_END_ADDR)
					break;
				else
					_blockPointer += BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE;
			}
			else
			{
				tRet = NVM_READ_RECORD_ERROR;
				break;
			}
		}

		if(tRet == NVM_RW_OK)
		{
			while(currentBlockState != NVM_BLOCK_VALID && tRet == NVM_RW_OK)
			{
				_blockPointer -= (BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE);

				if(_blockPointer < (uint32_t)FLASH_USER_START_ADDR)
				{
					tRet = NVM_NO_RECORDS;
					break;
				}

				tRet = NVM_Read(_blockPointer, BLOCK_HEADER_SIZE, (uint8_t*)&currentBlockState);
			}

			if(tRet == NVM_RW_OK)
				tRet = NVM_Read(_blockPointer+BLOCK_HEADER_SIZE, recordSize, (uint8_t *)nvmRecord);
			else if (tRet != NVM_NO_RECORDS)
				tRet = NVM_READ_RECORD_ERROR;
		}
	}
	else
		tRet = NVM_Read(_blockPointer, recordSize, (uint8_t *)nvmRecord);

	return tRet;
}

void eraseAllSector(void) { 
	uint32_t aux = _blockPointer;

	for(uint8_t i = 0; i < MAX_NO_OF_PAGES; i++) {
		DataEepromErase(aux);
		aux += 4;
	}
}

NVM_RW_RESULTS NVM_WriteRecord(uint8_t* nvmRecord, uint32_t recordSize)
{
	NVM_RW_RESULTS tRet = NVM_RW_OK;
	uint64_t currentBlockState;
	currentBlockState = NVM_BLOCK_INVALID;

	_blockPointer = _sfxDataAddress;

	if (_nvmType == NVM_TYPE_FLASH)
	{
		while(currentBlockState != NVM_BLOCK_EMPTY)
		{

			if(_blockPointer != (uint32_t)FLASH_USER_END_ADDR) {
				if(NVM_Read(_blockPointer, BLOCK_HEADER_SIZE, (uint8_t *)&currentBlockState) == NVM_RW_OK) {
					if(currentBlockState == NVM_BLOCK_EMPTY) {
						tRet = _writeRecordInternal(_blockPointer, nvmRecord, recordSize);
						break;
					} else
						_blockPointer += (BLOCK_BODY_SIZE+BLOCK_HEADER_SIZE);
				} else {
					tRet = NVM_READ_RECORD_ERROR;
					break;
				}
			} else {
				_blockPointer = (uint32_t)FLASH_USER_START_ADDR;
				eraseAllSector();
				tRet = _writeRecordInternal(_blockPointer, nvmRecord, recordSize);

				break;
			}
		}
	}
	else
		tRet = NVM_Write(_blockPointer, recordSize, nvmRecord, NVM_WRITE_MODE_WRITEOVER);

	return tRet;
}

NVM_RW_RESULTS NVM_UpdateOffset(NVM_UPDATE_OFFSET updateWhat, int32_t data) {
	uint8_t originalData[64];
	NVM_RW_RESULTS ret = NVM_RW_OK;
	/* avoid to corrupt eeprom content, this command works only with flash*/
	if (_nvmType == NVM_TYPE_FLASH) {
		ret = NVM_Read(_boardDataAddress, 64, &originalData[0]);

		if(ret == NVM_RW_OK) {
			if(updateWhat == NVM_FREQ_OFFSET)
				memcpy(&originalData[48], &data, sizeof(data));
			else if(updateWhat == NVM_RSSI_OFFSET)
				memcpy(&originalData[52], &data, sizeof(data));
			else if(updateWhat == NVM_LBT_OFFSET)
				memcpy(&originalData[56], &data, sizeof(data));
			else
				ret = NVM_WRITE_ERROR;
		}

		if(ret == NVM_RW_OK) {
			return NVM_Write(_boardDataAddress, 64, &originalData[0], NVM_WRITE_MODE_ERASE);
		}
	}

	return ret;
}

