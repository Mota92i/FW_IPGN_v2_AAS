################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CA_Src/DRV/MsDlyTmr/drv_msDlyTmr.c 

OBJS += \
./CA_Src/DRV/MsDlyTmr/drv_msDlyTmr.o 

C_DEPS += \
./CA_Src/DRV/MsDlyTmr/drv_msDlyTmr.d 


# Each subdirectory must supply rules for building sources it contributes
CA_Src/DRV/MsDlyTmr/%.o: ../CA_Src/DRV/MsDlyTmr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FX512VLL12 -DCPU_MK64FX512VLL12_cm4 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=2 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DUSE_RTOS=1 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\drivers" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\port" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\utilities" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\component\serial_manager" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\component\lists" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\component\uart" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\mdio" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\phy" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\freertos\freertos_kernel\include" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\template" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CMSIS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\freertos\template\ARM_CM4F" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\device" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\board" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\source" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\port\arch" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\compat\posix\arpa" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\compat\posix\net" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\compat\posix" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\compat\posix\sys" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\compat\stdc" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\lwip" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\lwip\priv" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\lwip\prot" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\netif" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\netif\ppp" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\lwip\src\include\netif\ppp\polarssl" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\CORE" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\OS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\TASK" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\BSP" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\I2C" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\EEPROM" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\DEBOUNCER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\MsDlyTmr" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\DISPLAY" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\SERIAL" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\COMMON" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\FW_ID" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\SECURITY" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\PROTOCOL" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\PROTOCOL\MODBUS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\PWM" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\MOTOR" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.4.0_6237\workspace\IPGN\CA_Src\DRV\MLX90316" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


