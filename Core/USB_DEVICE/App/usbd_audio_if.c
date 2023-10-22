#include "ak4490r.h"
#include "usbd_audio_if.h"
#include "main.h"

extern USBD_HandleTypeDef hUsbDeviceHS;
extern I2S_HandleTypeDef hi2s3;
extern I2S_HandleTypeDef hi2s1;
extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_spi1_tx;

static uint8_t AUDIO_Init();
static uint8_t AUDIO_DeInit();
static uint8_t AUDIO_AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd);
static uint8_t AUDIO_GetState();

USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops =
{
  AUDIO_Init,
  AUDIO_DeInit,
  AUDIO_AudioCmd,
  AUDIO_GetState,
};

AUDIO_CodecTypeDef codec =
{
	AK4490R_Init,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	AK4490R_SetMute,
	AK4490R_SetVolume
};

static void I2S_SetFreq(uint32_t freq)
{
	__HAL_I2S_DISABLE(&AUDIO_I2S_MSTR_HANDLE);
	LL_RCC_PLLI2S_Disable();

	if (freq % 48000U == 0)
	{
		LL_RCC_PLLI2S_ConfigDomain_I2S(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLI2SM_DIV_16, 128, LL_RCC_PLLI2SR_DIV_2);
		AUDIO_I2S_MSTR_HANDLE.Instance->I2SPR &= ~SPI_I2SPR_I2SDIV_Msk;
		AUDIO_I2S_MSTR_HANDLE.Instance->I2SPR |= (PLLI2SQ_48K / (freq << 7U)) & SPI_I2SPR_I2SDIV_Msk;
	}
	else
	{
		LL_RCC_PLLI2S_ConfigDomain_I2S(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLI2SM_DIV_20, 147, LL_RCC_PLLI2SR_DIV_2);
		AUDIO_I2S_MSTR_HANDLE.Instance->I2SPR &= ~SPI_I2SPR_I2SDIV_Msk;
		AUDIO_I2S_MSTR_HANDLE.Instance->I2SPR |= (PLLI2SQ_44K1 / (freq << 7U)) & SPI_I2SPR_I2SDIV_Msk;
	}

	LL_RCC_PLLI2S_Enable();
	while(!LL_RCC_PLLI2S_IsReady());
	__HAL_I2S_ENABLE(&AUDIO_I2S_MSTR_HANDLE);
}

static uint8_t AUDIO_Init()
{
	if (codec.Init != NULL)
	{
		codec.Init();
	}

  return USBD_OK;
}

static uint8_t AUDIO_DeInit()
{
	if (codec.DeInit != NULL)
	{
		codec.DeInit();
	}

  return USBD_OK;
}

static uint8_t AUDIO_AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd)
{
	USBD_AUDIO_HandleTypeDef* haudio = hUsbDeviceHS.pClassDataCmsit[hUsbDeviceHS.classId];
	AudioBuffer* aud_buf = &haudio->aud_buf;

  switch (cmd)
  {
	case AUDIO_CMD_PLAY:
		if (codec.Play != NULL)
		{
			codec.Play();
		}
		HAL_I2S_Transmit_DMA(&AUDIO_I2S_MSTR_HANDLE, (uint16_t*)aud_buf->mem, aud_buf->capacity >> 2);
		LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
		break;

	case AUDIO_CMD_FORMAT:
		if (codec.Format != NULL)
		{
			codec.Format(haudio->stream_type);
		}
		if (haudio->stream_type == AUDIO_FORMAT_DSD)
		{
			HAL_I2S_DMAStop(&AUDIO_I2S_MSTR_HANDLE);
			HAL_I2S_Transmit_DMA(&AUDIO_I2S_SLAVE_HANDLE, (uint16_t*)aud_buf->mem, aud_buf->capacity >> 2);
			HAL_I2S_Transmit_DMA(&AUDIO_I2S_MSTR_HANDLE, (uint16_t*)aud_buf->mem, aud_buf->capacity >> 2);
			LL_GPIO_SetOutputPin(DSDOE_GPIO_Port, DSDOE_Pin);
		}
		else
		{
			HAL_I2S_DMAStop(&AUDIO_I2S_SLAVE_HANDLE);
			HAL_I2S_DMAStop(&AUDIO_I2S_MSTR_HANDLE);
			HAL_I2S_Transmit_DMA(&AUDIO_I2S_MSTR_HANDLE, (uint16_t*)aud_buf->mem, aud_buf->capacity >> 2);
			LL_GPIO_ResetOutputPin(DSDOE_GPIO_Port, DSDOE_Pin);
		}
		break;

	case AUDIO_CMD_STOP:
		if (codec.Stop != NULL)
		{
			codec.Stop();
		}
		HAL_I2S_DMAStop(&AUDIO_I2S_SLAVE_HANDLE);
		HAL_I2S_DMAStop(&AUDIO_I2S_MSTR_HANDLE);
		LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
		LL_GPIO_SetOutputPin(LED3_GPIO_Port, LED3_Pin);
		LL_GPIO_ResetOutputPin(DSDOE_GPIO_Port, DSDOE_Pin);
		break;

	case AUDIO_CMD_FREQ:
		if (codec.Freq != NULL)
		{
			codec.Freq(*(uint32_t*)pbuf);
		}
		I2S_SetFreq(*(uint32_t*)pbuf);
		break;

	case AUDIO_CMD_MUTE:
		if (codec.Mute != NULL)
		{
			codec.Mute(*pbuf);
		}
		break;

	case AUDIO_CMD_VOLUME:
		if (codec.Volume != NULL)
		{
			codec.Volume(*pbuf);
		}
		break;

	default:
		return USBD_FAIL;
  }

  return USBD_OK;
}

static uint8_t AUDIO_GetState()
{
	return USBD_OK;
}


