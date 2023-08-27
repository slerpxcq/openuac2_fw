################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/USB_DEVICE/Target/usbd_conf.c 

OBJS += \
./Core/USB_DEVICE/Target/usbd_conf.o 

C_DEPS += \
./Core/USB_DEVICE/Target/usbd_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/USB_DEVICE/Target/%.o Core/USB_DEVICE/Target/%.su: ../Core/USB_DEVICE/Target/%.c Core/USB_DEVICE/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F446xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Drivers/STM32_USB_Device_Library/Core/Inc" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/App" -I"D:/Documents/ST/STM32CubeIDE/workspace/f446_usb_audio/Core/USB_DEVICE/Target" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-USB_DEVICE-2f-Target

clean-Core-2f-USB_DEVICE-2f-Target:
	-$(RM) ./Core/USB_DEVICE/Target/usbd_conf.d ./Core/USB_DEVICE/Target/usbd_conf.o ./Core/USB_DEVICE/Target/usbd_conf.su

.PHONY: clean-Core-2f-USB_DEVICE-2f-Target

