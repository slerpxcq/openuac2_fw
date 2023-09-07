/**
  ******************************************************************************
  * @file    usbd_audio.c
  * @author  MCD Application Team
  * @brief   This file provides the Audio core functions.
  *
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  * @verbatim
  *
  *          ===================================================================
  *                                AUDIO Class  Description
  *          ===================================================================
  *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
  *           Audio Devices V1.0 Mar 18, 98".
  *           This driver implements the following aspects of the specification:
  *             - Device descriptor management
  *             - Configuration descriptor management
  *             - Standard AC Interface Descriptor management
  *             - 1 Audio Streaming Interface (with single channel, PCM, Stereo mode)
  *             - 1 Audio Streaming Endpoint
  *             - 1 Audio Terminal Input (1 channel)
  *             - Audio Class-Specific AC Interfaces
  *             - Audio Class-Specific AS Interfaces
  *             - AudioControl Requests: only SET_CUR and GET_CUR requests are supported (for Mute)
  *             - Audio Feature Unit (limited to Mute control)
  *             - Audio Synchronization type: Asynchronous
  *             - Single fixed audio sampling rate (configurable in usbd_conf.h file)
  *          The current audio class version supports the following audio features:
  *             - Pulse Coded Modulation (PCM) format
  *             - sampling rate: 48KHz.
  *             - Bit resolution: 16
  *             - Number of channels: 2
  *             - No volume control
  *             - Mute/Unmute capability
  *             - Asynchronous Endpoints
  *
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *
  *
  *  @endverbatim
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}_audio.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbd_audio.h"
#include "usbd_ctlreq.h"
#include "usbd_audio_if.h"
#include "audio_desc.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_ll_usb.h"

#ifdef USE_USBD_COMPOSITE
#error "Composite device is unsupported."
#endif


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_AUDIO
  * @brief usbd core module
  * @{
  */

/** @defgroup USBD_AUDIO_Private_TypesDefinitions
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_AUDIO_Private_Defines
  * @{
  */
/**
  * @}
  */


/** @defgroup USBD_AUDIO_Private_Macros
  * @{
  */

/**
  * @}
  */


/** @defgroup USBD_AUDIO_Private_FunctionPrototypes
  * @{
  */
static uint8_t USBD_AUDIO_PrepareOUT(USBD_HandleTypeDef* pdev, uint8_t epnum, uint8_t incrRxAddr);
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

/**
  * @}
  */

/** @defgroup USBD_AUDIO_Private_Variables
  * @{
  */

extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
extern uint32_t g_TxSampleCnt;

static uint32_t s_SofCnt;
__ALIGN_BEGIN uint32_t s_AudFbVal __ALIGN_END;
__ALIGN_BEGIN static uint8_t s_AudBufMem[AUDIO_TOTAL_BUF_SIZE + AUDIO_OUT_PACKET] __ALIGN_END;

static AudioBuffer s_AudBuf;

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

/**
  * @}
  */

/** @defgroup USBD_AUDIO_Private_Functions
  * @{
  */

static uint8_t USBD_AUDIO_PrepareOUT(USBD_HandleTypeDef* pdev, uint8_t epnum, uint8_t incrRxAddr)
{
	USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

	uint16_t packetSize = (uint16_t)USBD_LL_GetRxDataSize(pdev, epnum);

	if (incrRxAddr != 0U)
	{
		AB_PreRx(haudio->aud_buf, packetSize);
	}

	uint8_t* rxMem = haudio->aud_buf->mem;
	return USBD_LL_PrepareReceive(pdev, STREAMING_EP_ADDR, &rxMem[haudio->aud_buf->wr_ptr], USB_HS_MAX_PACKET_SIZE);
}

/**
  * @brief  USBD_AUDIO_Init
  *         Initialize the AUDIO interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_AUDIO_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);
  USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)USBD_malloc(sizeof(USBD_AUDIO_HandleTypeDef));

  if (haudio == NULL)
  {
  	pdev->pClassDataCmsit[pdev->classId] = NULL;
    return (uint8_t)USBD_EMEM;
  }

  pdev->pClassDataCmsit[pdev->classId] = (void *)haudio;
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

  // Enable SOF interrupt
  hpcd_USB_OTG_HS.Instance->GINTMSK |= USB_OTG_GINTMSK_SOFM;

  USBD_LL_FlushEP(pdev, STREAMING_EP_ADDR);
  USBD_LL_FlushEP(pdev, FEEDBACK_EP_ADDR);
  USBD_LL_OpenEP(pdev, STREAMING_EP_ADDR, USBD_EP_TYPE_ISOC, USB_HS_MAX_PACKET_SIZE);
  USBD_LL_OpenEP(pdev, FEEDBACK_EP_ADDR, USBD_EP_TYPE_ISOC, FEEDBACK_PACKET_SIZE);
  pdev->ep_out[STREAMING_EP_NUM].is_used = 1U;
  pdev->ep_in[FEEDBACK_EP_NUM].is_used = 1U;

  haudio->alt_setting = 0U;
  haudio->aud_buf = &s_AudBuf;
  AB_Init(haudio->aud_buf, s_AudBufMem, AUDIO_TOTAL_BUF_SIZE);

  /* Initialize the Audio output Hardware layer */
  USBD_AUDIO_ItfTypeDef* aud_itf = pdev->pUserData[pdev->classId];
  if (aud_itf->Init(0, 0, 0) != USBD_OK)
  {
    return (uint8_t)USBD_FAIL;
  }

  /* Prepare Out endpoint to receive 1st packet */
  uint8_t* rxMem = haudio->aud_buf->mem;
  USBD_LL_PrepareReceive(pdev, STREAMING_EP_ADDR, &rxMem[haudio->aud_buf->wr_ptr], USB_HS_MAX_PACKET_SIZE);
  USBD_LL_Transmit(pdev, FEEDBACK_EP_ADDR, (void*)&s_AudFbVal, FEEDBACK_PACKET_SIZE);

  return USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Init
  *         DeInitialize the AUDIO layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t USBD_AUDIO_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  /* Open EP OUT */
  USBD_LL_CloseEP(pdev, STREAMING_EP_ADDR);
  USBD_LL_CloseEP(pdev, FEEDBACK_EP_ADDR);
  pdev->ep_out[STREAMING_EP_NUM].is_used = 0U;
  pdev->ep_out[STREAMING_EP_NUM].bInterval = 0U;
  pdev->ep_out[FEEDBACK_EP_NUM].is_used = 0U;
	pdev->ep_out[FEEDBACK_EP_NUM].bInterval = 0U;

  /* DeInit  physical Interface components */
  if (pdev->pClassDataCmsit[pdev->classId] != NULL)
  {
    ((USBD_AUDIO_ItfTypeDef *)pdev->pUserData[pdev->classId])->DeInit(0U);
    (void)USBD_free(pdev->pClassDataCmsit[pdev->classId]);
    pdev->pClassDataCmsit[pdev->classId] = NULL;
    pdev->pClassData = NULL;
  }

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_AUDIO_Setup
  *         Handle the AUDIO specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t USBD_AUDIO_Setup(USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  uint16_t len;
  uint8_t *pbuf;
  uint16_t status_info = 0U;
  USBD_StatusTypeDef ret = USBD_OK;

  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
  	ret = USBD_FAIL;
  	goto Exit;
  }

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
					AUDIO_REQ_GetRange(pdev, req);
					break;

				default:
					USBD_CtlError(pdev, req);
					ret = USBD_FAIL;
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
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_GET_DESCRIPTOR:
          if ((req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
          {
          	pbuf = (uint8_t *)USBD_AUDIO_GetAudioHeaderDesc(pdev->pConfDesc);
            if (pbuf != NULL)
            {
              len = MIN(USB_AUDIO_DESC_SIZ, req->wLength);
              (void)USBD_CtlSendData(pdev, pbuf, len);
            }
            else
            {
              USBD_CtlError(pdev, req);
              ret = USBD_FAIL;
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
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_SET_INTERFACE:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
          {
            if ((uint8_t)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
            {
              haudio->alt_setting = (uint8_t)(req->wValue);
            }
            else
            {
              /* Call the error management function (command will be NAKed */
              USBD_CtlError(pdev, req);
              ret = USBD_FAIL;
            }
          }
          else
          {
            USBD_CtlError(pdev, req);
            ret = USBD_FAIL;
          }
          break;

        case USB_REQ_CLEAR_FEATURE:
          break;

        default:
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
          break;
      }
      break;
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      break;
  }

Exit:
  USBD_DbgLog("%s: bmRequest=%u, bRequest=%u, status=%u", __FUNCTION__, req->bmRequest, req->bRequest, ret);
  return (uint8_t)ret;
}

#ifndef USE_USBD_COMPOSITE
/**
  * @brief  USBD_AUDIO_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_AUDIO_GetCfgDesc(uint16_t *length)
{
//  *length = (uint16_t)sizeof(USBD_AUDIO_CfgDesc);
	*length = USB_AUDIO_CONFIG_DESC_SIZ;

  return USBD_AUDIO_CfgDesc;
}
#endif /* USE_USBD_COMPOSITE  */
/**
  * @brief  USBD_AUDIO_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	if (epnum == FEEDBACK_EP_NUM)
	{
		USBD_LL_Transmit(pdev, FEEDBACK_EP_ADDR, (void*)&s_AudFbVal, FEEDBACK_PACKET_SIZE);
		LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_4);
	}

  return (uint8_t)USBD_OK;
}

/**
  * @brief  USBD_AUDIO_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_AUDIO_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];
  USBD_AUDIO_ItfTypeDef* aud_itf = pdev->pUserData[pdev->classId];

  if (haudio == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (haudio->control.unit == FEATURE_UNIT_ID)
  {
    if (haudio->control.cmd == AUDIO_REQ_SET_CUR)
    {
    	aud_itf->MuteCtl(haudio->control.data[0]);
    }
  }

  haudio->control.cmd = 0U;
	haudio->control.len = 0U;

  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_AUDIO_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_AUDIO_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
	UNUSED(pdev);
  return (uint8_t)USBD_OK;
}
/**
  * @brief  USBD_AUDIO_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t USBD_AUDIO_SOF(USBD_HandleTypeDef *pdev)
{
	if (++s_SofCnt == 8192U)
	{
		s_AudFbVal = g_TxSampleCnt << 2U;
		s_SofCnt = 0U;
		g_TxSampleCnt = 0U;
	}

  return (uint8_t)USBD_OK;
}

// Audio hardware DMA transmit complete callback
void USBD_AUDIO_Sync(USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset)
{
  if (pdev->pClassDataCmsit[pdev->classId] == NULL)
  {
    return;
  }

  USBD_AUDIO_HandleTypeDef* haudio = pdev->pClassDataCmsit[pdev->classId];
  USBD_AUDIO_ItfTypeDef* aud_itf = pdev->pUserData[pdev->classId];

  AB_PostTx(haudio->aud_buf, 4U);

  if (haudio->aud_buf->state == AB_UDFL)
	{
		LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
	}
	else
	{
		LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
	}

  if (haudio->aud_buf->state == AB_UDFL)
  {
  	if (aud_itf->GetState() == ITF_PLAYING)
  	{
  		aud_itf->AudioCmd(NULL, 0, AUDIO_CMD_STOP);
  	}
  }
}

/**
  * @brief  USBD_AUDIO_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	if (epnum == FEEDBACK_EP_NUM)
	{
		USBD_LL_Transmit(pdev, FEEDBACK_EP_ADDR, (void*)&s_AudFbVal, FEEDBACK_PACKET_SIZE);
	}

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	if (epnum == STREAMING_EP_NUM)
	{
		USBD_AUDIO_PrepareOUT(pdev, epnum, 0U);
	}

  return USBD_OK;
}
/**
  * @brief  USBD_AUDIO_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t USBD_AUDIO_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  USBD_AUDIO_HandleTypeDef* haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
    return (uint8_t)USBD_FAIL;
  }

  if (epnum == STREAMING_EP_NUM)
  {
    USBD_AUDIO_ItfTypeDef* aud_itf = pdev->pUserData[pdev->classId];

		if (haudio->aud_buf->size >= haudio->aud_buf->capacity >> 1U)
		{
			if (aud_itf->GetState() == ITF_STOPPED)
			{
				aud_itf->AudioCmd(NULL, 0, AUDIO_CMD_PLAY);
			}
		}

		USBD_AUDIO_PrepareOUT(pdev, epnum, 1U);

		if (haudio->aud_buf->state == AB_OVFL)
		{
			LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
		}
		else
		{
			LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
		}
  }

  return USBD_OK;
}

/**
  * @brief  AUDIO_Req_GetCurrent
  *         Handles the GET_CUR Audio control request.
  * @param  pdev: device instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
    return;
  }

  (void)USBD_memset(haudio->control.data, 0, USB_MAX_EP0_SIZE);

  /* Send the current mute state */
  (void)USBD_CtlSendData(pdev, haudio->control.data,
                         MIN(req->wLength, USB_MAX_EP0_SIZE));
}

/**
  * @brief  AUDIO_Req_SetCurrent
  *         Handles the SET_CUR Audio control request.
  * @param  pdev: device instance
  * @param  req: setup class request
  * @retval status
  */
static void AUDIO_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_AUDIO_HandleTypeDef *haudio;
  haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (haudio == NULL)
  {
    return;
  }

  if (req->wLength != 0U)
  {
    haudio->control.cmd = AUDIO_REQ_SET_CUR;     /* Set the request value */
    haudio->control.len = (uint8_t)MIN(req->wLength, USB_MAX_EP0_SIZE);  /* Set the request data length */
    haudio->control.unit = HIBYTE(req->wIndex);  /* Set the request target unit */

    /* Prepare the reception of the buffer over EP0 */
    (void)USBD_CtlPrepareRx(pdev, haudio->control.data, haudio->control.len);
  }
}

static void AUDIO_REQ_GetRange(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	USBD_AUDIO_HandleTypeDef *haudio;
	haudio = (USBD_AUDIO_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

	if (haudio == NULL)
	{
		return;
	}

	USBD_memset(haudio->control.data, 0, USB_MAX_EP0_SIZE);

	USBD_DbgLog("%s: id=%u, CS=%u", __FUNCTION__, HIBYTE(req->wIndex), HIBYTE(req->wValue));

	switch (HIBYTE(req->wIndex))
	{
	case CLOCK_SOURCE_ID:
		if (HIBYTE(req->wValue) == CS_SAM_FREQ_CONTROL)
		{
			uint8_t* pData = haudio->control.data;

			// See Audio20 5.2.3.3
			SET_DATA(pData, uint16_t, 1U);				// wNumSubRanges
			SET_DATA(pData, uint32_t, USBD_AUDIO_FREQ);		// wMIN(1)
			SET_DATA(pData, uint32_t, USBD_AUDIO_FREQ);		// wMAX(1)
			SET_DATA(pData, uint32_t, 1U);				// wRES(1)

			USBD_CtlSendData(pdev, haudio->control.data, MIN(req->wLength, USB_MAX_EP0_SIZE));
		}
		else
		{
			USBD_CtlError(pdev, req);
		}
		break;
	default:
		USBD_CtlError(pdev, req);
		break;
	}
}

#ifndef USE_USBD_COMPOSITE
/**
  * @brief  DeviceQualifierDescriptor
  *         return Device Qualifier descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t *USBD_AUDIO_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = USB_LEN_DEV_QUALIFIER_DESC;

  return USBD_AUDIO_DeviceQualifierDesc;
}
#endif /* USE_USBD_COMPOSITE  */
/**
  * @brief  USBD_AUDIO_RegisterInterface
  * @param  pdev: device instance
  * @param  fops: Audio interface callback
  * @retval status
  */
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

/**
  * @brief  USBD_AUDIO_GetAudioHeaderDesc
  *         This function return the Audio descriptor
  * @param  pdev: device instance
  * @param  pConfDesc:  pointer to Bos descriptor
  * @retval pointer to the Audio AC Header descriptor
  */
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
      if ((pdesc->bDescriptorType == AUDIO_INTERFACE_DESCRIPTOR_TYPE) &&
          (pdesc->bDescriptorSubType == AUDIO_CONTROL_HEADER))
      {
        pAudioDesc = (uint8_t *)pdesc;
        break;
      }
    }
  }
  return pAudioDesc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */