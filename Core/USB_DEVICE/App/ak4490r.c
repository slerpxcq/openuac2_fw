#include "ak4490r.h"
#include "usbd_audio_if.h"
#include "main.h"

extern I2C_HandleTypeDef AK4490R_I2C_HANDLE;

static uint8_t play;

AUDIO_CodecTypeDef ak4490r_instance =
{
	AK4490R_Init,
	NULL,
	AK4490R_Play,
	AK4490R_SetFormat,
	AK4490R_Stop,
	NULL,
	AK4490R_SetMute,
	AK4490R_SetVolume
};

static AK4490R_RegisterTypeDef reg;

uint8_t AK4490R_Init()
{
	LL_GPIO_ResetOutputPin(PDN_GPIO_Port, PDN_Pin);
	LL_mDelay(2);

	reg.control1 = AK4490R_ACKS | AK4490R_DIF2 | AK4490R_DIF1 | AK4490R_DIF0 | AK4490R_RSTN;
	reg.control2 = AK4490R_SD | AK4490R_DEM0;
	reg.control3 = 0;
	reg.lch_att = 0xff;
	reg.rch_att = 0xff;
	reg.control4 = AK4490R_INVL;
	reg.dsd1 = 0;
	reg.control5 = AK4490R_SYNCE;
	reg.sound_control = 0;
	reg.dsd2 = 0;
	reg.control6 = AK4490R_PW;
	reg.control7 = 0;
	reg.control8 = 0;

	HAL_I2C_Mem_Write_IT(&AK4490R_I2C_HANDLE, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL1_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&reg, sizeof(reg));

	return 0;
}

uint8_t AK4490R_SetVolume(uint8_t vol)
{
	if (AK4490R_I2C_HANDLE.State == HAL_I2C_STATE_READY)
	{
		vol = (vol > 0) ? (vol + 155) : 0;
		reg.lch_att = reg.rch_att = vol;
		HAL_I2C_Mem_Write_IT(&AK4490R_I2C_HANDLE, AK4490R_I2C_DEV_ADDR, AK4490R_LCH_ATT_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&reg.lch_att, 2);
	}

	return 0;
}

uint8_t AK4490R_SetMute(uint8_t mute)
{
	if (AK4490R_I2C_HANDLE.State == HAL_I2C_STATE_READY)
	{
		if (mute)
		{
			reg.control2 |= AK4490R_SMUTE;
		}
		else
		{
			reg.control2 &= ~AK4490R_SMUTE;
		}

		HAL_I2C_Mem_Write_IT(&AK4490R_I2C_HANDLE, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL2_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&reg.control2, 1);
	}

	return 0;
}

uint8_t AK4490R_SetFormat(uint8_t format)
{
	if (AK4490R_I2C_HANDLE.State == HAL_I2C_STATE_READY)
	{
		if (format == AUDIO_FORMAT_DSD)
		{
			reg.control3 |= AK4490R_DP;
		}
		else
		{
			reg.control3 &= ~AK4490R_DP;
		}

//		reg.control1 &= ~AK4490R_RSTN;
		HAL_I2C_Mem_Write_IT(&AK4490R_I2C_HANDLE, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL3_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&reg.control3, 1);
//		reg_reset = 1;
	}

	return 0;
}

uint8_t AK4490R_Play()
{
	play = 1;
	return 0;
}

uint8_t AK4490R_Stop()
{
	if (AK4490R_I2C_HANDLE.State == HAL_I2C_STATE_READY)
	{
		reg.control2 |= AK4490R_SMUTE;
	}

	HAL_I2C_Mem_Write_IT(&AK4490R_I2C_HANDLE, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL2_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&reg.control2, 1);
	return 0;
}

void AK4490R_ProcessEvents()
{
	if (play)
	{
		LL_mDelay(20);
		reg.control2 &= ~AK4490R_SMUTE;
		while (HAL_I2C_Mem_Write(&AK4490R_I2C_HANDLE, AK4490R_I2C_DEV_ADDR, AK4490R_CONTROL2_ADDR, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&reg.control2, 1, 1000) != HAL_OK);
		play = 0;
	}
}


