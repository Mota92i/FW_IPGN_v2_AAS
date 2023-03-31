/******************************************************************************/
/// \file           os_task.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2022-01-24
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
#ifndef __OS_TASK_H__
#define __OS_TASK_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
	TaskFunction_t 		 pxTaskCode;		// Function that implements the task.
	const char * const 	 pcName;			// Text name for the task.
	const uint32_t 		 ulStackDepth;		// Number of indexes in the xStack array.
	void * const 		 pvParameters;		// Parameter passed into the task.
	UBaseType_t 		 uxPriority;		// Priority at which the task is created.
	StackType_t * const  puxStackBuffer;	// Array to use as the task's stack.
	StaticTask_t * const pxTaskBuffer ;		// Variable to hold the task's data structure.
}t_task_cfg;

typedef struct {
	uint8_t		  bCreate;
	t_task_cfg	  const * const psTaskCfg;
	TaskHandle_t  sTaskHandle;
}t_task_data;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voOsTask_Create(t_task_data *psTaskData);
#endif /* __OS_TASK_H__ */

