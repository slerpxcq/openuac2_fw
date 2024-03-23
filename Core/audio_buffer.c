#include "audio_buffer.h"
#include "usbd_audio.h"

#define USE_DMA_COPY

#ifdef USE_DMA_COPY
#define DMA_M2M_HANDLE hdma_memtomem_dma2_stream0
extern DMA_HandleTypeDef DMA_M2M_HANDLE;
#endif

__attribute__ ((aligned (4))) static uint8_t s_AudBufMem[AUDIO_BUF_SIZE];
static AudioBuffer instance;

static void MemCopy(uint32_t* pDst, uint32_t* pSrc, uint32_t count)
{
#ifdef USE_DMA_COPY
	// Calling this function with count=0 will cause DMA handle to lock out
	DMA_M2M_HANDLE.Instance->CR |= DMA_SxCR_MINC_Msk;
	HAL_DMA_Start_IT(&DMA_M2M_HANDLE, (uint32_t)pSrc, (uint32_t)pDst, count);
#else
	for (uint32_t i = 0; i < count; ++i)
		*pDst++ = *pSrc++;
#endif
}

static void MemFill(uint32_t *pDst, uint32_t val, uint32_t count)
{
#ifdef USE_DMA_COPY
	// Calling this function with count=0 will cause DMA handle to lock out
	DMA_M2M_HANDLE.Instance->CR &= ~DMA_SxCR_MINC_Msk;
	HAL_DMA_Start_IT(&DMA_M2M_HANDLE, (uint32_t)&val, (uint32_t)pDst, count);
#else
	for (uint32_t i = 0; i < count; ++i)
		*pDst++ = val;
#endif
}

AudioBuffer* AudioBuffer_Instance()
{
	return &instance;
}

uint8_t AudioBuffer_GetState()
{
	return instance.state;
}

void AudioBuffer_Reset(uint32_t capacity)
{
	instance.state = AB_OK;
	instance.size = 0;
	instance.wr_ptr = 0;
	instance.rd_ptr = 0;
	instance.capacity = capacity;
}

void AudioBuffer_Init(uint32_t capacity)
{
	instance.mem = s_AudBufMem;
	AudioBuffer_Reset(capacity);
}

void AudioBuffer_Recieve(uint32_t rxSize)
{
	if (instance.size + rxSize <= instance.capacity)
	{
		instance.size += rxSize;
		instance.wr_ptr += rxSize;

		if (instance.wr_ptr >= instance.capacity)
		{
			instance.wr_ptr -= instance.capacity;
			if (instance.wr_ptr > 0)
			{
				MemCopy((uint32_t*)instance.mem, (uint32_t*)&instance.mem[instance.capacity], instance.wr_ptr >> 2);
			}
		}

		instance.state = AB_OK;
	}
	else
	{
		instance.size = instance.capacity;
		instance.state = AB_OVFL;
	}
}

void AudioBuffer_Sync(uint32_t txSize)
{
	if (instance.size >= txSize)
	{
		//MemFill((uint32_t*)&instance.mem[instance.rd_ptr], 0, txSize >> 2);
		instance.size -= txSize;
		instance.rd_ptr += txSize;

		if (instance.rd_ptr >= instance.capacity)
		{
			instance.rd_ptr -= instance.capacity;
		}

		instance.state = AB_OK;
	}
	else
	{
		instance.size = 0;
		instance.state = AB_UDFL;
	}
}





