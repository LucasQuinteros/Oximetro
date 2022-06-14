################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hiduser.c \
../src/usbcore.c \
../src/usbdesc.c \
../src/usbhid.c \
../src/usbhw.c \
../src/usbuser.c 

OBJS += \
./src/hiduser.o \
./src/usbcore.o \
./src/usbdesc.o \
./src/usbhid.o \
./src/usbhw.o \
./src/usbuser.o 

C_DEPS += \
./src/hiduser.d \
./src/usbcore.d \
./src/usbdesc.d \
./src/usbhid.d \
./src/usbhw.d \
./src/usbuser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\Guillermo\Documents\LPCXpresso_5.2.2_2108\oximetro\libUSB_HID\inc" -I"C:\Users\Guillermo\Documents\LPCXpresso_5.2.2_2108\oximetro\CMSISv2p00_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


