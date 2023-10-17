################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Clock.c \
../src/Gpio.c \
../src/LPIT.c \
../src/Luart.c \
../src/main.c 

S_UPPER_SRCS += \
../src/Init_Stack.S 

OBJS += \
./src/Clock.o \
./src/Gpio.o \
./src/Init_Stack.o \
./src/LPIT.o \
./src/Luart.o \
./src/main.o 

C_DEPS += \
./src/Clock.d \
./src/Gpio.d \
./src/LPIT.d \
./src/Luart.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/Clock.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS Assembler'
	arm-none-eabi-gcc "@src/Init_Stack.args" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


