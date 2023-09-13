#include "audio_buffer.h"
#include <string.h>

void AudioBuffer_Init(AudioBuffer* ab, void* mem, int32_t capacity)
{
	ab->mem = mem;
	AudioBuffer_Reset(ab, capacity);
}

void AudioBuffer_Reset(AudioBuffer* ab, int32_t capacity)
{
	ab->state = AB_OK;
	ab->size = 0;
	ab->wr_ptr = 0;
	ab->rd_ptr = 0;
	ab->capacity = capacity;
}

AudioBufferState AudioBuffer_PostRecieve(AudioBuffer* ab, int32_t rxSize)
{
	if (ab->size + rxSize < ab->capacity)
	{
		ab->size += rxSize;
		ab->state = AB_OK;
	}
	else
	{
		ab->state = AB_OVFL;
		goto Exit;
	}

	int32_t nxt_wr_ptr = ab->wr_ptr + rxSize;
	if (nxt_wr_ptr >= ab->capacity)
	{
		int32_t ovw_cnt = nxt_wr_ptr - ab->capacity;
		memcpy(ab->mem, ab->mem + ab->capacity, ovw_cnt);
		ab->wr_ptr = nxt_wr_ptr - ab->capacity;
	}
	else
	{
		ab->wr_ptr = nxt_wr_ptr;
	}

Exit:
	return ab->state;
}

AudioBufferState AudioBuffer_PostTransmit(AudioBuffer* ab, int32_t txSize)
{
	if (ab->size - txSize >= 0)
	{
		ab->size -= txSize;
		ab->state = AB_OK;
	}
	else
	{
		ab->state = AB_UDFL;
		goto Exit;
	}

	ab->rd_ptr += txSize;
	if (ab->rd_ptr == ab->capacity)
		ab->rd_ptr = 0;

Exit:
	return ab->state;
}





