/******************************************************************************/
/// \file           os_eventflag.h
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
#ifndef __OS_EVENT_FLAG_H__
#define __OS_EVENT_FLAG_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
	
/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voOsEventFlag_Init(void);
extern void voOsEventFlag_uSecDlyTmrWait(uint32_t u32Flag);
extern void voOsEventFlag_uSecDlyTmrPost(uint32_t u32Flag);

#endif /* __OS_EVENT_FLAG_H__ */

