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
	int32_t rd_ptr;
	AudioBufferState state;
} AudioBuffer;

void AudioBuffer_Init(AudioBuffer* ab, void* mem, int32_t capacity);
void AudioBuffer_Reset(AudioBuffer* ab, int32_t capacity);
AudioBufferState AudioBuffer_PostRecieve(AudioBuffer* ab, int32_t rxSize);
AudioBufferState AudioBuffer_PostTransmit(AudioBuffer* ab, int32_t txSize);

#endif // _AUDIO_BUFFER_H_
