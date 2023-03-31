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
//  Included header file list
/******************************************************************************/
#define public
    #include "protocol_modbus.h"
    #include "protocol_modbus_private.h"
#undef public

#define public extern
    #include <string.h>
    #include "protocol_modbus_rtu_private.h"
    #include "protocol_modbus_rtu_fc_holding_private.h"
    #include "protocol_modbus_rtu_fc_read_input_register_private.h"
#undef public

/******************************************************************************/
//  
/******************************************************************************/
// Constants which defines the format of a modbus frame. The example is
// shown for a Modbus RTU/ASCII frame. Note that the Modbus PDU is not
// dependent on the underlying transport.
//
// <------------------------ MODBUS SERIAL LINE PDU (1) ------------------->
//              <----------- MODBUS PDU (1') ---------------->
//  +-----------+---------------+----------------------------+-------------+
//  | Address   | Function Code | Data                       | CRC/LRC     |
//  +-----------+---------------+----------------------------+-------------+
//  |           |               |                                   |
// (2)        (3/2')           (3')                                (4)
//
// (1)  ... MB_SER_PDU_SIZE_MAX = 256
// (2)  ... MB_SER_PDU_ADDR_OFF = 0
// (3)  ... MB_SER_PDU_PDU_OFF  = 1
// (4)  ... MB_SER_PDU_SIZE_CRC = 2
//
// (1') ... MB_PDU_SIZE_MAX     = 253 (without nab and CRC 2 + 1 = 3)
// (2') ... MB_PDU_FUNC_OFF     = 0
// (3') ... MB_PDU_DATA_OFF     = 1

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef int32_t( *pMBFunctionHandler_t ) ( t_ModbusFcHandler p_ModbusFcHandler,
										   uint8_t     	 	 u8Nab,
										   Uartframe_t 	  	 *psInframe,
										   Uartframe_t 	  	 *psOutframe,
										   int32_t     	  	 s32AbonneIdx );

typedef struct
{
    uint8_t              u8FunctionCode;
    pMBFunctionHandler_t pHandler;
} MBFunctionHandler_t;

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
static const MBFunctionHandler_t g_FuncHandlers[] = {
[ INDEX_CB_MB_FUNC_NONE                         ] = { MB_FUNC_NONE,                         NULL                                              },
[ INDEX_CB_MB_FUNC_READ_COILS                   ] = { MB_FUNC_READ_COILS,                   NULL                                              },
[ INDEX_CB_MB_FUNC_READ_DISCRETE_INPUTS         ] = { MB_FUNC_READ_DISCRETE_INPUTS,         NULL                                              },
[ INDEX_CB_MB_FUNC_WRITE_SINGLE_COIL            ] = { MB_FUNC_WRITE_SINGLE_COIL,            NULL                                              },
[ INDEX_CB_MB_FUNC_WRITE_MULTIPLE_COILS         ] = { MB_FUNC_WRITE_MULTIPLE_COILS,         NULL                                              },
[ INDEX_CB_MB_FUNC_READ_HOLDING_REGISTER        ] = { MB_FUNC_READ_HOLDING_REGISTER,        DrvProtoModbusRtuFC_ReadHoldingReg                },
[ INDEX_CB_MB_FUNC_READ_INPUT_REGISTER          ] = { MB_FUNC_READ_INPUT_REGISTER,          DrvProtoModbusRtuFC_ReadInputRegister             },
[ INDEX_CB_MB_FUNC_WRITE_REGISTER               ] = { MB_FUNC_WRITE_REGISTER,               NULL                                              },
[ INDEX_CB_MB_FUNC_WRITE_MULTIPLE_REGISTERS     ] = { MB_FUNC_WRITE_MULTIPLE_REGISTERS,     DrvProtoModbusRtuFC_WriteMultipleHoldingRegister  },
[ INDEX_CB_MB_FUNC_READWRITE_MULTIPLE_REGISTERS ] = { MB_FUNC_READWRITE_MULTIPLE_REGISTERS, NULL                                              },
[ INDEX_CB_MB_FUNC_DIAG_READ_EXCEPTION          ] = { MB_FUNC_DIAG_READ_EXCEPTION,          NULL                                              },
[ INDEX_CB_MB_FUNC_DIAG_DIAGNOSTIC              ] = { MB_FUNC_DIAG_DIAGNOSTIC,              NULL                                              },
[ INDEX_CB_MB_FUNC_DIAG_GET_COM_EVENT_CNT       ] = { MB_FUNC_DIAG_GET_COM_EVENT_CNT,       NULL                                              },
[ INDEX_CB_MB_FUNC_DIAG_GET_COM_EVENT_LOG       ] = { MB_FUNC_DIAG_GET_COM_EVENT_LOG,       NULL                                              },
[ INDEX_CB_MB_FUNC_OTHER_REPORT_SLAVEID         ] = { MB_FUNC_OTHER_REPORT_SLAVEID,         NULL                                              },
[ INDEX_CB_MB_FUNC_ERROR                        ] = { MB_FUNC_ERROR,                        NULL                                              }
};


/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      uint8_t u8MBAddress
///                 Uartframe_t *psInframe
/// \param[out]     Uartframe_t *psOutframe
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
#if 0
int32_t ProtocolModbus_Process( uint8_t u8MBAddress,
                                 t_ModbusFcHandler *p_ModbusFcHandler,
                                 Uartframe_t *psInframe, 
                                 Uartframe_t *psOutframe )
{
    uint8_t  u8Nab;
    uint8_t  u8FunctionCode;
    uint8_t  u8ExceptionCode;
    int32_t  Error;
    uint8_t  i;
    Uartframe_t s_in_frame;  // extract from " psInframe "
    Uartframe_t s_out_frame; // extract from " psOutframe "
    
    s_out_frame.pu8Buf = &psOutframe->pu8Buf[MB_SER_PDU_PDU_OFF];
      
    Error = DrvProtoModbusRtu_Check(psInframe, &u8Nab, &s_in_frame);
    if( Error == 0 )
    {
        Error = -1;

        // Check if the frame is for us otherwise ignore it.
        if( (u8Nab == u8MBAddress) || (u8Nab == MB_ADDRESS_BROADCAST) )
        {
            u8FunctionCode = s_in_frame.pu8Buf[MB_PDU_FUNC_OFF];
            u8ExceptionCode = MB_EX_ILLEGAL_FUNCTION;
            // Execute function code
            for( i = 0; i < INDEX_CB_MB_FUNC_COUNT; i++ )
            {
                if( g_FuncHandlers[i].u8FunctionCode == u8FunctionCode )
                {
                    if( g_FuncHandlers[i].pHandler != NULL )
                    {
                        if( p_ModbusFcHandler[i] != NULL )
                        {
                            u8ExceptionCode = g_FuncHandlers[i].pHandler( p_ModbusFcHandler[i],&s_in_frame, &s_out_frame );
                        }
                    }
                    break;
                }
            }
            
            // If the request was not sent to the broadcast address we return a reply.
            if( u8Nab != MB_ADDRESS_BROADCAST )
            {
                if( u8ExceptionCode != MB_EX_NONE )
                {
                    // An exception occured. Build an error frame.
                    s_out_frame.u16Len = 0;
                    s_out_frame.pu8Buf[s_out_frame.u16Len++] = (uint8_t)(u8FunctionCode | MB_FUNC_ERROR);
                    s_out_frame.pu8Buf[s_out_frame.u16Len++] = u8ExceptionCode;
                }
                //if( ( eMBCurrentMode == MB_ASCII ) && MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS )
                //{
                //    Delay( MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS );
                //}
                Error = DrvProtoModbusRtu_Build(u8Nab, &s_out_frame);
                psOutframe->u16Len = s_out_frame.u16Len;
            }
        }
    }
    
    return( Error );
}
#endif

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      uint8_t u8MBAddress
///                 Uartframe_t *psInframe
/// \param[out]     Uartframe_t *psOutframe
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
int32_t ProtocolModbus_Process(  int32_t (* pNabCheck)(uint8_t u8Nab),
                                 t_ModbusFcHandler *p_ModbusFcHandler,
                                 Uartframe_t *psInframe,
                                 Uartframe_t *psOutframe,
								 int32_t     *ps32AbonneIdx)
{
    uint8_t  u8Nab;
    int32_t  s32AbonneIdx = -1;
    uint8_t  u8FunctionCode;
    uint8_t  u8ExceptionCode;
    int32_t  Error;
    uint8_t  i;
    Uartframe_t s_in_frame;  // extract from " psInframe "
    Uartframe_t s_out_frame; // extract from " psOutframe "

    s_out_frame.pu8Buf = &psOutframe->pu8Buf[MB_SER_PDU_PDU_OFF];

    Error = DrvProtoModbusRtu_Check(psInframe, &u8Nab, &s_in_frame);
    if( Error == 0 )
    {
        Error = -1;

        // Check if the frame is for us otherwise ignore it.
        s32AbonneIdx = pNabCheck(u8Nab);
        if( (s32AbonneIdx != -1) || (u8Nab == MB_ADDRESS_BROADCAST) )
        {
            u8FunctionCode = s_in_frame.pu8Buf[MB_PDU_FUNC_OFF];
            u8ExceptionCode = MB_EX_ILLEGAL_FUNCTION;
            // Execute function code
            for( i = 0; i < INDEX_CB_MB_FUNC_COUNT; i++ )
            {
                if( g_FuncHandlers[i].u8FunctionCode == u8FunctionCode )
                {
                    if( g_FuncHandlers[i].pHandler != NULL )
                    {
                        if( p_ModbusFcHandler[i] != NULL )
                        {
                            u8ExceptionCode = g_FuncHandlers[i].pHandler( p_ModbusFcHandler[i], u8Nab, &s_in_frame, &s_out_frame, s32AbonneIdx );
                        }
                    }
                    break;
                }
            }

            // If the request was not sent to the broadcast address we return a reply.
            if( u8Nab != MB_ADDRESS_BROADCAST )
            {
                if( u8ExceptionCode != MB_EX_NONE )
                {
                    // An exception occured. Build an error frame.
                    s_out_frame.u16Len = 0;
                    s_out_frame.pu8Buf[s_out_frame.u16Len++] = (uint8_t)(u8FunctionCode | MB_FUNC_ERROR);
                    s_out_frame.pu8Buf[s_out_frame.u16Len++] = u8ExceptionCode;
                }
                //if( ( eMBCurrentMode == MB_ASCII ) && MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS )
                //{
                //    Delay( MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS );
                //}
                Error = DrvProtoModbusRtu_Build(u8Nab, &s_out_frame);
                psOutframe->u16Len = s_out_frame.u16Len;
            }
        }
    }

    *ps32AbonneIdx = s32AbonneIdx;

    return( Error );
}
