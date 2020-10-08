/*!
 * \file mcu_api_stm32.c
 * \brief Sigfox MCU functions
 * \author  R&D HT Micron
 * \version 1.0
 * \date Sept 2, 2019
 *
 */

#include "main.h"
#include "st_mcu_api.h"
#include "mcu_api.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"
#include "retriever_api.h"
#include "tim.h"

#if defined CREDENTIALS_SECURE_ELEMENT || defined CREDENTIALS_UNCRYPTED
#define MCU_ERR_API_GET_ID                  (sfx_u8)(0x1F) /*!< Error on MCU_API_get_device_id */
#define MCU_ERR_API_GET_PAC                 (sfx_u8)(0x20) /*!< Error on MCU_API_get_initial_pac */
#else
#define MCU_ERR_API_AES_GET_CRYPT           (sfx_u8)(0x21) /*!< Error on MCU_API_get_encrypted_info */
#define MCU_ERR_API_AES_DECRYPT             (sfx_u8)(0x22) /*!< Error on MCU_API_aes_128_cbc_decrypt */
#endif


#define MCU_API_VER		"v2.3.5"

static uint8_t _encryptedPayload = 0;
static const uint8_t _libVersion[] = MCU_API_VER;

void ST_MCU_API_SetEncryptionPayload(uint8_t ePayload)
{
	_encryptedPayload = ePayload;
}

sfx_u8 MCU_API_get_device_id_and_payload_encryption_flag(sfx_u8 dev_id[ID_LENGTH], sfx_bool *payload_encryption_enabled)
{
	//printf("MCU encryption_flag IN\n\r");
	enc_utils_get_id(dev_id);
	(*payload_encryption_enabled) = _encryptedPayload;
	//printf("MCU encryption_flag OUT\n\r");
	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_get_initial_pac(sfx_u8 initial_pac[PAC_LENGTH])
{
	// printf("MCU_API_get_initial_pac IN\n\r");
	enc_utils_get_initial_pac(initial_pac);
	//printf("MCU_API_get_initial_pac OUT\n\r");
	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_malloc(sfx_u16 size, sfx_u8 **returned_pointer)
{
	//printf("MCU_API_malloc IN\n\r");

	static sfx_u32 mem[500/4];

	if(size>500)
	{
		//printf("MCU_API_malloc OUT\n\r");
		return MCU_ERR_API_MALLOC;
	}
	else
	{
		(*returned_pointer)=(sfx_u8*)mem;
	}

	//printf("MCU_API_malloc OUT\n\r");

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_get_nv_mem(sfx_u8 read_data[SFX_NVMEM_BLOCK_SIZE])
{
	//printf("MCU_API_get_nv_mem IN\n\r");

	/* Read data */
	NVM_RW_RESULTS res = NVM_ReadRecord((uint8_t *)read_data, SFX_NVMEM_BLOCK_SIZE);

	//printf("MCU_API_get_nv_mem OUT\n\r");
	return res;
}

sfx_u8 MCU_API_set_nv_mem(sfx_u8 data_to_write[SFX_NVMEM_BLOCK_SIZE])
{
	//printf("MCU_API_set_nv_mem IN\n\r");

	/* Write data */
	NVM_RW_RESULTS res = NVM_WriteRecord((uint8_t *)data_to_write, SFX_NVMEM_BLOCK_SIZE);

	//printf("MCU_API_set_nv_mem OUT\n\r");
	return res;
}

sfx_u8 MCU_API_free(sfx_u8 *ptr)
{
	//printf("MCU_API_free IN\n\r");
	//printf("MCU_API_free OUT\n\r");

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_get_voltage_temperature(sfx_u16 *voltage_idle, sfx_u16 *voltage_tx, sfx_s16 *temperature) {

#ifdef USE_OOB_MSG

	/* get the idle voltage of the complete device
	  get the temperature of the device
	  if those values are not available : set it to 0x0000
	  return the voltage_idle in 1/10 volt on 16bits and 1/10 degrees for the temperature */

	uint32_t ad;
	uint32_t vref;
	uint32_t temp;

	vref = HT_getVrefData();

	ad = HT_getTemperatureAD();
	temp = HT_computeTemperature(ad, vref);

	(*voltage_idle)=(sfx_s16)vref;
	(*voltage_tx)=HT_getVddTx();
	(*temperature)=(sfx_s16)temp*10;

	HT_setVddTx(0);
	// printf("MCU_API_get_voltage_temperature OUT\n\r");

#else

	(*voltage_idle)=0;
	(*voltage_tx)=0;
	(*temperature)=0;

#endif

	return SFX_ERR_NONE;
}

static void priv_ST_MCU_API_delay(uint32_t delay_ms)
{
	//printf("priv_ST_MCU_API_delay IN (%d)\n\r",delay_ms); 

	HAL_Delay(delay_ms);

	//printf("priv_ST_MCU_API_delay OUT\n\r");
}

sfx_u8 MCU_API_delay(sfx_delay_t delay_type)
{
	switch(delay_type)
	{
	case SFX_DLY_INTER_FRAME_TRX:
	case SFX_DLY_INTER_FRAME_TX:
	case SFX_DLY_CS_SLEEP:
		/* ramping should be considered in the ramp up/down:
    since we have 72 samples in the ramp
    (18ms for each ramp, we need to compensate 36 ms)
    Moreover we have also 6ms of silence (2 before and 4 after packet)
		 */
		priv_ST_MCU_API_delay(500-2*ST_RF_API_get_ramp_duration());
		break;
	case SFX_DLY_OOB_ACK:
		priv_ST_MCU_API_delay(2000-2*ST_RF_API_get_ramp_duration());
		break;
	}

	//printf("MCU_API_delay OUT\n\r");

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_aes_128_cbc_encrypt(sfx_u8 *encrypted_data,
		sfx_u8 *data_to_encrypt,
		sfx_u8 aes_block_len,
		sfx_u8 key[16],
		sfx_credentials_use_key_t use_key)
{
	/* Let the retriever encrypts the requested buffer using the ID_KEY_RETRIEVER function.
  The retriever knows the KEY of this node. */
	//printf("MCU_API_aes_128_cbc_encrypt IN\n\r");

	enc_utils_encrypt(encrypted_data, data_to_encrypt, aes_block_len, key, use_key);

	//printf("MCU_API_aes_128_cbc_encrypt OUT\n\r");

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_get_version(sfx_u8 **version, sfx_u8 *size)
{
	(*size) = sizeof(_libVersion);
	(*version) = (sfx_u8*)_libVersion;

	return SFX_ERR_NONE;
}

sfx_u8 MCU_API_report_test_result(sfx_bool status, sfx_s16 rssi)
{

	//ST_MANUF_report_CB(status, rssi);

	// use this function to : print output result : status and rssi on uart if you have one or any link is available on device
	// or use a gpio to indicate at least the status
	// or to send a message over the air using any link to report the status with rssi
	// you could also use the RF part in specific modulation (ook ask or gfsk or else to return status and rssi
	return SFX_ERR_NONE;
}


