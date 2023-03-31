/******************************************************************************/
/// \file           drv_serial_uart.h
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
#ifndef _DRV_SERIAL_UART_H_
#define _DRV_SERIAL_UART_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
    UART_MSG_ID_NO_MSG 			= 0,
    UART_MSG_ID_RX_RECEIVED,
	UART_MSG_ID_TX_COMPLETED,
    UART_MSG_ID_BREAK_START,
    UART_MSG_ID_BREAK_END,
    UART_MSG_ID_IDLE,

	UART_MSG_ID_UART_CONFIG_CHANGED,
	UART_MSG_ID_STOP_MODBUS,
	UART_MSG_ID_START_MODBUS,
};

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct
{
    uint8_t  u8Buf[512];
    uint16_t u16SizeMax;
    uint16_t u16Idx;
    uint8_t  u8TxInProgress;
    uint16_t u16Ptr;
}
t_uart_Rx_Tx_xfer;

typedef struct
{
    t_uart_Rx_Tx_xfer sRx;
    t_uart_Rx_Tx_xfer sTx;
}
t_uart_xfer;

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct
{
    uint8_t 	u8Channel;
    uint8_t 	u8IdMsg;
    t_uart_xfer *pUartXfer;
}
t_uart_msg_queue;

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
    t_uart_msg_queue sMsgQueue;
    void (* p_App_Cb)(t_uart_msg_queue *p_msgBuffer);
}t_drv_serial_arg;

#endif /* _DRV_SERIAL_UART_H_ */
