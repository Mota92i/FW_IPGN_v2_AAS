//*****************************************************************************/
/// \file           core_app_serial_msg.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2022-05-06
/// \copyright      Chauvin Arnoux Energy
///
/// \ingroup
/// \brief
/// \details
///
//*****************************************************************************/
//  Software configuration management
//*****************************************************************************/

//*****************************************************************************/
//  Inclusion checking
//*****************************************************************************/
#ifndef _CORE_APP_SERIAL_MSG_H_
#define _CORE_APP_SERIAL_MSG_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include "app_types.h"
#include "ca_prototype.h"
#include <stdint.h>

/******************************************************************************/
//  Type declaration
/******************************************************************************/
typedef struct{
	uint16_t *pu16Data;
	uint8_t  u8Size;
}t_modbus_data_decoder;

//*****************************************************************************/
//  Multi-file-scope function prototype declarations
//*****************************************************************************/
extern int intCoreAppSerialMsg_ModbusDecoder(Uartframe_t *psTx, Uartframe_t *psRx, t_modbus_data_decoder *psDataOut);
#endif /* _CORE_APP_SERIAL_MSG_H_ */
