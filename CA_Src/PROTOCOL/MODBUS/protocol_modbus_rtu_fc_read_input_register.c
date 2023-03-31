/******************************************************************************/
/// \file           protocol_modbus_rtu_fc_read_input_register.c
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
    #include "protocol_modbus_rtu_fc_read_input_register_private.h"
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
int32_t DrvProtoModbusRtuFC_ReadInputRegister( t_ModbusFcHandler p_ModbusFcHandler,
											   uint8_t     	  	 u8Nab,
											   Uartframe_t 	  	 *psInframe,
											   Uartframe_t 	  	 *psOutframe,
											   int32_t     	  	 s32AbonneIdx )
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
            *sArg.pu8RegBuffer++ = MB_FUNC_READ_INPUT_REGISTER;
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
