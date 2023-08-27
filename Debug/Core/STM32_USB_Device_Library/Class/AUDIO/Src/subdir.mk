################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.c 

OBJS += \
./Core/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.o 

C_DEPS += \
./Core/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.d 


# Each subdirectory must supply rules for building sources it contributes
Core/STM32_USB_Device_Library/Class/AUDIO/Src/%.o Core/STM32_USB_Device_Library/Class/AUDIO/Src/%.su: ../Core/STM32_USB_Device_Library/Class/AUDIO/Src/%.c Core/STM32_USB_Device_Library/Class/AUDIO/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-STM32_USB_Device_Library-2f-Class-2f-AUDIO-2f-Src

clean-Core-2f-STM32_USB_Device_Library-2f-Class-2f-AUDIO-2f-Src:
	-$(RM) ./Core/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.d ./Core/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.o ./Core/STM32_USB_Device_Library/Class/AUDIO/Src/usbd_audio.su

.PHONY: clean-Core-2f-STM32_USB_Device_Library-2f-Class-2f-AUDIO-2f-Src

