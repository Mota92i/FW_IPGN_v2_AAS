/******************************************************************************/
/// \file           drv_serial_uart.c
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
    #include "drv_serial.h"
#undef public

#define public extern
    #include <string.h>
    #include "drv_serial_break_detection_private.h"
    #include "drv_serial_modbus_rtu_private.h"
    #include "drv_serial_nmea_listen_private.h"
#undef public

/******************************************************************************/
//  variables declarations
/******************************************************************************/
static const serial_interface_t SerialInterface[ DRV_SERIAL_COUNT ] = {
  [ DRV_SERIAL_MODBUS_RTU  ] = { DrvModbusRtu_Open,   		DrvModbusRtu_Close,   		DrvModbusRtu_Write,   		DrvModbusRtu_WriteInProgress,   	 DrvModbusRtu_ReceiveInProgress,		DrvModbusRtu_GetData        },
  [ DRV_SERIAL_NMEA_LISTEN ] = { DrvSerialNmeaListen_Open,  DrvSerialNmeaListen_Close,  DrvSerialNmeaListen_Write,  DrvSerialNmeaListen_WriteInProgress, DrvSerialNmeaListen_ReceiveInProgress,	DrvSerialNmeaListen_GetData },
};

/******************************************************************************/
//  Function definitions
/******************************************************************************/


//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
serial_interface_t * psDrvSerial_GetInterface(uint8_t proto)
{
    serial_interface_t *ptr = NULL;
    
    if( proto < DRV_SERIAL_COUNT )
    {
        ptr = (serial_interface_t *)&SerialInterface[proto];
    }

    return( ptr );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
void voDrvSerial_Init(void)
{
	voDrvSerialDetection_Init();
}

