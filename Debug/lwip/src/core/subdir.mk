################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lwip/src/core/altcp.c \
../lwip/src/core/altcp_alloc.c \
../lwip/src/core/altcp_tcp.c \
../lwip/src/core/def.c \
../lwip/src/core/dns.c \
../lwip/src/core/inet_chksum.c \
../lwip/src/core/init.c \
../lwip/src/core/ip.c \
../lwip/src/core/mem.c \
../lwip/src/core/memp.c \
../lwip/src/core/netif.c \
../lwip/src/core/pbuf.c \
../lwip/src/core/raw.c \
../lwip/src/core/stats.c \
../lwip/src/core/sys.c \
../lwip/src/core/tcp.c \
../lwip/src/core/tcp_in.c \
../lwip/src/core/tcp_out.c \
../lwip/src/core/timeouts.c \
../lwip/src/core/udp.c 

OBJS += \
./lwip/src/core/altcp.o \
./lwip/src/core/altcp_alloc.o \
./lwip/src/core/altcp_tcp.o \
./lwip/src/core/def.o \
./lwip/src/core/dns.o \
./lwip/src/core/inet_chksum.o \
./lwip/src/core/init.o \
./lwip/src/core/ip.o \
./lwip/src/core/mem.o \
./lwip/src/core/memp.o \
./lwip/src/core/netif.o \
./lwip/src/core/pbuf.o \
./lwip/src/core/raw.o \
./lwip/src/core/stats.o \
./lwip/src/core/sys.o \
./lwip/src/core/tcp.o \
./lwip/src/core/tcp_in.o \
./lwip/src/core/tcp_out.o \
./lwip/src/core/timeouts.o \
./lwip/src/core/udp.o 

C_DEPS += \
./lwip/src/core/altcp.d \
./lwip/src/core/altcp_alloc.d \
./lwip/src/core/altcp_tcp.d \
./lwip/src/core/def.d \
./lwip/src/core/dns.d \
./lwip/src/core/inet_chksum.d \
./lwip/src/core/init.d \
./lwip/src/core/ip.d \
./lwip/src/core/mem.d \
./lwip/src/core/memp.d \
./lwip/src/core/netif.d \
./lwip/src/core/pbuf.d \
./lwip/src/core/raw.d \
./lwip/src/core/stats.d \
./lwip/src/core/sys.d \
./lwip/src/core/tcp.d \
./lwip/src/core/tcp_in.d \
./lwip/src/core/tcp_out.d \
./lwip/src/core/timeouts.d \
./lwip/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
lwip/src/core/%.o: ../lwip/src/core/%.c lwip/src/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FX512VLL12 -DCPU_MK64FX512VLL12_cm4 -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -DSDK_DEBUGCONSOLE=2 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DUSE_RTOS=1 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\drivers" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\port" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\utilities" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\component\serial_manager" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\component\lists" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\component\uart" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\mdio" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\phy" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\freertos\freertos_kernel\include" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\template" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CMSIS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\freertos\template\ARM_CM4F" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\device" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\board" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\source" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\port\arch" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix\arpa" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix\net" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\posix\sys" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\compat\stdc" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\lwip" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\lwip\priv" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\lwip\prot" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\netif" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\netif\ppp" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\lwip\src\include\netif\ppp\polarssl" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\CORE" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\OS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\TASK" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\BSP" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\I2C" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\EEPROM" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\DEBOUNCER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\DISPLAY" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\SERIAL" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\COMMON" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\FW_ID" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\SECURITY" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\PROTOCOL" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\PROTOCOL\MODBUS" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\PWM" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\MOTOR" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\MLX90316" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\HDW_TIMER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\DRV\FLEX_TIMER" -I"C:\Users\aseervatham\Documents\MCUXpressoIDE_11.5.0_7232\WorkSpace_Ipgn\IPGN\CA_Src\PROTOCOL\NMEA" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


