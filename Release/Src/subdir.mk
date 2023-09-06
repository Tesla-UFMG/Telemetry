################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DMA_USART.c \
../Src/Failure.c \
../Src/Interface_Master.c \
../Src/can.c \
../Src/dma.c \
../Src/gpio.c \
../Src/iwdg.c \
../Src/lcd_dash.c \
../Src/main.c \
../Src/nextion.c \
../Src/spi.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/system_stm32f1xx.c \
../Src/telemetry.c \
../Src/tim.c \
../Src/usart.c \
../Src/xbee.c 

C_DEPS += \
./Src/DMA_USART.d \
./Src/Failure.d \
./Src/Interface_Master.d \
./Src/can.d \
./Src/dma.d \
./Src/gpio.d \
./Src/iwdg.d \
./Src/lcd_dash.d \
./Src/main.d \
./Src/nextion.d \
./Src/spi.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/system_stm32f1xx.d \
./Src/telemetry.d \
./Src/tim.d \
./Src/usart.d \
./Src/xbee.d 

OBJS += \
./Src/DMA_USART.o \
./Src/Failure.o \
./Src/Interface_Master.o \
./Src/can.o \
./Src/dma.o \
./Src/gpio.o \
./Src/iwdg.o \
./Src/lcd_dash.o \
./Src/main.o \
./Src/nextion.o \
./Src/spi.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/system_stm32f1xx.o \
./Src/telemetry.o \
./Src/tim.o \
./Src/usart.o \
./Src/xbee.o 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Og -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/DMA_USART.d ./Src/DMA_USART.o ./Src/DMA_USART.su ./Src/Failure.d ./Src/Failure.o ./Src/Failure.su ./Src/Interface_Master.d ./Src/Interface_Master.o ./Src/Interface_Master.su ./Src/can.d ./Src/can.o ./Src/can.su ./Src/dma.d ./Src/dma.o ./Src/dma.su ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/iwdg.d ./Src/iwdg.o ./Src/iwdg.su ./Src/lcd_dash.d ./Src/lcd_dash.o ./Src/lcd_dash.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/nextion.d ./Src/nextion.o ./Src/nextion.su ./Src/spi.d ./Src/spi.o ./Src/spi.su ./Src/stm32f1xx_hal_msp.d ./Src/stm32f1xx_hal_msp.o ./Src/stm32f1xx_hal_msp.su ./Src/stm32f1xx_it.d ./Src/stm32f1xx_it.o ./Src/stm32f1xx_it.su ./Src/system_stm32f1xx.d ./Src/system_stm32f1xx.o ./Src/system_stm32f1xx.su ./Src/telemetry.d ./Src/telemetry.o ./Src/telemetry.su ./Src/tim.d ./Src/tim.o ./Src/tim.su ./Src/usart.d ./Src/usart.o ./Src/usart.su ./Src/xbee.d ./Src/xbee.o ./Src/xbee.su

.PHONY: clean-Src

