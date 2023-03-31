/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_ftm.h"
#include "fsl_i2c.h"
#include "fsl_dspi.h"
#include "fsl_uart.h"
#include "fsl_wdog.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define FTM1_PERIPHERAL FTM1
/* Definition of the clock source frequency */
#define FTM1_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_BusClk)
/* FTM1 interrupt vector ID (number). */
#define FTM1_IRQN FTM1_IRQn
/* FTM1 interrupt vector priority. */
#define FTM1_IRQ_PRIORITY 3
/* FTM1 interrupt handler identifier. */
#define FTM1_DriverIRQHandler FTM1_IRQHandler
/* Definition of peripheral ID */
#define FTM2_PERIPHERAL FTM2
/* Definition of the clock source frequency */
#define FTM2_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_BusClk)
/* FTM2 interrupt vector ID (number). */
#define FTM2_IRQN FTM2_IRQn
/* FTM2 interrupt vector priority. */
#define FTM2_IRQ_PRIORITY 3
/* FTM2 interrupt handler identifier. */
#define FTM2_DriverIRQHandler FTM2_IRQHandler
/* Definition of FTM2 channel/pair number 0 on Edge-aligned PWM */
#define FTM2_MOTOR_PWM_CLK_CHANNEL kFTM_Chnl_0
/* Definition of FTM2 channel/pair number 1 on Edge-aligned PWM */
#define FTM2_LED_PWM_CLK_CHANNEL kFTM_Chnl_1
/* BOARD_InitPeripherals defines for I2C1 */
/* Definition of peripheral ID */
#define I2C1_PERIPHERAL I2C1
/* Definition of the clock source */
#define I2C1_CLOCK_SOURCE I2C1_CLK_SRC
/* Definition of the clock source frequency */
#define I2C1_CLK_FREQ CLOCK_GetFreq(I2C1_CLOCK_SOURCE)
/* BOARD_InitPeripherals defines for SPI0 */
/* Definition of peripheral ID */
#define SPI0_PERIPHERAL SPI0
/* Definition of the clock source */
#define SPI0_CLOCK_SOURCE DSPI0_CLK_SRC
/* Definition of the clock source frequency */
#define SPI0_CLK_FREQ CLOCK_GetFreq(SPI0_CLOCK_SOURCE)
/* Definition of peripheral ID */
#define UART0_PERIPHERAL UART0
/* Definition of the clock source frequency */
#define UART0_CLOCK_SOURCE CLOCK_GetFreq(UART0_CLK_SRC)
/* UART0 interrupt vector ID (number). */
#define UART0_SERIAL_RX_TX_IRQN UART0_RX_TX_IRQn
/* UART0 interrupt handler identifier. */
#define Irq_Uart0_RxTx UART0_RX_TX_IRQHandler
/* UART0 interrupt vector ID (number). */
#define UART0_SERIAL_ERROR_IRQN UART0_ERR_IRQn
/* UART0 interrupt handler identifier. */
#define UART0_SERIAL_ERROR_IRQHANDLER UART0_ERR_IRQHandler
/* Definition of peripheral ID */
#define WDOG_PERIPHERAL WDOG
/* Definition of the clock source frequency */
#define WDOG_CLOCK_SOURCE 1000UL
/* Definition of peripheral ID */
#define FTM0_PERIPHERAL FTM0
/* Definition of the clock source frequency */
#define FTM0_CLOCK_SOURCE CLOCK_GetFreq(kCLOCK_BusClk)
/* FTM0 interrupt vector ID (number). */
#define FTM0_IRQN FTM0_IRQn
/* FTM0 interrupt handler identifier. */
#define FTM0_IRQHANDLER FTM0_IRQHandler
/* Definition of FTM0 channel/pair number 5 on Input Capture */
#define FTM0_VOLTMETRE_INPUT_CHANNEL kFTM_Chnl_5

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const ftm_config_t FTM1_config;
extern const ftm_config_t FTM2_config;
extern const i2c_master_config_t I2C1_config;
extern const dspi_master_config_t SPI0_config;
extern const uart_config_t UART0_config;
extern const wdog_config_t WDOG_config;
extern const ftm_config_t FTM0_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
