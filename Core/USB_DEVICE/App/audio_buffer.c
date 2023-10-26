#include "audio_buffer.h"
#include "usbd_audio.h"

__attribute__ ((aligned (4))) static uint8_t s_AudBufMem[AUDIO_BUF_SIZE];

void AudioBuffer_Init(AudioBuffer* ab, uint32_t capacity)
{
	ab->mem = s_AudBufMem;
	AudioBuffer_Reset(ab, capacity);
}

void AudioBuffer_Reset(AudioBuffer* ab, uint32_t capacity)
{
	ab->state = AB_OK;
	ab->size = 0;
	ab->wr_ptr = 0;
	ab->capacity = capacity;
}

uint8_t AudioBuffer_Recieve(AudioBuffer* ab, uint32_t rxSize)
{
	if (ab->size + rxSize <= ab->capacity)
	{
		ab->size += rxSize;
		ab->wr_ptr += rxSize;

		if (ab->wr_ptr >= ab->capacity)
		{
			ab->wr_ptr -= ab->capacity;
		}

		ab->state = AB_OK;
	}
	else
	{
		ab->size = ab->capacity;
		ab->state = AB_OVFL;
	}

	return ab->state;
}

uint8_t AudioBuffer_Sync(AudioBuffer* ab, uint32_t txSize)
{
	if (ab->size >= txSize)
	{
		ab->size -= txSize;
		ab->state = AB_OK;
	}
	else
	{
		ab->size = 0;
		ab->state = AB_UDFL;
	}

	return ab->state;
}





