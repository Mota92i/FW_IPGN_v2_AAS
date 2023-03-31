/******************************************************************************/
/// \file           os_timer.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-09-20
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
#ifndef __OS_TIMER_H__
#define __OS_TIMER_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
	
/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern int intOsTimer_Create(uint32_t u32PeriodMs, void(* p_CallBack)(void));
#endif /* __OS_TIMER_H__ */

