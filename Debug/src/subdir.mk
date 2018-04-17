################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Driver_ADC.c \
../src/Driver_Core.c \
../src/Driver_GPIO.c \
../src/Driver_I2C.c \
../src/Driver_SPI.c \
../src/Driver_Terminal.c \
../src/Driver_Timer.c \
../src/Func_Boost.c \
../src/Func_Clock.c \
../src/Func_Display.c \
../src/Func_System.c \
../src/Hasiok.c \
../src/Utilities.c \
../src/main.c \
../src/syscalls.c \
../src/system_stm32f10x.c 

OBJS += \
./src/Driver_ADC.o \
./src/Driver_Core.o \
./src/Driver_GPIO.o \
./src/Driver_I2C.o \
./src/Driver_SPI.o \
./src/Driver_Terminal.o \
./src/Driver_Timer.o \
./src/Func_Boost.o \
./src/Func_Clock.o \
./src/Func_Display.o \
./src/Func_System.o \
./src/Hasiok.o \
./src/Utilities.o \
./src/main.o \
./src/syscalls.o \
./src/system_stm32f10x.o 

C_DEPS += \
./src/Driver_ADC.d \
./src/Driver_Core.d \
./src/Driver_GPIO.d \
./src/Driver_I2C.d \
./src/Driver_SPI.d \
./src/Driver_Terminal.d \
./src/Driver_Timer.d \
./src/Func_Boost.d \
./src/Func_Clock.d \
./src/Func_Display.d \
./src/Func_System.d \
./src/Hasiok.d \
./src/Utilities.d \
./src/main.d \
./src/syscalls.d \
./src/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F101C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/StdPeriph_Driver/inc" -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/inc" -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/CMSIS/device" -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


