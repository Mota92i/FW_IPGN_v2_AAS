################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/fsl_adc16.c \
../drivers/fsl_clock.c \
../drivers/fsl_common.c \
../drivers/fsl_common_arm.c \
../drivers/fsl_dspi.c \
../drivers/fsl_enet.c \
../drivers/fsl_ftm.c \
../drivers/fsl_gpio.c \
../drivers/fsl_i2c.c \
../drivers/fsl_rtc.c \
../drivers/fsl_smc.c \
../drivers/fsl_uart.c \
../drivers/fsl_wdog.c 

OBJS += \
./drivers/fsl_adc16.o \
./drivers/fsl_clock.o \
./drivers/fsl_common.o \
./drivers/fsl_common_arm.o \
./drivers/fsl_dspi.o \
./drivers/fsl_enet.o \
./drivers/fsl_ftm.o \
./drivers/fsl_gpio.o \
./drivers/fsl_i2c.o \
./drivers/fsl_rtc.o \
./drivers/fsl_smc.o \
./drivers/fsl_uart.o \
./drivers/fsl_wdog.o 

C_DEPS += \
./drivers/fsl_adc16.d \
./drivers/fsl_clock.d \
./drivers/fsl_common.d \
./drivers/fsl_common_arm.d \
./drivers/fsl_dspi.d \
./drivers/fsl_enet.d \
./drivers/fsl_ftm.d \
./drivers/fsl_gpio.d \
./drivers/fsl_i2c.d \
./drivers/fsl_rtc.d \
./drivers/fsl_smc.d \
./drivers/fsl_uart.d \
./drivers/fsl_wdog.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FX512VLL12 -DCPU_MK64FX512VLL12_cm4 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=2 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DUSE_RTOS=1 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\drivers" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\port" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\utilities" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\component\serial_manager" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\component\lists" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\component\uart" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\mdio" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\phy" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\freertos\freertos_kernel\include" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\template" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CMSIS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\freertos\template\ARM_CM4F" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\device" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\board" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\source" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\port\arch" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix\arpa" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix\net" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix\sys" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\stdc" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\lwip" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\lwip\priv" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\lwip\prot" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\netif" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\netif\ppp" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\netif\ppp\polarssl" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\CORE" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\OS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\TASK" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\BSP" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\I2C" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\EEPROM" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\DEBOUNCER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\DISPLAY" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\SERIAL" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\COMMON" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\FW_ID" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\SECURITY" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\PROTOCOL" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\PROTOCOL\MODBUS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\PWM" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\MOTOR" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\MLX90316" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\HDW_TIMER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\FLEX_TIMER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\PROTOCOL\NMEA" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


