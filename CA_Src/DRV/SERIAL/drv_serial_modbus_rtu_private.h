/******************************************************************************/
/// \file           drv_serial_modbus_rtu_private.h
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
#ifndef _DRV_SERIAL_MODBUS_RTU_PRIVATE_H_
#define _DRV_SERIAL_MODBUS_RTU_PRIVATE_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "drv_serial_uart.h"
#include "drv_serial.h"

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern int32_t DrvModbusRtu_Open( uint8_t  		   u8Channel,
		   	   	   	   	   	   	  serial_config_t  *psSerialCfg,
								  void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer),
								  t_uart_xfer **p_GetTheBuffer );
extern int32_t DrvModbusRtu_Close( uint8_t  u8Channel );
extern int32_t DrvModbusRtu_Write( uint8_t  u8Channel, 
                                   uint8_t  *pDataTx, 
                                   uint16_t u16LengthTx );
extern int32_t DrvModbusRtu_WriteInProgress(uint8_t u8Channel);
extern int32_t DrvModbusRtu_ReceiveInProgress(uint8_t u8Channel);
extern uint32_t DrvModbusRtu_GetData(uint8_t Channel, uint8_t GetInfo);
#endif /* _DRV_SERIAL_MODBUS_RTU_PRIVATE_H_ */
