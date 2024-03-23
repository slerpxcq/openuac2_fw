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
	uint32_t capacity;
	uint32_t size;
	uint32_t wr_ptr;
	uint32_t rd_ptr;
	uint8_t state;
} AudioBuffer;

//public:
AudioBuffer* AudioBuffer_Instance();
uint8_t AudioBuffer_GetState();
void AudioBuffer_Init(uint32_t);
void AudioBuffer_Reset(uint32_t);
void AudioBuffer_Recieve(uint32_t);
void AudioBuffer_Sync(uint32_t);

#define AudioBuffer_WrPtr() (&AudioBuffer_Instance()->mem[AudioBuffer_Instance()->wr_ptr])

#endif // _AUDIO_BUFFER_H_