# OpenUAC2
An open-source audio streaming bridge compatible with USB Audio 2.0 specification

## Features
- Full USB 2.0 HS support
- Asynchronous feedback
- Very low cost: no external programmable logic required
- Easy interfacing with DACs
- PCM supports: **44.1k, 48k, 88.2k, 96k, 176.4k, 192k, 352.8k, 384k**
- DoP supports: **DSD64, DSD128**

## Current limitations
- DSD native is unsupported due to driver
- Manual reset may be required after reconnecting USB cable

## Implementation details
For details of ST's USB device library, please refer to [UM1734](https://www.st.com/resource/en/user_manual/um1734-stm32cube-usb-device-library-stmicroelectronics.pdf)

These files contain the main work of this project
- [audio_desc.c](audio_desc.c): USB configuration descriptor
- [usbd_audio.c](usbd_audio.c): USB interrupt callbacks
- [audio_buffer.c](audio_buffer.c): Circular buffer
- [usbd_audio_if.c](usbd_audio_if.c): Peripheral and external DAC interface

