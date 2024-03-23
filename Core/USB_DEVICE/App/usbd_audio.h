/**
  ******************************************************************************
  * @file    usbd_audio.h
  * @author  MCD Application Team
  * @brief   header file for the usbd_audio.c file.
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_AUDIO_H
#define __USB_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"
#include "audio_buffer.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_AUDIO
  * @brief This file is the Header file for usbd_audio.c
  * @{
  */


/** @defgroup USBD_AUDIO_Exported_Defines
  * @{
  */

#define AUDIO_DOP_DETECT_COUNT												16U

// Sync
//#define AUDIO_SYNC_CLK_DIV														64U
#define AUDIO_SYNC_CLK_DIV														 (TIM3->ARR + 1)
#define AUDIO_AUTO_RELOAD_BASE                        80

// Feedback
#define AUDIO_48K_FEEDBACK_VALUE											0x60000
#define AUDIO_44K1_FEEDBACK_VALUE											0x58333
#define AUDIO_FEEDBACK_GAIN                           8

// Control
#define AUDIO_MIN_FREQ																44100U
#define AUDIO_MAX_FREQ																768000U
#define	AUDIO_FREQ_RES																1U
#define AUDIO_MIN_VOL																	0U
#define AUDIO_MAX_VOL																	100U
#define AUDIO_VOL_RES																	1U

#define FEEDBACK_HS_BINTERVAL													4U
#define STREAMING_HS_BINTERVAL                        1U

#define AUDIO_WTOTALLENGTH														60U

#define USB_AUDIO_CONFIG_DESC_SIZE                    201U

#define USB_AUDIO_DESC_SIZE                           0x09U

/* Audio Control Interface Descriptor Subtypes */
#define FORMAT_TYPE_I			                           	0x01U

#define AUDIO_REQ_CUR																	0x01U
#define AUDIO_REQ_RANGE																0x02U

#define FEEDBACK_PACKET_SIZE													4U


#define AUDIO_BUFFER_PACKET_NUM												40U
#define AUDIO_BUF_SIZE                        				(AUDIO_BUFFER_PACKET_NUM * (AUDIO_MAX_FREQ / 1000U))

/* Audio20 appendix definitions */
#define AUDIO_FUNCTION 																AUDIO
#define FUNCTION_SUBCLASS_UNDEFINED 									0x00
#define FUNCTION_PROTOCOL_UNDEFINED										0x00
#define AF_VERSION_02_00															IP_VERSION_02_00
#define AUDIO																					0x01
#define INTERFACE_SUBCLASS_UNDEFINED									0x00
#define AUDIOCONTROL																	0x01
#define AUDIOSTREAMING																0x02
#define INTERFACE_PROTOCOL_UNDEFINED									0x00
#define IP_VERSION_02_00															0x20
#define FUNCTION_SUBCLASS_UNDEFINED										0x00

#define CS_UNDEFINED																	0x20
#define CS_DEVICE																			0x21
#define CS_CONFIGURATION															0x22
#define CS_STRING																			0x23
#define CS_INTERFACE																	0x24
#define CS_ENDPOINT																		0x25

#define CS_SAM_FREQ_CONTROL														0x01
#define FU_MUTE_CONTROL																0x01
#define FU_VOLUME_CONTROL															0x02

#define AC_DESCRIPTOR_UNDEFINED												0x00
#define HEADER																				0x01
#define INPUT_TERMINAL																0x02
#define OUTPUT_TERMINAL																0x03
#define CLOCK_SOURCE																	0x0A
#define FEATURE_UNIT																	0x06

#define AS_DESCRIPTOR_UNDEFINED												0x00
#define AS_GENERAL																		0x01
#define FORMAT_TYPE																		0x02

// Interface definitions
#define AC_INTERFACE_NUM															0x00
#define AS_INTERFACE_NUM															0x01

// Clock source definitions
#define CLOCK_SOURCE_ID																0x04

// Terminal definitions
#define INPUT_TERMINAL_ID															0x01
#define INPUT_TERMINAL_TYPE														0x0101	// USB Streaming; See Termt20 section 2.1
#define OUTPUT_TERMINAL_ID														0x03
#define OUTPUT_TERMINAL_TYPE													0x0301	// Speaker; See Termt20 section 2.3
#define FEATURE_UNIT_ID																0x02

// Endpoint definitions
#define STREAMING_EP_ADDR															0x01
#define STREAMING_EP_ATTRIB														0x05
#define FEEDBACK_EP_ADDR															(STREAMING_EP_ADDR | 0x80)
#define FEEDBACK_EP_ATTRIB														0x11
#define STREAMING_EP_NUM 															(STREAMING_EP_ADDR & 0xF)
#define FEEDBACK_EP_NUM 															(FEEDBACK_EP_ADDR & 0xF)

#define EP_GENERAL																		0x01

#define PACK_DATA(_ptr, _type, _value) do{\
	*(_type*)_ptr = (_value); \
	_ptr += sizeof(_type); } while(0)

/* Audio Commands enumeration */
typedef enum
{
  AUDIO_CMD_PLAY,
	AUDIO_CMD_FORMAT,
  AUDIO_CMD_STOP,
	AUDIO_CMD_FREQ,
	AUDIO_CMD_MUTE,
	AUDIO_CMD_VOLUME,
} AUDIO_CommandTypeDef;

typedef enum
{
	AUDIO_FORMAT_PCM,
	AUDIO_FORMAT_DSD
} AUDIO_FormatTypeDef;

typedef enum
{
	AUDIO_STATE_STOPPED,
	AUDIO_STATE_PLAYING,
} AUDIO_StateTypeDef;

/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */

typedef struct
{
  uint8_t data[USB_MAX_EP0_SIZE];
  uint8_t cmd;
  uint8_t len;
  uint8_t unit;
} USBD_AUDIO_ControlTypeDef;

typedef struct
{
  USBD_AUDIO_ControlTypeDef control;
  uint32_t alt_setting;
  uint32_t sam_freq;
  uint32_t feedback_base;
  uint32_t feedback_value;
  uint8_t bit_depth;
  uint8_t stream_type;
  uint8_t state;
} USBD_AUDIO_HandleTypeDef;

typedef struct
{
  uint8_t (*Init)();
  uint8_t (*DeInit)();
  uint8_t (*AudioCmd)(uint8_t* pbuf, uint32_t size, uint8_t cmd);
  uint8_t (*GetState)();
} USBD_AUDIO_ItfTypeDef;


/**
  * @}
  */



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */

extern USBD_ClassTypeDef USBD_AUDIO;
#define USBD_AUDIO_CLASS &USBD_AUDIO
/**
  * @}
  */

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */
uint8_t USBD_AUDIO_RegisterInterface(USBD_HandleTypeDef *pdev,
                                     USBD_AUDIO_ItfTypeDef *fops);

void USBD_AUDIO_Sync(USBD_HandleTypeDef *pdev);

#define CLAMP(x, y, z) ((x) < (y) ? (y) : (x) > (z) ? (z) : (x))

#ifdef USE_USBD_COMPOSITE
uint32_t USBD_AUDIO_GetEpPcktSze(USBD_HandleTypeDef *pdev, uint8_t If, uint8_t Ep);
#endif /* USE_USBD_COMPOSITE */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* __USB_AUDIO_H */
/**
  * @}
  */

/**
  * @}
  */
