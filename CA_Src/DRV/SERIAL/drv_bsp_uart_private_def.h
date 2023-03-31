/******************************************************************************/
/// \file           drv_bsp_uartprivate_def.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2018-10-11
/// \copyright      Chauvin Arnoux Energy
///
/// \ingroup        
/// \brief          
/// \details        
///                 
/******************************************************************************/

/******************************************************************************/
//  Inclusion checking
/******************************************************************************/
#ifndef _DRV_BSP_UART_PRIVATE_DEF_H_
#define _DRV_BSP_UART_PRIVATE_DEF_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "fsl_uart.h"
#include "fsl_ftm.h"
#include "peripherals.h"
#include "drv_serial.h"
#include "drv_serial_def.h"

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
	struct {
		UART_Type *base;					// Timer base Register
		uint32_t  u32NvicIrq;				// Irq number in NVIC
		uint32_t  u32NvicIrqPriority;		// Irq priority
		uint32_t  u32ClkSrc;				// Irq number in NVIC
		void(* pvoSetRs485HalfOrFull)(uint8_t u8Half);	// Physical port Rx reception
	}sUart;

	struct {
		FTM_Type *base;						// Timer base Register
		uint32_t  u32NvicIrq;				// Irq number in NVIC
		uint32_t  u32ClkSrc;				// Irq number in NVIC
	}sTimer;
}t_uart_feature;

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct t_uart_custom t_uart_custom;
struct t_uart_custom
{
	uint32_t				u32TimerClockFreq;
	uint8_t					u8DataRx;
	// Tx Rx Buffer
	t_uart_xfer       		sRxTxBuf;

	// arg for app
	t_drv_serial_arg  		sArgApp;

	struct{
		void (* voUart) (t_uart_custom *p_args, uint8_t u8Status);
		void (* voTimer)(t_uart_custom *p_args);
	}sProtocolDrvCallback;

	t_uart_feature			const * ps_UartDef;
	uart_transfer_t 		Txfer;
	uart_handle_t 			Txhandle;

	volatile uint8_t        UartWriteInProgress;
	volatile uint8_t        UartReceiveInProgress;
    volatile uint8_t        u8PortOpened;

    struct{
        uint32_t            TimeOutModbusEOF;
        uint32_t            TimeOutFifo;
        uint32_t            TimeOutForOneByte;
    }sRS485;

    uint8_t                 VirtualChannel;

    serial_config_t  		sSerialCfg;
    uart_config_t 			sNxpSerialCfg;
};

#endif /* _DRV_BSP_UART_PRIVATE_DEF_H_ */
