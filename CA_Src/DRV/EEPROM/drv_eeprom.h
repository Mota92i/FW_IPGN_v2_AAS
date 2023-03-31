/******************************************************************************/
/// \file           drv_eeprom.h
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
#ifndef __DRV_EEPROM_H__
#define __DRV_EEPROM_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
	
/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern int32_t DrvEeprom_Write(uint32_t u32Adr, uint8_t *pDest, uint32_t u32NbWrite, uint8_t u8Retry);
extern int32_t DrvEeprom_Read(uint32_t u32Adr, uint8_t *pDest, uint32_t u32NbRead, uint8_t u8Retry);

#endif /* __DRV_EEPROM_H__ */

