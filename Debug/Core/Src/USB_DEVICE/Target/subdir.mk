################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/USB_DEVICE/Target/usbd_conf.c 

OBJS += \
./Core/Src/USB_DEVICE/Target/usbd_conf.o 

C_DEPS += \
./Core/Src/USB_DEVICE/Target/usbd_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/USB_DEVICE/Target/%.o Core/Src/USB_DEVICE/Target/%.su: ../Core/Src/USB_DEVICE/Target/%.c Core/Src/USB_DEVICE/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-USB_DEVICE-2f-Target

clean-Core-2f-Src-2f-USB_DEVICE-2f-Target:
	-$(RM) ./Core/Src/USB_DEVICE/Target/usbd_conf.d ./Core/Src/USB_DEVICE/Target/usbd_conf.o ./Core/Src/USB_DEVICE/Target/usbd_conf.su

.PHONY: clean-Core-2f-Src-2f-USB_DEVICE-2f-Target

