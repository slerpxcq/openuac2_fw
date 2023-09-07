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
	uint8_t lock;
	int32_t capacity;
	int32_t size;
	int32_t wr_ptr;
	int32_t rd_ptr;
	AudioBufferState state;
} AudioBuffer;

void AB_Init(AudioBuffer* ab, void* mem, int32_t capacity);
AudioBufferState AB_PreRx(AudioBuffer* ab, int32_t rxSize);
AudioBufferState AB_PostTx(AudioBuffer* ab, int32_t txSize);

#endif // _AUDIO_BUFFER_H_
