################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32_USB_Device_Library/Core/Src/usbd_core.c \
../Drivers/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
../Drivers/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Drivers/STM32_USB_Device_Library/Core/Src/usbd_core.o \
./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o \
./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o 

C_DEPS += \
./Drivers/STM32_USB_Device_Library/Core/Src/usbd_core.d \
./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.d \
./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32_USB_Device_Library/Core/Src/%.o Drivers/STM32_USB_Device_Library/Core/Src/%.su: ../Drivers/STM32_USB_Device_Library/Core/Src/%.c Drivers/STM32_USB_Device_Library/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F446xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Drivers/STM32_USB_Device_Library/Core/Inc" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/App" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/Target" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32_USB_Device_Library-2f-Core-2f-Src

clean-Drivers-2f-STM32_USB_Device_Library-2f-Core-2f-Src:
	-$(RM) ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_core.d ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_core.o ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_core.su ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.d ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.su ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ioreq.d ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o ./Drivers/STM32_USB_Device_Library/Core/Src/usbd_ioreq.su

.PHONY: clean-Drivers-2f-STM32_USB_Device_Library-2f-Core-2f-Src

