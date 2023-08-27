################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/STM32_USB_Device_Library/Core/Src/usbd_core.c \
../Core/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
../Core/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Core/STM32_USB_Device_Library/Core/Src/usbd_core.o \
./Core/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o \
./Core/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o 

C_DEPS += \
./Core/STM32_USB_Device_Library/Core/Src/usbd_core.d \
./Core/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.d \
./Core/STM32_USB_Device_Library/Core/Src/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Core/STM32_USB_Device_Library/Core/Src/%.o Core/STM32_USB_Device_Library/Core/Src/%.su: ../Core/STM32_USB_Device_Library/Core/Src/%.c Core/STM32_USB_Device_Library/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F446xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/STM32_USB_Device_Library/Core/Inc" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/App" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/Target" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-STM32_USB_Device_Library-2f-Core-2f-Src

clean-Core-2f-STM32_USB_Device_Library-2f-Core-2f-Src:
	-$(RM) ./Core/STM32_USB_Device_Library/Core/Src/usbd_core.d ./Core/STM32_USB_Device_Library/Core/Src/usbd_core.o ./Core/STM32_USB_Device_Library/Core/Src/usbd_core.su ./Core/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.d ./Core/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o ./Core/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.su ./Core/STM32_USB_Device_Library/Core/Src/usbd_ioreq.d ./Core/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o ./Core/STM32_USB_Device_Library/Core/Src/usbd_ioreq.su

.PHONY: clean-Core-2f-STM32_USB_Device_Library-2f-Core-2f-Src

