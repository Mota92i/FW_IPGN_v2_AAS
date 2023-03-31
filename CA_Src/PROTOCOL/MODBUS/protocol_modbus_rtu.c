/******************************************************************************/
/// \file           protocol_modbus_rtu.c
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
    #include "protocol_modbus_rtu_private.h"
#undef public

#define public extern
    #include "protocol_modbus_private.h"
    #include "crc16.h"
#undef public

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      Uartframe_t *psInframe
///                 uint8_t *pu8Nab
/// \param[out]     Uartframe_t *psOutframe
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
int32_t DrvProtoModbusRtu_Check( Uartframe_t *psInframe, 
                                 uint8_t *pu8Nab, 
                                 Uartframe_t *psOutframe )
{
    int32_t Error = -1;

//ENTER_CRITICAL_SECTION(  );

    // Length
    if( (psInframe->u16Len <= MB_SER_PDU_SIZE_MAX ) && 
        (psInframe->u16Len >= MB_SER_PDU_SIZE_MIN) )
    {
        // CRC check
        if( CRC16_ComputeOnBuffer(psInframe->pu8Buf, psInframe->u16Len) == 0  )
        {
            *pu8Nab = psInframe->pu8Buf[MB_SER_PDU_ADDR_OFF];            // address field.
            psOutframe->pu8Buf = &psInframe->pu8Buf[MB_SER_PDU_PDU_OFF]; // start of the Modbus PDU.
            
            // Length of Modbus-PDU minus size of address field and CRC checksum.
            psOutframe->u16Len = (uint16_t)(psInframe->u16Len - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC);
            
            Error = 0;
        }
    }

//EXIT_CRITICAL_SECTION(  );
    return( Error );
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      uint8_t u8Nab
/// \param[out]     Uartframe_t *psOutframe
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
int32_t DrvProtoModbusRtu_Build(uint8_t u8Nab, Uartframe_t *psOutframe)
{
    uint8_t *ptr;
    uint16_t crc16;
    uint16_t u16TxLen;
    
//ENTER_CRITICAL_SECTION(  );

    // First byte before the Modbus-PDU is the slave address.
    ptr = (uint8_t *)psOutframe->pu8Buf - 1;
    u16TxLen = 1;

    // Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU.
    ptr[MB_SER_PDU_ADDR_OFF] = u8Nab;
    u16TxLen += psOutframe->u16Len;

    // Calculate CRC16 checksum for Modbus-Serial-Line-PDU.
    crc16 = CRC16_ComputeOnBuffer(ptr, u16TxLen);
    ptr[u16TxLen++] = ( uint8_t )( crc16 & 0xFF );
    ptr[u16TxLen++] = ( uint8_t )( crc16 >> 8 );

    psOutframe->u16Len = u16TxLen;

//EXIT_CRITICAL_SECTION(  );
    return( 0 );
}
