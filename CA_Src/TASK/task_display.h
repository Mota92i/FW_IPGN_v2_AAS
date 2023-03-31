/******************************************************************************/
/// \file           task_display.c
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
#ifndef __TASK_DISPLAY_H__
#define __TASK_DISPLAY_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "app_types.h"

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void task_display(void * pvParameters);
extern void voTaskApp_GetMotorPosition(t_motor_position *pMeasure);

#endif /* __TASK_DISPLAY_H__ */

