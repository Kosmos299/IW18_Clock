################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f10x_md.s 

OBJS += \
./startup/startup_stm32f10x_md.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/StdPeriph_Driver/inc" -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/inc" -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/CMSIS/device" -I"D:/My Designs/IW18_Clock/software/Githubwc/trunk/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


