/******************************************************************************/
/// \file           drv_serial.h
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
#ifndef _DRV_SERIAL_H_
#define _DRV_SERIAL_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "drv_serial_def.h"
#include "drv_serial_uart.h"

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
    DRV_SERIAL_MODBUS_RTU 		= 0,
	DRV_SERIAL_NMEA_LISTEN,

    DRV_SERIAL_COUNT,
};

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
    uint32_t  u32BaudRate;
    uint8_t   u8DataBits;
    uint8_t   u8Parity;
    uint8_t   u8StopBits;
	uint8_t   u8HalfFullDuplex;
	uint8_t   u8BreakProcessEnable;
	uint32_t  u32IdleTimeOut_Ms;
}serial_config_t;

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
    int32_t (* Open)( uint8_t  		   u8Channel,
   	   	   	   	  	  serial_config_t  *psSerialCfg,
					  void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer),
					  t_uart_xfer **p_GetTheBuffer );
    int32_t  (* Close)( uint8_t  u8Channel );
    int32_t  (* Write)( uint8_t  u8Channel, uint8_t  *pDataTx, uint16_t u16LengthTx );
    int32_t  (* WriteInProgress)( uint8_t u8Channel );
    int32_t  (* ReceiveInProgress)( uint8_t u8Channel );
    uint32_t (* GetData)(uint8_t Channel, uint8_t GetInfo);
}serial_interface_t;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern serial_interface_t * psDrvSerial_GetInterface(uint8_t proto);
extern void voDrvSerial_Init(void);

#endif /* _DRV_SERIAL_H_ */
