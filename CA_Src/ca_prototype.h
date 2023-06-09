/******************************************************************************/
/// \file           ca_prototype.h
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
#ifndef _CA_PROTOTYPE_H_
#define _CA_PROTOTYPE_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
    uint8_t *pu8Buf;
    uint16_t u16Len;
}Uartframe_t;

#endif /* _CA_PROTOTYPE_H_ */
