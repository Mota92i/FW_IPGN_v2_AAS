/******************************************************************************/
/// \file           protocole_modbus.h
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
    #include "protocol_modbus_diagnostic.h"
#undef public

#define public extern
    #include "core_cmd_diagnostic.h"
    #include "crc16.h"
#undef public

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
int32_t ProtocolModbusDiagnostic_Process( uint8_t u8MBAddress,
                                          Uartframe_t *psInframe,
                                          Uartframe_t *psOutframe )
{
    t_frame_fct_PC_reply *pOutFrame = (t_frame_fct_PC_reply *)psOutframe->pu8Buf;
    t_frame_fct_PC_cmd   *pInFrame  = (t_frame_fct_PC_cmd *)psInframe->pu8Buf;
    uint16_t u16Crc; 
    int32_t  Error;
    
    //--------------------------------------------------------------------------
    // Do some check on the frame
    //-------------------------------------------------------------------------- 
    if( (psInframe->u16Len > 255 ) && (psInframe->u16Len < 11) )
        return( -1 );
 
    if( pInFrame->common.tag != 0xFF )
        return( -1 );
 
    if( CRC16_ComputeOnBuffer(psInframe->pu8Buf, psInframe->u16Len) != 0 )
        return( -1 );
    
    Error = -1;
    
    // Build reply
    pOutFrame->common.tag = 0xFF;
    psOutframe->u16Len = 1;
            
    // reply command
    Error = CoreCmdDiagnostic_Process(psInframe, psOutframe);

    if( Error != -1 )
    {
        u16Crc = CRC16_ComputeOnBuffer((uint8_t *)psOutframe->pu8Buf, psOutframe->u16Len);
        psOutframe->pu8Buf[psOutframe->u16Len]  = (uint8_t)u16Crc;
        psOutframe->pu8Buf[psOutframe->u16Len + 1]  = (uint8_t)(u16Crc >> 8);
        psOutframe->u16Len += 2;
    }
    
    return( Error );
}
