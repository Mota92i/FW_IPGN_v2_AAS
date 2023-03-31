/******************************************************************************/
/// \file           os_task.c
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
//  Included header file list
/******************************************************************************/
#define public
    #include "os_task.h"
#undef public

#define public extern
	#include <string.h>
	#include <stdbool.h>
#undef public

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
void voOsTask_Create(t_task_data *psTaskData)
{
	if( psTaskData->bCreate == true )
	{
		// Create the task without using any dynamic memory allocation.
		psTaskData->sTaskHandle = xTaskCreateStatic(
				psTaskData->psTaskCfg->pxTaskCode,      // Function that implements the task.
				psTaskData->psTaskCfg->pcName,          // Text name for the task.
				psTaskData->psTaskCfg->ulStackDepth,    // Number of indexes in the xStack array.
				psTaskData->psTaskCfg->pvParameters,    // Parameter passed into the task.
				psTaskData->psTaskCfg->uxPriority,	  	// Priority at which the task is created.
				psTaskData->psTaskCfg->puxStackBuffer,  // Array to use as the task's stack.
				psTaskData->psTaskCfg->pxTaskBuffer );  // Variable to hold the task's data structure.
		while( psTaskData->sTaskHandle == NULL );
	}
}
