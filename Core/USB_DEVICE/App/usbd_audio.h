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
#ifndef USBD_AUDIO_FREQ
/* AUDIO Class Config */
#define USBD_AUDIO_FREQ                               48000U
#endif /* USBD_AUDIO_FREQ */

#ifndef USBD_MAX_NUM_INTERFACES
#define USBD_MAX_NUM_INTERFACES                       1U
#endif /* USBD_AUDIO_FREQ */

#ifndef AUDIO_HS_BINTERVAL
#define AUDIO_HS_BINTERVAL                            0x01U
#endif /* AUDIO_HS_BINTERVAL */

#ifndef AUDIO_FS_BINTERVAL
#define AUDIO_FS_BINTERVAL                            0x01U
#endif /* AUDIO_FS_BINTERVAL */

#define AUDIO_WTOTALLENGTH														60U

#ifndef AUDIO_OUT_EP
#define AUDIO_OUT_EP                                  0x01U
#endif /* AUDIO_OUT_EP */

#define USB_AUDIO_CONFIG_DESC_SIZ                     148U
//#define USB_AUDIO_CONFIG_DESC_SIZ                     112U
#define AUDIO_INTERFACE_DESC_SIZE                     0x09U
#define USB_AUDIO_DESC_SIZ                            0x09U
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09U
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07U

#define AUDIO_DESCRIPTOR_TYPE                         0x21U
#define USB_DEVICE_CLASS_AUDIO                        0x01U
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01U
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02U
#define AUDIO_PROTOCOL_UNDEFINED                      0x00U
#define AUDIO_STREAMING_GENERAL                       0x01U
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02U

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24U
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25U

/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01U
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02U
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03U
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06U

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0CU
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09U
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07U

#define AUDIO_CONTROL_MUTE                            0x0001U

#define AUDIO_FORMAT_TYPE_I                           0x01U
#define AUDIO_FORMAT_TYPE_III                         0x03U

#define AUDIO_ENDPOINT_GENERAL                        0x01U

#define AUDIO_REQ_CUR																	0x01U
#define AUDIO_REQ_RANGE																0x02U
#define AUDIO_REQ_GET_CUR                             0x81U
#define AUDIO_REQ_SET_CUR                             0x01U
#define AUDIO_REQ_GET_RANGE														0x82U

#define AUDIO_OUT_STREAMING_CTRL                      0x02U

#define AUDIO_OUT_TC                                  0x01U
#define AUDIO_IN_TC                                   0x02U


#define AUDIO_OUT_PACKET                              (uint16_t)(((USBD_AUDIO_FREQ * 2U * 2U) / 1000U))
#define AUDIO_DEFAULT_VOLUME                          70U

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define AUDIO_OUT_PACKET_NUM                          80U
/* Total size of the audio transfer buffer */
#define AUDIO_TOTAL_BUF_SIZE                          ((uint16_t)(AUDIO_OUT_PACKET * AUDIO_OUT_PACKET_NUM))

// Audio20 appendix definitions
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
#define CONTROL_EP_ADDR
#define CONTROL_EP_ATTRIB
#define STREAMING_EP_ADDR															0x01
#define STREAMING_EP_ATTRIB														0x05
#define FEEDBACK_EP_ADDR															0x82
#define FEEDBACK_EP_ATTRIB														0x11

#define EP_GENERAL																		0x01

#define SET_DATA(_ptr, _type, _value) \
	*(_type*)_ptr = _value; \
	_ptr += sizeof(_type)

/* Audio Commands enumeration */
typedef enum
{
  AUDIO_CMD_START = 1,
  AUDIO_CMD_PLAY,
  AUDIO_CMD_STOP,
} AUDIO_CMD_TypeDef;


typedef enum
{
  AUDIO_OFFSET_NONE = 0,
  AUDIO_OFFSET_HALF,
  AUDIO_OFFSET_FULL,
  AUDIO_OFFSET_UNKNOWN,
} AUDIO_OffsetTypeDef;
/**
  * @}
  */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef struct
{
  uint8_t cmd;
  uint8_t data[USB_MAX_EP0_SIZE];
  uint8_t len;
  uint8_t unit;
} USBD_AUDIO_ControlTypeDef;


typedef struct
{
  uint32_t alt_setting;
  uint8_t buffer[AUDIO_TOTAL_BUF_SIZE];
  AUDIO_OffsetTypeDef offset;
  uint8_t rd_enable;
  uint16_t rd_ptr;
  uint16_t wr_ptr;
  USBD_AUDIO_ControlTypeDef control;
} USBD_AUDIO_HandleTypeDef;


typedef struct
{
  int8_t (*Init)(uint32_t AudioFreq, uint32_t Volume, uint32_t options);
  int8_t (*DeInit)(uint32_t options);
  int8_t (*AudioCmd)(uint8_t *pbuf, uint32_t size, uint8_t cmd);
  int8_t (*VolumeCtl)(uint8_t vol);
  int8_t (*MuteCtl)(uint8_t cmd);
  int8_t (*PeriodicTC)(uint8_t *pbuf, uint32_t size, uint8_t cmd);
  int8_t (*GetState)(void);
} USBD_AUDIO_ItfTypeDef;

/*
 * Audio Class specification release 1.0
 */

/* Table 4-2: Class-Specific AC Interface Header Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptorSubtype;
  uint16_t          bcdADC;
  uint16_t          wTotalLength;
  uint8_t           bInCollection;
  uint8_t           baInterfaceNr;
} __PACKED USBD_SpeakerIfDescTypeDef;

/* Table 4-3: Input Terminal Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptorSubtype;
  uint8_t           bTerminalID;
  uint16_t          wTerminalType;
  uint8_t           bAssocTerminal;
  uint8_t           bNrChannels;
  uint16_t          wChannelConfig;
  uint8_t           iChannelNames;
  uint8_t           iTerminal;
} __PACKED USBD_SpeakerInDescTypeDef;

/* USB Speaker Audio Feature Unit Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptorSubtype;
  uint8_t           bUnitID;
  uint8_t           bSourceID;
  uint8_t           bControlSize;
  uint16_t          bmaControls;
  uint8_t           iTerminal;
} __PACKED USBD_SpeakerFeatureDescTypeDef;

/* Table 4-4: Output Terminal Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptorSubtype;
  uint8_t           bTerminalID;
  uint16_t          wTerminalType;
  uint8_t           bAssocTerminal;
  uint8_t           bSourceID;
  uint8_t           iTerminal;
} __PACKED USBD_SpeakerOutDescTypeDef;

/* Table 4-19: Class-Specific AS Interface Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptorSubtype;
  uint8_t           bTerminalLink;
  uint8_t           bDelay;
  uint16_t          wFormatTag;
} __PACKED USBD_SpeakerStreamIfDescTypeDef;

/* USB Speaker Audio Type III Format Interface Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptorSubtype;
  uint8_t           bFormatType;
  uint8_t           bNrChannels;
  uint8_t           bSubFrameSize;
  uint8_t           bBitResolution;
  uint8_t           bSamFreqType;
  uint8_t           tSamFreq2;
  uint8_t           tSamFreq1;
  uint8_t           tSamFreq0;
} USBD_SpeakerIIIFormatIfDescTypeDef;

/* Table 4-17: Standard AC Interrupt Endpoint Descriptor */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bEndpointAddress;
  uint8_t           bmAttributes;
  uint16_t          wMaxPacketSize;
  uint8_t           bInterval;
  uint8_t           bRefresh;
  uint8_t           bSynchAddress;
} __PACKED USBD_SpeakerEndDescTypeDef;

/* Table 4-21: Class-Specific AS Isochronous Audio Data Endpoint Descriptor        */
typedef struct
{
  uint8_t           bLength;
  uint8_t           bDescriptorType;
  uint8_t           bDescriptor;
  uint8_t           bmAttributes;
  uint8_t           bLockDelayUnits;
  uint16_t          wLockDelay;
} __PACKED USBD_SpeakerEndStDescTypeDef;

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

void USBD_AUDIO_Sync(USBD_HandleTypeDef *pdev, AUDIO_OffsetTypeDef offset);

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
