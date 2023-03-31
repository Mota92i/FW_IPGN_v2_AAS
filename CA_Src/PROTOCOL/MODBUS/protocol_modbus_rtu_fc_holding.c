/******************************************************************************/
/// \file           protocol_modbus_rtu_fc_holding_private.c
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
    #include "protocol_modbus_rtu_fc_holding_private.h"
#undef public

#define public extern
    #include "protocol_modbus.h"
    #include "protocol_modbus_private.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define MB_PDU_FUNC_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READ_REGCNT_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE                   ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )

#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE                  ( 4 )

#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x007B )

#define MB_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )


/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      Uartframe_t *psInframe
/// \param[out]     Uartframe_t *psOutframe
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
int32_t DrvProtoModbusRtuFC_ReadHoldingReg( t_ModbusFcHandler p_ModbusFcHandler,
											uint8_t     	  u8Nab,
                                            Uartframe_t 	  *psInframe,
                                            Uartframe_t 	  *psOutframe,
											int32_t     	  s32AbonneIdx )
{
    cb_MbFuncReadHoldingReg_t   sArg;
    int32_t                     ret_code = MB_EX_ILLEGAL_DATA_VALUE;
    
    if(psInframe->u16Len == (MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN))
    {
    	sArg.u8Nab		  = u8Nab;
    	sArg.s32AbonneIdx = s32AbonneIdx;

        sArg.u16Address  = (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_READ_ADDR_OFF] << 8);
        sArg.u16Address |= (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_READ_ADDR_OFF + 1]);

        sArg.u16NRegs = (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_READ_REGCNT_OFF] << 8);
        sArg.u16NRegs |= (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_READ_REGCNT_OFF + 1]);

        // Check if the number of registers to read is valid. If not
        // return Modbus illegal data value exception. 
        if( (sArg.u16NRegs >= 1) && (sArg.u16NRegs <= MB_PDU_FUNC_READ_REGCNT_MAX) )
        {
            // Set the current PDU data pointer to the beginning.
            sArg.pu8RegBuffer = &psOutframe->pu8Buf[MB_PDU_FUNC_OFF];
            psOutframe->u16Len = MB_PDU_FUNC_OFF;

            // First byte contains the function code.
            *sArg.pu8RegBuffer++ = MB_FUNC_READ_HOLDING_REGISTER;
            psOutframe->u16Len += 1;

            // Second byte in the response contain the number of bytes.
            *sArg.pu8RegBuffer++ = (uint8_t)(sArg.u16NRegs * 2);
            psOutframe->u16Len += 1;

            // Make callback to fill the buffer.
            ret_code = p_ModbusFcHandler(&sArg);
            if( ret_code == MB_EX_NONE )
            {
                psOutframe->u16Len += sArg.u16NRegs * 2;
            }
        }
    }   
    
    return( ret_code );
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      Uartframe_t *psInframe
/// \param[out]     Uartframe_t *psOutframe
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
int32_t DrvProtoModbusRtuFC_WriteMultipleHoldingRegister( t_ModbusFcHandler p_ModbusFcHandler,
														  uint8_t     	    u8Nab,
														  Uartframe_t 	    *psInframe,
														  Uartframe_t 	    *psOutframe,
														  int32_t     	    s32AbonneIdx )
{
    cb_MbFuncWriteMultipleHoldingReg_t  sArg;
    int32_t                             ret_code = MB_EX_ILLEGAL_DATA_VALUE;
    uint8_t                             u8RegByteCount;
    uint8_t                             *ptr;
    
    if(psInframe->u16Len >= (MB_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB_PDU_SIZE_MIN))
    {
    	sArg.u8Nab		  = u8Nab;
    	sArg.s32AbonneIdx = s32AbonneIdx;

        sArg.u16Address  = (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8);
        sArg.u16Address |= (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1]);

        sArg.u16NRegs = (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8);
        sArg.u16NRegs |= (uint16_t)(psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1]);

        sArg.pu8RegBuffer = &psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_VALUES_OFF];
        
        u8RegByteCount = psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];
        
        if( (sArg.u16NRegs >= 1) &&
            (sArg.u16NRegs <= MB_PDU_FUNC_WRITE_MUL_REGCNT_MAX) &&
            (u8RegByteCount == (uint8_t)(2 * sArg.u16NRegs)) )
        {
            // Set the current PDU data pointer to the beginning.
            ptr = &psOutframe->pu8Buf[MB_PDU_FUNC_OFF];
            psOutframe->u16Len = MB_PDU_FUNC_OFF;

            // First byte contains the function code.
            *ptr++ = MB_FUNC_WRITE_MULTIPLE_REGISTERS;
            psOutframe->u16Len += 1;

            // Address 2 bytes 
            *ptr++ = psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF];
            *ptr++ = psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1];
            psOutframe->u16Len += 2;

            // Second byte in the response contain the number of bytes.
            *ptr++ = psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF];
            *ptr++ = psInframe->pu8Buf[MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1];
            psOutframe->u16Len += 2;
             
            // Make callback to fill the buffer.      
            ret_code = p_ModbusFcHandler(&sArg);
            if( ret_code == MB_EX_NONE )
            {
                // The response contains the function code, the starting
                // address and the quantity of registers. We reuse the
                // old values in the buffer because they are still valid.
                psOutframe->u16Len = MB_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
            }
        }
    }   
    
    return( ret_code );
}
