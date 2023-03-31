/******************************************************************************/
/// \file           protocol_modbus_rtu_private.h
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
#ifndef __PROTOCOL_MODBUS_RTU_PRIVATE_H__
#define __PROTOCOL_MODBUS_RTU_PRIVATE_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "ca_prototype.h"

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
int32_t DrvProtoModbusRtu_Check( Uartframe_t *psInframe, 
                                 uint8_t *pu8Nab, 
                                 Uartframe_t *psOutframe );
int32_t DrvProtoModbusRtu_Build(uint8_t u8Nab, Uartframe_t *psOutframe);
#endif /* __PROTOCOL_MODBUS_RTU_PRIVATE_H__ */

