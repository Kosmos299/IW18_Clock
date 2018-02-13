################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BSP.c \
../src/Driver_ADC.c \
../src/Driver_Core.c \
../src/Driver_I2C.c \
../src/Driver_NVIC.c \
../src/Driver_SPI.c \
../src/Driver_Timer.c \
../src/Driver_UART.c \
../src/Func_Clock.c \
../src/Func_Display.c \
../src/Hasiok.c \
../src/Utility.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

OBJS += \
./src/BSP.o \
./src/Driver_ADC.o \
./src/Driver_Core.o \
./src/Driver_I2C.o \
./src/Driver_NVIC.o \
./src/Driver_SPI.o \
./src/Driver_Timer.o \
./src/Driver_UART.o \
./src/Func_Clock.o \
./src/Func_Display.o \
./src/Hasiok.o \
./src/Utility.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f10x.o 

C_DEPS += \
./src/BSP.d \
./src/Driver_ADC.d \
./src/Driver_Core.d \
./src/Driver_I2C.d \
./src/Driver_NVIC.d \
./src/Driver_SPI.d \
./src/Driver_Timer.d \
./src/Driver_UART.d \
./src/Func_Clock.d \
./src/Func_Display.d \
./src/Hasiok.d \
./src/Utility.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F101C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"D:/My Designs/IW18_Clock/software/IW18_Software.git/trunk/IW18_CMSIS_v01/StdPeriph_Driver/inc" -I"D:/My Designs/IW18_Clock/software/IW18_Software.git/trunk/IW18_CMSIS_v01/inc" -I"D:/My Designs/IW18_Clock/software/IW18_Software.git/trunk/IW18_CMSIS_v01/CMSIS/device" -I"D:/My Designs/IW18_Clock/software/IW18_Software.git/trunk/IW18_CMSIS_v01/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


