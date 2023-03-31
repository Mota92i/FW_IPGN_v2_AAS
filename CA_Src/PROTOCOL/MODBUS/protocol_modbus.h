/******************************************************************************/
/// \file           protocol_modbus.h
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
#ifndef __PROTOCOL_MODBUS_H__
#define __PROTOCOL_MODBUS_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "ca_prototype.h"

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
enum{
    INDEX_CB_MB_FUNC_NONE = 0,
    INDEX_CB_MB_FUNC_READ_COILS,
    INDEX_CB_MB_FUNC_READ_DISCRETE_INPUTS,
    INDEX_CB_MB_FUNC_WRITE_SINGLE_COIL,
    INDEX_CB_MB_FUNC_WRITE_MULTIPLE_COILS,
    INDEX_CB_MB_FUNC_READ_HOLDING_REGISTER,
    INDEX_CB_MB_FUNC_READ_INPUT_REGISTER,
    INDEX_CB_MB_FUNC_WRITE_REGISTER,
    INDEX_CB_MB_FUNC_WRITE_MULTIPLE_REGISTERS,
    INDEX_CB_MB_FUNC_READWRITE_MULTIPLE_REGISTERS,
    INDEX_CB_MB_FUNC_DIAG_READ_EXCEPTION,
    INDEX_CB_MB_FUNC_DIAG_DIAGNOSTIC,
    INDEX_CB_MB_FUNC_DIAG_GET_COM_EVENT_CNT,
    INDEX_CB_MB_FUNC_DIAG_GET_COM_EVENT_LOG,
    INDEX_CB_MB_FUNC_OTHER_REPORT_SLAVEID,
    INDEX_CB_MB_FUNC_ERROR,
    
    INDEX_CB_MB_FUNC_COUNT,    
};

typedef uint32_t( * t_ModbusFcHandler ) (void *args);
typedef struct{
	uint8_t  u8Nab;
    uint8_t  *pu8RegBuffer;
    uint16_t u16Address;
    uint16_t u16NRegs;
    int32_t  s32AbonneIdx;
}cb_MbFuncReadHoldingReg_t, cb_MbFuncWriteMultipleHoldingReg_t;

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
// Code function
typedef enum
{
    MB_FUNC_NONE                          =  0,
    MB_FUNC_READ_COILS                    =  1,
    MB_FUNC_READ_DISCRETE_INPUTS          =  2,
    MB_FUNC_WRITE_SINGLE_COIL             =  5,
    MB_FUNC_WRITE_MULTIPLE_COILS          = 15,
    MB_FUNC_READ_HOLDING_REGISTER         =  3,
    MB_FUNC_READ_INPUT_REGISTER           =  4,
    MB_FUNC_WRITE_REGISTER                =  6,
    MB_FUNC_WRITE_MULTIPLE_REGISTERS      = 16,
    MB_FUNC_READWRITE_MULTIPLE_REGISTERS  = 23,
    MB_FUNC_DIAG_READ_EXCEPTION           =  7,
    MB_FUNC_DIAG_DIAGNOSTIC               =  8,
    MB_FUNC_DIAG_GET_COM_EVENT_CNT        = 11,
    MB_FUNC_DIAG_GET_COM_EVENT_LOG        = 12,
    MB_FUNC_OTHER_REPORT_SLAVEID          = 17,
    MB_FUNC_ERROR                         = 128,
}eMBFunctionCode;

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
// Exception code
typedef enum
{
    MB_EX_NONE                  = 0x00,
    MB_EX_ILLEGAL_FUNCTION      = 0x01,
    MB_EX_ILLEGAL_DATA_ADDRESS  = 0x02,
    MB_EX_ILLEGAL_DATA_VALUE    = 0x03,
    MB_EX_SLAVE_DEVICE_FAILURE  = 0x04,
    MB_EX_ACKNOWLEDGE           = 0x05,
    MB_EX_SLAVE_BUSY            = 0x06,
    MB_EX_MEMORY_PARITY_ERROR   = 0x08,
    MB_EX_GATEWAY_PATH_FAILED   = 0x0A,
    MB_EX_GATEWAY_TGT_FAILED    = 0x0B
} eMBException;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
/*extern int32_t ProtocoleModbus_Process( uint8_t u8MBAddress,
                                        t_ModbusFcHandler *p_ModbusFcHandler,
                                        Uartframe_t *psInframe, 
                                        Uartframe_t *psOutframe );*/
extern int32_t ProtocolModbus_Process(  int32_t (* pNabCheck)(uint8_t u8Nab),
										t_ModbusFcHandler *p_ModbusFcHandler,
										Uartframe_t *psInframe,
										Uartframe_t *psOutframe,
										int32_t     *ps32AbonneIdx);
#endif /* __PROTOCOL_MODBUS_H__ */

