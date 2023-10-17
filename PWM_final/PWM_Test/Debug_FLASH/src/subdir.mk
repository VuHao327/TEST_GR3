################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/FTM.c \
../src/GPIO.c \
../src/LPUART.c \
../src/main.c \
../src/my_S32K144.c 

OBJS += \
./src/FTM.o \
./src/GPIO.o \
./src/LPUART.o \
./src/main.o \
./src/my_S32K144.o 

C_DEPS += \
./src/FTM.d \
./src/GPIO.d \
./src/LPUART.d \
./src/main.d \
./src/my_S32K144.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/FTM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


