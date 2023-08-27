################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/USB_DEVICE/App/usb_device.c \
../Core/Src/USB_DEVICE/App/usbd_audio_if.c \
../Core/Src/USB_DEVICE/App/usbd_desc.c 

OBJS += \
./Core/Src/USB_DEVICE/App/usb_device.o \
./Core/Src/USB_DEVICE/App/usbd_audio_if.o \
./Core/Src/USB_DEVICE/App/usbd_desc.o 

C_DEPS += \
./Core/Src/USB_DEVICE/App/usb_device.d \
./Core/Src/USB_DEVICE/App/usbd_audio_if.d \
./Core/Src/USB_DEVICE/App/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/USB_DEVICE/App/%.o Core/Src/USB_DEVICE/App/%.su: ../Core/Src/USB_DEVICE/App/%.c Core/Src/USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-USB_DEVICE-2f-App

clean-Core-2f-Src-2f-USB_DEVICE-2f-App:
	-$(RM) ./Core/Src/USB_DEVICE/App/usb_device.d ./Core/Src/USB_DEVICE/App/usb_device.o ./Core/Src/USB_DEVICE/App/usb_device.su ./Core/Src/USB_DEVICE/App/usbd_audio_if.d ./Core/Src/USB_DEVICE/App/usbd_audio_if.o ./Core/Src/USB_DEVICE/App/usbd_audio_if.su ./Core/Src/USB_DEVICE/App/usbd_desc.d ./Core/Src/USB_DEVICE/App/usbd_desc.o ./Core/Src/USB_DEVICE/App/usbd_desc.su

.PHONY: clean-Core-2f-Src-2f-USB_DEVICE-2f-App

