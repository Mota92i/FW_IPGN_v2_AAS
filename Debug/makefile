################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include phy/subdir.mk
-include mdio/subdir.mk
-include lwip/src/netif/ppp/subdir.mk
-include lwip/src/netif/subdir.mk
-include lwip/src/core/ipv6/subdir.mk
-include lwip/src/core/ipv4/subdir.mk
-include lwip/src/core/subdir.mk
-include lwip/src/api/subdir.mk
-include lwip/port/subdir.mk
-include freertos/freertos_kernel/portable/MemMang/subdir.mk
-include freertos/freertos_kernel/portable/GCC/ARM_CM4F/subdir.mk
-include freertos/freertos_kernel/subdir.mk
-include drivers/subdir.mk
-include device/subdir.mk
-include component/uart/subdir.mk
-include component/serial_manager/subdir.mk
-include component/lists/subdir.mk
-include board/subdir.mk
-include CA_Src/TASK/subdir.mk
-include CA_Src/PROTOCOL/NMEA/subdir.mk
-include CA_Src/PROTOCOL/MODBUS/subdir.mk
-include CA_Src/OS/subdir.mk
-include CA_Src/FW_ID/subdir.mk
-include CA_Src/DRV/WDOG/subdir.mk
-include CA_Src/DRV/SERIAL/subdir.mk
-include CA_Src/DRV/SECURITY/subdir.mk
-include CA_Src/DRV/PWM/subdir.mk
-include CA_Src/DRV/MOTOR/subdir.mk
-include CA_Src/DRV/MLX90316/subdir.mk
-include CA_Src/DRV/I2C/subdir.mk
-include CA_Src/DRV/HDW_TIMER/subdir.mk
-include CA_Src/DRV/FLEX_TIMER/subdir.mk
-include CA_Src/DRV/EEPROM/subdir.mk
-include CA_Src/DRV/DISPLAY/subdir.mk
-include CA_Src/DRV/DEBOUNCER/subdir.mk
-include CA_Src/DRV/BSP/subdir.mk
-include CA_Src/CORE/subdir.mk
-include CA_Src/COMMON/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := IPGN
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: IPGN.axf

# Tool invocations
IPGN.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="IPGN.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -T "SLAVE_ETH_Debug.ld" -o "IPGN.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) IPGN.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "IPGN.axf"; # arm-none-eabi-objcopy -v -O binary "IPGN.axf" "IPGN.bin" ; # checksum -p MK64FX512xxx12 -d "IPGN.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
