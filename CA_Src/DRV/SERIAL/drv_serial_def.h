/******************************************************************************/
/// \file           drv_serial_def.h
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
#ifndef _DRV_SERIAL_DEF_H_
#define _DRV_SERIAL_DEF_H_

/******************************************************************************/
//  Serial port baudrate
/******************************************************************************/
enum{
    SERIAL_CHANNEL_1 = 0,

    SERIAL_CHANNEL_COUNT
};

enum{
    SERIAL_STOP_BITS_1 = 1,
    SERIAL_STOP_BITS_2 = 2
};

enum{
    SERIAL_PORT_PARITY_NONE  = 0,
    SERIAL_PORT_PARITY_ODD   = 1,
    SERIAL_PORT_PARITY_EVEN  = 2,
    
    SERIAL_PORT_PARITY_COUNT    
};

enum{
    SERIAL_PORT_HALF_DUPLEX  = 0,
    SERIAL_PORT_FULL_DUPLEX  = 1,
    
    SERIAL_PORT_FULL_COUNT    
};

enum{
    GET_SERIAL_TIMEOUT_FOR_ONE_BYTE = 0,
};

#endif /* _DRV_SERIAL_DEF_H_ */
