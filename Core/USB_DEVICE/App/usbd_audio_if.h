/**
  ******************************************************************************
  * @file           : usbd_audio_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_audio_if.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __USBD_AUDIO_IF_H__
#define __USBD_AUDIO_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "usbd_audio.h"

#define PLLI2SQ_48K		98304000UL
#define PLLI2SQ_44K1	90316800UL

#define AUDIO_I2S_MSTR_HANDLE				hi2s3
#define AUDIO_I2S_SLAVE_HANDLE			hi2s1
#define AUDIO_I2S_MSTR_DMA_HANDLE		hdma_spi3_tx
#define AUDIO_I2S_SLAVE_DMA_HANDLE	hdma_spi1_tx

extern USBD_AUDIO_ItfTypeDef USBD_AUDIO_fops;

typedef enum _AudioItfState
{
	ITF_PLAYING,
	ITF_STOPPED
} AudioItfState;

typedef struct
{
	uint8_t (*Init)(void);
	uint8_t (*DeInit)(void);
	uint8_t (*Play)(void);
	uint8_t (*Format)(uint8_t);
	uint8_t (*Stop)(void);
	uint8_t (*Freq)(uint32_t);
	uint8_t (*Mute)(uint8_t);
	uint8_t (*Volume)(uint8_t);
} AUDIO_CodecTypeDef;

#ifdef __cplusplus
}
#endif

#endif /* __USBD_AUDIO_IF_H__ */
