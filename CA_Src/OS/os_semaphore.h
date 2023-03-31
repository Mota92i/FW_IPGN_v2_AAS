/******************************************************************************/
/// \file           os_semaphore.h
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
#ifndef __OS_SEMAPHORE_H__
#define __OS_SEMAPHORE_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
	
/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voOsSemaphore_Init(void);

extern void voOsSemaphore_MsDlyTmrTake(void);
extern void voOsSemaphore_MsDlyTmrGive(void);

extern void voOsSemaphore_EepromTake(void);
extern void voOsSemaphore_EepromGive(void);
#endif /* __OS_SEMAPHORE_H__ */

