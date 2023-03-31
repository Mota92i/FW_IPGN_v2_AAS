/******************************************************************************/
/// \file           protocol_modbus_private.h
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
#ifndef __PROTOCOL_MODBUS_PRIVATE_H__
#define __PROTOCOL_MODBUS_PRIVATE_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define MB_PDU_SIZE_MAX     253 // Maximum size of a PDU.
#define MB_PDU_SIZE_MIN     1   // Function Code.
#define MB_PDU_FUNC_OFF     0   // Offset of function code in PDU.
#define MB_PDU_DATA_OFF     1   // Offset for response data in PDU.

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define MB_SER_PDU_SIZE_MIN     4       // Minimum size of a Modbus RTU frame.
#define MB_SER_PDU_SIZE_MAX     256     // Maximum size of a Modbus RTU frame.
#define MB_SER_PDU_SIZE_CRC     2       // Size of CRC field in PDU.
#define MB_SER_PDU_ADDR_OFF     0       // Offset of slave address in Ser-PDU.
#define MB_SER_PDU_PDU_OFF      1       // Offset of Modbus-PDU in Ser-PDU.

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define MB_ADDRESS_BROADCAST    ( 0 )   // Modbus broadcast address.
#define MB_ADDRESS_MIN          ( 1 )   // Smallest possible slave address.
#define MB_ADDRESS_MAX          ( 247 ) // Biggest possible slave address.

#endif /* __PROTOCOL_MODBUS_PRIVATE_H__ */

