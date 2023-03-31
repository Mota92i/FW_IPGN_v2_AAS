/******************************************************************************/
/// \file           drv_serial_uart_private.h
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
#ifndef _DRV_SERIAL_UART_PRIVATE_H_
#define _DRV_SERIAL_UART_PRIVATE_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "drv_serial_uart.h"
#include "drv_serial.h"
#include "drv_bsp_uart_private_def.h"

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
// MAX UART AVAILABLE
enum{
  DRV_UART_TX_COMPLETE = 0,
  DRV_UART_RX_RECEIVE,
  DRV_UART_ERROR
};

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
void voDrvSerialUart_IrqUartCb (t_uart_custom * p_args, uint8_t u8Status);
void voDrvSerialUart_IrqTimerCb(t_uart_custom * p_args);

int32_t DrvSerial_Open( uint8_t  		 u8Channel,
						serial_config_t  *psSerialCfg,
                        void (* pIrq_Uart_cb)(t_uart_custom * p_args, uint8_t u8Status),
                        void (* pIrq_Timer_cb)(t_uart_custom * p_args),
                        void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer),
                        t_uart_xfer **p_GetTheBuffer );
int32_t DrvSerial_Close( uint8_t u8Channel );
int32_t DrvSerial_Write( uint8_t u8Channel, uint8_t *pDataTx, uint16_t u16LengthTx );
int32_t DrvSerial_WriteInProgress( uint8_t u8Channel );
int32_t DrvSerial_ReceiveInProgress(uint8_t u8Channel);

int32_t DrvSerial_SetTimeOut(uint8_t u8Channel, uint32_t TimeOutModbusEOF, uint32_t TimeOutFifo);
uint32_t DrvSerial_GetData(uint8_t u8Channel, uint8_t GetInfo);
#endif /* _DRV_SERIAL_UART_PRIVATE_H_ */
