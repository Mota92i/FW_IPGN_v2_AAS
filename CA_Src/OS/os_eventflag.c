/******************************************************************************/
/// \file           os_eventflag.c
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
//  Included header file list
/******************************************************************************/
#define public
    #include "os_eventflag.h"
#undef public

#define public extern
	#include "FreeRTOS.h"
	#include "event_groups.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
	EventGroupHandle_t xHandle;	// Declare a variable to hold the handle of the created event group.
	StaticEventGroup_t xBuffer; // Declare a variable to hold the data associated with the created event group.
}t_event_flag;

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static t_event_flag g_sEventFlagUSecDlyTmr;

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
void voOsEventFlag_Init(void)
{
	// uSecDlyTmr event flag
	g_sEventFlagUSecDlyTmr.xHandle = xEventGroupCreateStatic(&g_sEventFlagUSecDlyTmr.xBuffer);

    if( g_sEventFlagUSecDlyTmr.xHandle == NULL )
        while(1);
}

void voOsEventFlag_uSecDlyTmrWait(uint32_t u32Flag)
{
	EventBits_t uxBits;

	if( u32Flag == 0 )
		return;

	do{
		// Clear the bits before exiting.
		uxBits = xEventGroupWaitBits(
				g_sEventFlagUSecDlyTmr.xHandle, // The event group being tested.
				u32Flag, 						// The bits within the event group to wait for.
				pdTRUE,        					// bits should be cleared before returning.
				pdFALSE,       					// Wait at least for one bit
				portMAX_DELAY );
	}while( (uxBits & u32Flag) == 0 );
}

void voOsEventFlag_uSecDlyTmrPost(uint32_t u32Flag)
{
	BaseType_t xHigherPriorityTaskWoken;
	BaseType_t xResult;

	if( u32Flag == 0 )
		return;

	xHigherPriorityTaskWoken = pdFALSE;	// must be initialised to pdFALSE.

	xResult = xEventGroupSetBitsFromISR(
					g_sEventFlagUSecDlyTmr.xHandle,	// The event group being updated.
					u32Flag, 						// The bits being set.
					&xHigherPriorityTaskWoken );

	// Was the message posted successfully?
	if( xResult != pdFAIL )
	{
		// If xHigherPriorityTaskWoken is now set to pdTRUE then a context
		// switch should be requested.  The macro used is port specific and will
		// be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
		// the documentation page for the port being used.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}
