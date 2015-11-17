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
all: Sistema_Enfriamiento.axf

# Tool invocations
Sistema_Enfriamiento.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/maxi/LPCXpresso/workspace/CMSISv2p00_LPC17xx/Debug" -Xlinker -Map="Sistema_Enfriamiento.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Sistema_Enfriamiento_Debug.ld" -o "Sistema_Enfriamiento.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Sistema_Enfriamiento.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Sistema_Enfriamiento.axf"; # arm-none-eabi-objcopy -v -O binary "Sistema_Enfriamiento.axf" "Sistema_Enfriamiento.bin" ; # checksum -p LPC1769 -d "Sistema_Enfriamiento.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
