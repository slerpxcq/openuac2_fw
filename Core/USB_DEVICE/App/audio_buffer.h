#ifndef _AUDIO_BUFFER_H_
#define _AUDIO_BUFFER_H_

#include <stdint.h>

typedef enum
{
	AB_OK,
	AB_OVFL,
	AB_UDFL
} AudioBufferState;

typedef struct
{
	uint8_t* mem;
	int32_t capacity;
	int32_t size;
	int32_t wr_ptr;
	uint8_t state;
} AudioBuffer;

void AudioBuffer_Init(AudioBuffer* ab, uint32_t capacity);
void AudioBuffer_Reset(AudioBuffer* ab, uint32_t capacity);
uint8_t AudioBuffer_Recieve(AudioBuffer* ab, uint32_t rxSize);
uint8_t AudioBuffer_Sync(AudioBuffer* ab, uint32_t txSize);

#endif // _AUDIO_BUFFER_H_
