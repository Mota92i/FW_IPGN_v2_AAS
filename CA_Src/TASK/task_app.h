/******************************************************************************/
/// \file           task_app.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2022-01-17
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
#ifndef __TASK_APP_H__
#define __TASK_APP_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "app_types.h"

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void 	task_app(void * pvParameters);
extern void 	voTaskApp_GetSMeasure(t_app_measure *pMeasure);
extern uint8_t 	u8TaskApp_GetModeFct(void);
#endif /* __TASK_APP_H__ */

