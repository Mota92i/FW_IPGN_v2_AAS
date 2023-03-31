/******************************************************************************/
/// \file           os_delay.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-10-14
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
    #include "os_short_timer.h"
#undef public

#define public extern
	#include "FreeRTOS.h"
	#include "timers.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
#define MAX_TIMER       ( 4 )

/******************************************************************************/
//  Variable declarations
/******************************************************************************/
static struct{
    void(* pCbFunc)(void);
    int32_t s32TickValue;
}g_timer[ MAX_TIMER ];

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
/// \date           2021-10-14
///
/*----------------------------------------------------------------------------*/
// Define a callback function that will be used by multiple timer instances.
// The callback function does nothing but count the number of times the
// associated timer expires, and stop the timer once the timer has expired 10 times.
// The count is saved as the ID of the timer.
static void vTimerCallback( TimerHandle_t xTimer )
{
    int32_t i;

    for(i = 0; i < MAX_TIMER; i++)
    {
    	if( g_timer[ i ].s32TickValue > 0 )
    	{
			if( --g_timer[ i ].s32TickValue == 0 )
			{
				g_timer[ i ].pCbFunc();

				g_timer[ i ].s32TickValue = -1;
				g_timer[ i ].pCbFunc 	  = NULL;
			}
    	}
    }
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-14
///                 
/*----------------------------------------------------------------------------*/
int intOsShortTimer_Init(void)
{
	static TimerHandle_t s_xTimers;
	static StaticTimer_t s_xTimerBuffers;
	int Error = -1;
    int32_t i;

	// -------------------------------------------------------------------------
	// OS create
	// -------------------------------------------------------------------------
	s_xTimers = xTimerCreateStatic(
				"PeriodicShortTimer",							// Just a text name, not used by the RTOS kernel.
				pdMS_TO_TICKS( OS_PERIODIC_SHORT_TIMER_MS ),	// The timer period in ticks, must be greater than 0.
				pdTRUE,											// The timers will auto-reload themselves when they expire.
				( void * )NULL,									// The ID is used to store a count of the number of times the
																// timer has expired, which is initialised to 0.
				vTimerCallback,									// Each timer calls the same callback when it expires.
				&s_xTimerBuffers								// Pass in the address of a StaticTimer_t variable, which will
																// hold the data associated with the timer being created.
	);

	if(  s_xTimers != NULL ) 	// The timer was created.
	{
		if( xTimerStart( s_xTimers, 0 ) == pdPASS )	// The timer could be set into the Active state.
			Error = 0;
	}

	// -------------------------------------------------------------------------
	// Software init
	// -------------------------------------------------------------------------
    for(i = 0; i < MAX_TIMER; i++)
    {
        g_timer[ i ].pCbFunc 	  = NULL;
        g_timer[ i ].s32TickValue = -1;
    }

	return( Error );
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-14
///
/*----------------------------------------------------------------------------*/
int intOsShortTimer_Register(uint32_t u32PeriodMs, void(* p_CallBack)(void))
{
    int32_t  Error;
    uint32_t i;

    // Check if it has already been registered
    for(i = 0; i < MAX_TIMER; i++)
    {
        if( g_timer[ i ].pCbFunc == p_CallBack )
        {
        	g_timer[ i ].s32TickValue = (int32_t)(u32PeriodMs / OS_PERIODIC_SHORT_TIMER_MS);
            return( 0 );
        }
    }

    Error = -1;
    for(i = 0; i < MAX_TIMER; i++)
    {
        if( g_timer[ i ].s32TickValue == -1 )
        {
        	g_timer[ i ].pCbFunc      = p_CallBack;
        	g_timer[ i ].s32TickValue = (int32_t)(u32PeriodMs / OS_PERIODIC_SHORT_TIMER_MS);
            Error = 0;
            break;
        }
    }

    return( Error );
}
