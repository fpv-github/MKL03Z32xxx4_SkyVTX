################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/subdir.mk
-include drivers/subdir.mk
-include board/subdir.mk
-include CMSIS/subdir.mk
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
all: MKL03Z32xxx4_SkyVTX.axf

# Tool invocations
MKL03Z32xxx4_SkyVTX.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="MKL03Z32xxx4_SkyVTX.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -mcpu=cortex-m0plus -mthumb -T "MKL03Z32xxx4_SkyVTX_Debug.ld" -o "MKL03Z32xxx4_SkyVTX.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) MKL03Z32xxx4_SkyVTX.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "MKL03Z32xxx4_SkyVTX.axf"; # arm-none-eabi-objcopy -v -O binary "MKL03Z32xxx4_SkyVTX.axf" "MKL03Z32xxx4_SkyVTX.bin" ; # checksum -p MKL03Z32xxx4 -d "MKL03Z32xxx4_SkyVTX.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
