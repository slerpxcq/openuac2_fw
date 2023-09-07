################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/USB_DEVICE/App/audio_buffer.c \
../Core/USB_DEVICE/App/audio_desc.c \
../Core/USB_DEVICE/App/usb_device.c \
../Core/USB_DEVICE/App/usbd_audio.c \
../Core/USB_DEVICE/App/usbd_audio_if.c \
../Core/USB_DEVICE/App/usbd_desc.c 

OBJS += \
./Core/USB_DEVICE/App/audio_buffer.o \
./Core/USB_DEVICE/App/audio_desc.o \
./Core/USB_DEVICE/App/usb_device.o \
./Core/USB_DEVICE/App/usbd_audio.o \
./Core/USB_DEVICE/App/usbd_audio_if.o \
./Core/USB_DEVICE/App/usbd_desc.o 

C_DEPS += \
./Core/USB_DEVICE/App/audio_buffer.d \
./Core/USB_DEVICE/App/audio_desc.d \
./Core/USB_DEVICE/App/usb_device.d \
./Core/USB_DEVICE/App/usbd_audio.d \
./Core/USB_DEVICE/App/usbd_audio_if.d \
./Core/USB_DEVICE/App/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/USB_DEVICE/App/%.o Core/USB_DEVICE/App/%.su: ../Core/USB_DEVICE/App/%.c Core/USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F446xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Drivers/STM32_USB_Device_Library/Core/Inc" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/App" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/Target" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-USB_DEVICE-2f-App

clean-Core-2f-USB_DEVICE-2f-App:
	-$(RM) ./Core/USB_DEVICE/App/audio_buffer.d ./Core/USB_DEVICE/App/audio_buffer.o ./Core/USB_DEVICE/App/audio_buffer.su ./Core/USB_DEVICE/App/audio_desc.d ./Core/USB_DEVICE/App/audio_desc.o ./Core/USB_DEVICE/App/audio_desc.su ./Core/USB_DEVICE/App/usb_device.d ./Core/USB_DEVICE/App/usb_device.o ./Core/USB_DEVICE/App/usb_device.su ./Core/USB_DEVICE/App/usbd_audio.d ./Core/USB_DEVICE/App/usbd_audio.o ./Core/USB_DEVICE/App/usbd_audio.su ./Core/USB_DEVICE/App/usbd_audio_if.d ./Core/USB_DEVICE/App/usbd_audio_if.o ./Core/USB_DEVICE/App/usbd_audio_if.su ./Core/USB_DEVICE/App/usbd_desc.d ./Core/USB_DEVICE/App/usbd_desc.o ./Core/USB_DEVICE/App/usbd_desc.su

.PHONY: clean-Core-2f-USB_DEVICE-2f-App

