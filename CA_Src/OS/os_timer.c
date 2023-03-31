/******************************************************************************/
/// \file           os_delay.c
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
//  Included header file list
/******************************************************************************/
#define public
    #include "os_timer.h"
#undef public

#define public extern
	#include "FreeRTOS.h"
	#include "timers.h"
#undef public

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define MAX_TIMERS			( 2 )

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static uint8_t       g_TimerIdx = 0;
static TimerHandle_t g_xTimers[ MAX_TIMERS ];
static StaticTimer_t g_xTimerBuffers[ MAX_TIMERS ];

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void vTimerCallback( TimerHandle_t xTimer );

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
/// \date           2020-09-20
///
/*----------------------------------------------------------------------------*/
// Define a callback function that will be used by multiple timer instances.
// The callback function does nothing but count the number of times the
// associated timer expires, and stop the timer once the timer has expired 10 times.
// The count is saved as the ID of the timer.
static void vTimerCallback( TimerHandle_t xTimer )
{
	void(* p_CallBack)(void);

	p_CallBack = pvTimerGetTimerID( xTimer );
	p_CallBack();
	/*
	const uint32_t ulMaxExpiryCountBeforeStopping = 10;
	uint32_t ulCount;

	// Optionally do something if the pxTimer parameter is NULL.
	configASSERT( pxTimer );

	// The number of times this timer has expired is saved as the
	// timer's ID.  Obtain the count.
	ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );

	// Increment the count, then test to see if the timer has expired
	// ulMaxExpiryCountBeforeStopping yet.
	ulCount++;

	// If the timer has expired 10 times then stop it from running.
	if( ulCount >= ulMaxExpiryCountBeforeStopping )
	{
		// Do not use a block time if calling a timer API function from
		// a timer callback function, as doing so could cause a deadlock!
		xTimerStop( xTimer, 0 );
	}
	else
	{
		// Store the incremented count back into the timer's ID field so
		// it can be read back again the next time this software timer expires.
		vTimerSetTimerID( xTimer, ( void * ) ulCount );
	}
	*/
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2020-09-20
///                 
/*----------------------------------------------------------------------------*/
int intOsTimer_Create(uint32_t u32PeriodMs, void(* p_CallBack)(void))
{
	int Error = 0;

	if( g_TimerIdx >= MAX_TIMERS )
		return( -1 );

	g_xTimers[ g_TimerIdx ] = xTimerCreateStatic(
				"Timer",							// Just a text name, not used by the RTOS kernel.
				pdMS_TO_TICKS( u32PeriodMs ),		// The timer period in ticks, must be greater than 0.
				pdTRUE,								// The timers will auto-reload themselves when they expire.
				( void * )p_CallBack,				// The ID is used to store a count of the number of times the
													// timer has expired, which is initialised to 0.
				vTimerCallback,						// Each timer calls the same callback when it expires.
				&g_xTimerBuffers[ g_TimerIdx ]		// Pass in the address of a StaticTimer_t variable, which will
													// hold the data associated with the timer being created.
	);

	if( g_xTimers[ g_TimerIdx ] == NULL )
	{
		Error = -1;	// The timer was not created.
	}
	else
	{
		// Start the timer.  No block time is specified, and even if one was it would be ignored because the RTOS
		// scheduler has not yet been started.
		if( xTimerStart( g_xTimers[ g_TimerIdx ], 0 ) != pdPASS )
		{
			Error = -1;	// The timer could not be set into the Active state.
		}
	}

	if( Error == 0 )
		g_TimerIdx++;

	return( Error );
}
