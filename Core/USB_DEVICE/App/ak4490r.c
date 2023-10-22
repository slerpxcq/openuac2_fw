#include "ak4490r.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t AK4490R_Init()
{
	LL_GPIO_ResetOutputPin(PDN_GPIO_Port, PDN_Pin);
	uint8_t tmp = 0x00;
	if (HAL_I2C_Mem_Write(&hi2c1, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL1_ADDR, I2C_MEMADD_SIZE_8BIT,
				&tmp, 1, 0xffffffff) != HAL_OK)
	{
		return 1;
	}

	tmp = AK4490R_ACKS | AK4490R_RSTN | AK4490R_DIF2 | AK4490R_DIF1 | AK4490R_DIF0;
	if (HAL_I2C_Mem_Write(&hi2c1, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL1_ADDR, I2C_MEMADD_SIZE_8BIT,
			&tmp, 1, 0xffffffff) != HAL_OK)
	{
		return 1;
	}

//	AK4490R_SetVolume(255-40);

	return 0;
}

uint8_t AK4490R_SetVolume(uint8_t vol)
{
	uint16_t tmp = (vol << 8) | vol;
	if (HAL_I2C_Mem_Write(&hi2c1, AK4490R_I2C_DEV_ADDR, AK4490R_LCH_ATT_ADDR, I2C_MEMADD_SIZE_8BIT,
				(uint8_t*)&tmp, 2, 0xffffffff) != HAL_OK)
	{
		return 1;
	}

	return 0;
}

uint8_t AK4490R_SetMute(uint8_t mute)
{
	return 0;
}


