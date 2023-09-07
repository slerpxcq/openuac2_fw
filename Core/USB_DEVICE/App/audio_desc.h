#ifndef _AUDIO_DESC_H_
#define _AUDIO_DESC_H_

#include <stdint.h>

#define __ALIGN_BEGIN
#define __ALIGN_END    __attribute__ ((aligned (4)))

extern uint8_t USBD_AUDIO_CfgDesc[];
extern uint8_t USBD_AUDIO_DeviceQualifierDesc[];

#endif // _AUDIO_DESC_H_
