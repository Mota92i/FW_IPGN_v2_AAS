/******************************************************************************/
/// \file           protocol_modbus_rtu_fc_read_input_register_private.h
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
#ifndef __PROTOCOL_MODBUS_RTU_FC_READ_INPUT_REGISTER_PRIVATE_H__
#define __PROTOCOL_MODBUS_RTU_FC_READ_INPUT_REGISTER_PRIVATE_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include "protocol_modbus.h"
#include <stdint.h>
#include "ca_prototype.h"

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
int32_t DrvProtoModbusRtuFC_ReadInputRegister( t_ModbusFcHandler p_ModbusFcHandler,
											   uint8_t     	  	 u8Nab,
											   Uartframe_t 	  	 *psInframe,
											   Uartframe_t 	  	 *psOutframe,
											   int32_t     	  	 s32AbonneIdx );

#endif /* __PROTOCOL_MODBUS_RTU_FC_READ_INPUT_REGISTER_PRIVATE_H__ */

