#include "usbd_audio_if.h"
#include "main.h"

extern USBD_HandleTypeDef hUsbDeviceHS;
extern I2S_HandleTypeDef hi2s2;

static AudioItfState s_ItfState;

static int8_t AUDIO_Init();
static int8_t AUDIO_DeInit();
static int8_t AUDIO_AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd);
static int8_t AUDIO_GetState();

USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops =
{
  AUDIO_Init,
  AUDIO_DeInit,
  AUDIO_AudioCmd,
  AUDIO_GetState,
};

static void AUDIO_SetSamFreq(uint32_t freq)
{
	__HAL_I2S_DISABLE(&hi2s2);
	LL_RCC_PLLI2S_Disable();

	if (freq % 48000U == 0)
	{
		LL_RCC_PLLI2S_ConfigDomain_I2S(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLI2SM_DIV_16, 128, LL_RCC_PLLI2SR_DIV_2);
		hi2s2.Instance->I2SPR &= ~SPI_I2SPR_I2SDIV_Msk;
		hi2s2.Instance->I2SPR |= (PLLI2SQ_48K / (freq << 7U)) & SPI_I2SPR_I2SDIV_Msk;
	}
	else
	{
		LL_RCC_PLLI2S_ConfigDomain_I2S(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLI2SM_DIV_20, 147, LL_RCC_PLLI2SR_DIV_2);
		hi2s2.Instance->I2SPR &= ~SPI_I2SPR_I2SDIV_Msk;
		hi2s2.Instance->I2SPR |= (PLLI2SQ_44K1 / (freq << 7U)) & SPI_I2SPR_I2SDIV_Msk;
	}

	LL_RCC_PLLI2S_Enable();
	while(!LL_RCC_PLLI2S_IsReady());
	__HAL_I2S_ENABLE(&hi2s2);
}

static int8_t AUDIO_Init()
{
	s_ItfState = ITF_STOPPED;

  return USBD_OK;
}

static int8_t AUDIO_DeInit()
{
	s_ItfState = ITF_STOPPED;

  return USBD_OK;
}

static int8_t AUDIO_AudioCmd(uint8_t* pbuf, uint32_t size, uint8_t cmd)
{
	USBD_AUDIO_HandleTypeDef* haudio = hUsbDeviceHS.pClassDataCmsit[hUsbDeviceHS.classId];
	AudioBuffer* aud_buf = &haudio->aud_buf;

  switch (cmd)
  {
  case AUDIO_CMD_START:
  	break;

	case AUDIO_CMD_PLAY:
		HAL_I2S_Transmit_DMA(&hi2s2, (void*)aud_buf->mem, aud_buf->capacity);
		s_ItfState = ITF_PLAYING;
		break;

	case AUDIO_CMD_STOP:
		HAL_I2S_DMAStop(&hi2s2);
		AudioBuffer_Reset(aud_buf, 0U);
		s_ItfState = ITF_STOPPED;
		break;

	case AUDIO_CMD_FREQ:
		AUDIO_SetSamFreq(*(uint32_t*)pbuf);
		break;

	case AUDIO_CMD_MUTE:
		break;

	case AUDIO_CMD_VOLUME:
		break;

	default:
		return USBD_FAIL;
  }

  return USBD_OK;
}


static int8_t AUDIO_GetState()
{
  return s_ItfState;
}


