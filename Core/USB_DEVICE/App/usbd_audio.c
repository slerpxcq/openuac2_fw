// USB Audio class 2.0 Implementation based on ST's 1.0 library

#include "usbd_audio.h"
#include "usbd_ctlreq.h"
#include "usbd_audio_if.h"
#include "audio_desc.h"

#ifdef USE_USBD_COMPOSITE
#error "Composite device is unsupported."
#endif

static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length);
static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev);
static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void AUDIO_REQ_GetRange(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void *USBD_AUDIO_GetAudioHeaderDesc(uint8_t *pConfDesc);

static USBD_AUDIO_HandleTypeDef s_Haudio;

USBD_ClassTypeDef USBD_AUDIO =
{
  USBD_AUDIO_Init,
  USBD_AUDIO_DeInit,
  USBD_AUDIO_Setup,
  USBD_AUDIO_EP0_TxReady,
  USBD_AUDIO_EP0_RxReady,
  USBD_AUDIO_DataIn,
  USBD_AUDIO_DataOut,
  USBD_AUDIO_SOF,
  USBD_AUDIO_IsoINIncomplete,
  USBD_AUDIO_IsoOutIncomplete,
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetCfgDesc,
  USBD_AUDIO_GetDeviceQualifierDesc,
};

static uint8_t USBD_AUDIO_IsDoPStream(USBD_HandleTypeDef* pdev)
{
	USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];
	uint32_t* buf = (uint32_t*)&haudio->aud_buf.mem[haudio->aud_buf.wr_ptr];

	const uint8_t marker_table[] = { 0x05, 0xfa };

	uint8_t idx = 0;

	switch (*buf >> 24)
	{
	case 0x05:
		idx = 0;
		break;

	case 0xfa:
		idx = 1;
		break;

	default:
		return AUDIO_FORMAT_PCM;
		break;
	}

	for (uint32_t i = 0; i < AUDIO_DOP_DETECT_COUNT; i += 2)
	{
		if (buf[i] >> 24 != marker_table[idx] || buf[i + 1] >> 24 != marker_table[idx])
		{
			return AUDIO_FORMAT_PCM;
		}

		idx = !idx;
	}

	return AUDIO_FORMAT_DSD;
}

void USBD_AUDIO_UpdateFB(USBD_HandleTypeDef *pdev)
{
	USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];

	int64_t tmp = (haudio->aud_buf.size - (haudio->aud_buf.capacity >> 1U)) << 3U;

	tmp = CLAMP(tmp, -65535, 65535);
	haudio->feedback_value = haudio->feedback_base - tmp;
}

static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  USBD_AUDIO_HandleTypeDef* haudio = &s_Haudio;

  pdev->pClassDataCmsit[pdev->classId] = haudio;
  pdev->pClassData = pdev->pClassDataCmsit[pdev->classId];

  if (pdev->dev_speed == USBD_SPEED_HIGH)
  {
    pdev->ep_out[STREAMING_EP_NUM].bInterval = STREAMING_HS_BINTERVAL;
    pdev->ep_in[FEEDBACK_EP_NUM].bInterval = 	FEEDBACK_HS_BINTERVAL;
  }
  else
  {
  	return USBD_FAIL;
  }

  USBD_LL_FlushEP(pdev, STREAMING_EP_ADDR);
  USBD_LL_FlushEP(pdev, FEEDBACK_EP_ADDR);
  USBD_LL_OpenEP(pdev, STREAMING_EP_ADDR, USBD_EP_TYPE_ISOC, USB_HS_MAX_PACKET_SIZE);
  USBD_LL_OpenEP(pdev, FEEDBACK_EP_ADDR, USBD_EP_TYPE_ISOC, FEEDBACK_PACKET_SIZE);
  pdev->ep_out[STREAMING_EP_NUM].is_used = 1U;
  pdev->ep_in[FEEDBACK_EP_NUM].is_used = 1U;

  haudio->alt_setting = 0U;
  AudioBuffer_Init(&haudio->aud_buf, 0);

  /* Initialize the Audio output Hardware layer */
  USBD_AUDIO_ItfTypeDef* itf = pdev->pUserData[pdev->classId];
  if (itf->Init() != USBD_OK)
  {
    return USBD_FAIL;
  }

  /* Prepare Out endpoint to receive 1st packet */
  USBD_LL_PrepareReceive(pdev, STREAMING_EP_ADDR, (uint8_t*)haudio->pkt_buf, USB_HS_MAX_PACKET_SIZE);
  USBD_LL_Transmit(pdev, FEEDBACK_EP_ADDR, (uint8_t*)&haudio->feedback_value, FEEDBACK_PACKET_SIZE);

  return USBD_OK;
}

static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  USBD_LL_CloseEP(pdev, STREAMING_EP_ADDR);
  USBD_LL_CloseEP(pdev, FEEDBACK_EP_ADDR);
  pdev->ep_out[STREAMING_EP_NUM].is_used = 0U;
  pdev->ep_out[STREAMING_EP_NUM].bInterval = 0U;
  pdev->ep_out[FEEDBACK_EP_NUM].is_used = 0U;
	pdev->ep_out[FEEDBACK_EP_NUM].bInterval = 0U;

  if (pdev->pClassDataCmsit[pdev->classId] != NULL)
  {
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData[pdev->classId])->DeInit();
    pdev->pClassDataCmsit[pdev->classId] = NULL;
    pdev->pClassData = NULL;
  }

  return USBD_OK;
}

static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];
  uint16_t len;
  uint8_t *pbuf;
  uint16_t status_info = 0U;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS:
    	switch (req->bRequest)
    	{
				case AUDIO_REQ_CUR:
					if (req->bmRequest & 0x80)
					{
						AUDIO_REQ_GetCurrent(pdev, req);
					}
					else
					{
						AUDIO_REQ_SetCurrent(pdev, req);
					}
					break;

				case AUDIO_REQ_RANGE:
				if (req->bmRequest & 0x80)
				{
					AUDIO_REQ_GetRange(pdev, req);
				}
				else
				{
					USBD_CtlError(pdev, req);
					return USBD_FAIL;
				}
				break;

				default:
					USBD_CtlError(pdev, req);
					return USBD_FAIL;
					break;
    	}
      break;

    case USB_REQ_TYPE_STANDARD:
      switch (req->bRequest)
      {
        case USB_REQ_GET_STATUS:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            return USBD_FAIL;
          }
          break;

        case USB_REQ_GET_DESCRIPTOR:
          if (HIBYTE(req->wValue) == CS_DEVICE)
          {
          	pbuf = (uint8_t *)USBD_AUDIO_GetAudioHeaderDesc(pdev->pConfDesc);
            if (pbuf != NULL)
            {
              len = MIN(USB_AUDIO_DESC_SIZE, req->wLength);
              (void)USBD_CtlSendData(pdev, pbuf, len);
            }
            else
            {
              USBD_CtlError(pdev, req);
              return USBD_FAIL;
            }
          }
          break;

        case USB_REQ_GET_INTERFACE:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            (void)USBD_CtlSendData(pdev, (uint8_t *)&haudio->alt_setting, 1U);
          }
          else
          {
            USBD_CtlError(pdev, req);
            return USBD_FAIL;
          }
          break;

        case USB_REQ_SET_INTERFACE:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            if ((uint8_t)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
            {
              haudio->alt_setting = (uint8_t)(req->wValue);
              haudio->bit_depth = (haudio->alt_setting == 1) ? 32U : 24U;
            }
            else
            {
              USBD_CtlError(pdev, req);
              return USBD_FAIL;
            }
          }
          else
          {
            USBD_CtlError(pdev, req);
            return USBD_FAIL;
          }
          break;

        case USB_REQ_CLEAR_FEATURE:
          break;

        default:
          USBD_CtlError(pdev, req);
          return USBD_FAIL;
          break;
      }
      break;

    default:
      USBD_CtlError(pdev, req);
      return USBD_FAIL;
      break;
  }

  return USBD_OK;
}

#ifndef USE_USBD_COMPOSITE

static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length)
{
	*length = USB_AUDIO_CONFIG_DESC_SIZE;
  return (uint8_t*)USBD_AUDIO_CfgDesc;
}
#endif /* USE_USBD_COMPOSITE  */

static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];

	if (epnum == FEEDBACK_EP_NUM)
	{
		USBD_LL_Transmit(pdev, FEEDBACK_EP_ADDR, (uint8_t*)&haudio->feedback_value, FEEDBACK_PACKET_SIZE);
	}

  return (uint8_t)USBD_OK;
}

static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
	USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];
	USBD_AUDIO_ItfTypeDef* itf = pdev->pUserData[pdev->classId];

	switch (haudio->control.unit)
	{
	case CLOCK_SOURCE_ID:
		if (haudio->control.cmd == CS_SAM_FREQ_CONTROL)
		{
			haudio->sam_freq = *(uint32_t*)haudio->control.data;

			uint32_t packetSize = (haudio->sam_freq % 48000U == 0) ? (haudio->sam_freq / 1000U) : (haudio->sam_freq / 147U * 160U / 1000U);
			uint32_t audBufCap = packetSize * AUDIO_BUFFER_PACKET_NUM;

			AudioBuffer_Reset(&haudio->aud_buf, audBufCap);

			if (haudio->sam_freq % 48000U == 0)
			{
				haudio->feedback_base = haudio->sam_freq / 48000U * AUDIO_48K_FEEDBACK_VALUE;
			}
			else
			{
				haudio->feedback_base = haudio->sam_freq / 44100U * AUDIO_44K1_FEEDBACK_VALUE;
			}
			haudio->feedback_value = haudio->feedback_base;

			itf->AudioCmd(haudio->control.data, haudio->control.len, AUDIO_CMD_FREQ);
		}
		else
		{
			return USBD_FAIL;
		}
		break;

	case FEATURE_UNIT_ID:
		switch (haudio->control.cmd)
		{
		case FU_MUTE_CONTROL:
			itf->AudioCmd(haudio->control.data, haudio->control.len, AUDIO_CMD_MUTE);
			break;

		case FU_VOLUME_CONTROL:
			itf->AudioCmd(haudio->control.data, haudio->control.len, AUDIO_CMD_VOLUME);
			break;

		default:
			return USBD_FAIL;
			break;
		}
		break;

	default:
		return USBD_FAIL;
		break;
	}

  return USBD_OK;
}

static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
	UNUSED(pdev);
  return USBD_OK;
}

static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev)
{
	UNUSED(pdev);
  return (uint8_t)USBD_OK;
}

void USBD_AUDIO_Sync(USBD_HandleTypeDef *pdev)
{
  USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];
  USBD_AUDIO_ItfTypeDef* itf = pdev->pUserData[pdev->classId];

  if (itf->GetState() == ITF_STOPPED)
  {
  	return;
  }

  AudioBuffer_Sync(&haudio->aud_buf, AUDIO_SYNC_CLK_DIV << 3);

  USBD_AUDIO_UpdateFB(pdev);

  if ((haudio->aud_buf.state == AB_UDFL) && (itf->GetState() == ITF_PLAYING))
	{
		itf->AudioCmd(NULL, 0, AUDIO_CMD_STOP);
	}

  if (haudio->aud_buf.size < haudio->aud_buf.capacity >> 2)
	{
		LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
	}
  else
  {
  	LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
  }
}

static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

	if (epnum == FEEDBACK_EP_NUM)
	{
		USBD_LL_Transmit(pdev, FEEDBACK_EP_ADDR, (uint8_t*)&haudio->feedback_value, FEEDBACK_PACKET_SIZE);
	}

  return USBD_OK;
}

static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

	if (epnum == STREAMING_EP_NUM)
	{
		USBD_LL_PrepareReceive(pdev, STREAMING_EP_ADDR, (uint8_t*)haudio->pkt_buf, USB_HS_MAX_PACKET_SIZE);
	}

  return USBD_OK;
}

static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (epnum == STREAMING_EP_NUM)
  {
    USBD_AUDIO_ItfTypeDef* itf = pdev->pUserData[pdev->classId];

    uint8_t stream_type = USBD_AUDIO_IsDoPStream(pdev);

    if (stream_type != haudio->stream_type)
    {
    	haudio->stream_type = stream_type;
			itf->AudioCmd(&stream_type, sizeof(stream_type), AUDIO_CMD_FORMAT);
    }

		uint32_t packetSize = USBD_LL_GetRxDataSize(pdev, epnum);

		if (stream_type == AUDIO_FORMAT_PCM)
		{
			uint32_t* pDst = (uint32_t*)&haudio->aud_buf.mem[haudio->aud_buf.wr_ptr];
			uint32_t* pSrc = haudio->pkt_buf;

			for (uint32_t i = 0; i < (packetSize >> 2); ++i)
			{
				union
				{
					uint16_t x[2];
					uint32_t y;
				} tmp;

				tmp.y = *pSrc++;
				*pDst++ = (tmp.x[0] << 16) | tmp.x[1];

				if (pDst == (uint32_t*)(haudio->aud_buf.mem + haudio->aud_buf.capacity))
				{
					pDst = (uint32_t*)haudio->aud_buf.mem;
				}
			}
		}
		else
		{

		}

//		// Split DSD channel
//		for (uint16_t i = 0; i < packetSize; i += 8)
//		{
//			uint16_t frameOffset = haudio->aud_buf.wr_ptr + i;
//			haudio->dsd_buf.ch0_mem[haudio->dsd_buf.wr_ptr] = *(uint16_t*)&haudio->aud_buf.mem[frameOffset + DOP_CH0_OFFSET];
//			haudio->dsd_buf.ch1_mem[haudio->dsd_buf.wr_ptr] = *(uint16_t*)&haudio->aud_buf.mem[frameOffset + DOP_CH1_OFFSET];
//
//			if (++haudio->dsd_buf.wr_ptr == haudio->dsd_buf.capacity)
//			{
//				haudio->dsd_buf.wr_ptr = 0;
//			}
//		}

		AudioBuffer_Recieve(&haudio->aud_buf, packetSize);

		if ((itf->GetState() == ITF_STOPPED) && (haudio->aud_buf.size > haudio->aud_buf.capacity >> 1))
		{
			itf->AudioCmd(NULL, 0U, AUDIO_CMD_PLAY);
		}

		if (haudio->aud_buf.size > haudio->aud_buf.capacity - (haudio->aud_buf.capacity >> 2))
		{
			LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);
		}
		else
		{
			LL_GPIO_SetOutputPin(LED3_GPIO_Port, LED3_Pin);
		}

		USBD_LL_PrepareReceive(pdev, STREAMING_EP_ADDR, (uint8_t*)haudio->pkt_buf, USB_HS_MAX_PACKET_SIZE);
  }

  return USBD_OK;
}

static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
    return;
  }

  USBD_memset(haudio->control.data, 0, USB_MAX_EP0_SIZE);

  switch (HIBYTE(req->wIndex))
  {
  case FEATURE_UNIT_ID:
  	break;

  case CLOCK_SOURCE_ID:
  	break;

  default:
  	USBD_CtlError(pdev, req);
  	break;
  }

  USBD_CtlSendData(pdev, haudio->control.data, MIN(req->wLength, USB_MAX_EP0_SIZE));
}

static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
    return;
  }

  if (req->wLength != 0U)
  {
  	haudio->control.cmd = HIBYTE(req->wValue);
  	haudio->control.len = (uint8_t)MIN(req->wLength, USB_MAX_EP0_SIZE);
  	haudio->control.unit = HIBYTE(req->wIndex);

  	USBD_CtlPrepareRx(pdev, haudio->control.data, haudio->control.len);
  }
}

static void AUDIO_REQ_GetRange(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];

	if (haudio == NULL)
	{
		return;
	}

	USBD_memset(haudio->control.data, 0, USB_MAX_EP0_SIZE);

	uint8_t* pbuf = haudio->control.data;

	switch (HIBYTE(req->wIndex))
	{
	case CLOCK_SOURCE_ID:
		if (HIBYTE(req->wValue) == CS_SAM_FREQ_CONTROL)
		{
			SET_DATA(pbuf, uint16_t, 1U);
			SET_DATA(pbuf, uint32_t, AUDIO_MIN_FREQ);
			SET_DATA(pbuf, uint32_t, AUDIO_MAX_FREQ);
			SET_DATA(pbuf, uint32_t, AUDIO_FREQ_RES);
		}
		else
		{
			USBD_CtlError(pdev, req);
			return;
		}
		break;

	case FEATURE_UNIT_ID:
		if (HIBYTE(req->wValue) == FU_VOLUME_CONTROL)
		{
			SET_DATA(pbuf, uint16_t, 1U);
			SET_DATA(pbuf, uint16_t, AUDIO_MIN_VOL);
			SET_DATA(pbuf, uint16_t, AUDIO_MAX_VOL);
			SET_DATA(pbuf, uint16_t, AUDIO_VOL_RES);
		}
		else
		{
			USBD_CtlError(pdev, req);
			return;
		}
		break;

	default:
		USBD_CtlError(pdev, req);
		return;
		break;
	}

	USBD_CtlSendData(pdev, haudio->control.data, MIN(req->wLength, USB_MAX_EP0_SIZE));
}

#ifndef USE_USBD_COMPOSITE

static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = USB_LEN_DEV_QUALIFIER_DESC;

  return (uint8_t*)USBD_AUDIO_DeviceQualifierDesc;
}

#endif /* USE_USBD_COMPOSITE  */

uint8_t USBD_AUDIO_RegisterInterface(USBD_HandleTypeDef *pdev,
                                     USBD_AUDIO_ItfTypeDef *fops)
{
  if (fops == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  pdev->pUserData[pdev->classId] = fops;

  return (uint8_t)USBD_OK;
}

static void *USBD_AUDIO_GetAudioHeaderDesc(uint8_t *pConfDesc)
{
  USBD_ConfigDescTypeDef *desc = (USBD_ConfigDescTypeDef *)(void *)pConfDesc;
  USBD_DescHeaderTypeDef *pdesc = (USBD_DescHeaderTypeDef *)(void *)pConfDesc;
  uint8_t *pAudioDesc =  NULL;
  uint16_t ptr;

  if (desc->wTotalLength > desc->bLength)
  {
    ptr = desc->bLength;

    while (ptr < desc->wTotalLength)
    {
      pdesc = USBD_GetNextDesc((uint8_t *)pdesc, &ptr);
      if ((pdesc->bDescriptorType == CS_INTERFACE) &&
          (pdesc->bDescriptorSubType == HEADER))
      {
        pAudioDesc = (uint8_t *)pdesc;
        break;
      }
    }
  }
  return pAudioDesc;
}
