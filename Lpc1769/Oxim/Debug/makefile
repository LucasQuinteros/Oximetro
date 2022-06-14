################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Oxim.axf

# Tool invocations
Oxim.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Guillermo\Documents\LPCXpresso_5.2.2_2108\oximetro\LCD_Library\Debug" -L"C:\Users\Guillermo\Documents\LPCXpresso_5.2.2_2108\oximetro\libUSB_HID\Debug" -L"C:\Users\Guillermo\Documents\LPCXpresso_5.2.2_2108\oximetro\CMSISv2p00_LPC17xx\Debug" -Xlinker -Map="Oxim.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Oxim_Debug.ld" -o "Oxim.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Oxim.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Oxim.axf"; # arm-none-eabi-objcopy -O binary "Oxim.axf" "Oxim.bin" ; checksum -p LPC1769 -d "Oxim.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
