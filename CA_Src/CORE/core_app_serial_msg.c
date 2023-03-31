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
//  Included header file list
//*****************************************************************************/
#define public
	#include "core_app_serial_msg.h"
#undef public

#define public extern
	#include <math.h>
	#include <string.h>
	#include <stdbool.h>
	#include "crc16.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/


/******************************************************************************/
//  Variable declaration
/******************************************************************************/


/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-05-06
///
//----------------------------------------------------------------------------*/
int intCoreAppSerialMsg_ModbusDecoder(Uartframe_t *psTx, Uartframe_t *psRx, t_modbus_data_decoder *psDataOut)
{
	int 	 Error = -1;
	uint16_t u16Size;
	uint8_t  u8Idx;
	uint8_t  i;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	// CRC check
	if( CRC16_ComputeOnBuffer(psRx->pu8Buf, psRx->u16Len) == 0  )
	{
		if( (psRx->pu8Buf[ 0 ] == psTx->pu8Buf[ 0 ]) &&	// NAB
			(psRx->pu8Buf[ 1 ] == psTx->pu8Buf[ 1 ]) )	// Code function
		{
			// Check size
			u16Size = (uint16_t)(psTx->pu8Buf[ 4 ] << 8) + (uint16_t)psTx->pu8Buf[ 5 ];

			if( u16Size == (psRx->pu8Buf[ 2 ] / 2) )
			{
				if( u16Size > psDataOut->u8Size )
					u16Size = psDataOut->u8Size;
				psDataOut->u8Size = (uint8_t)u16Size; // return number of element

				for(i = 0; i < u16Size; i++)
				{
					u8Idx = 3 + (i * 2);
					psDataOut->pu16Data[ i ] = (uint16_t)(psRx->pu8Buf[ u8Idx ] << 8) + (uint16_t)psRx->pu8Buf[ u8Idx + 1 ];
				}

				Error = 0;
			}
		}
	}

    return( Error );
}
