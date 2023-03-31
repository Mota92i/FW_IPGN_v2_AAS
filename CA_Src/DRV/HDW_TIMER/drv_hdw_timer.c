/******************************************************************************/
/// \file           drv_hdw_timer.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-11-04
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
    #include "drv_hdw_timer.h"
#undef public
	
#define public extern
	#include <stdbool.h>
	#include <string.h>

    #include "os_semaphore.h"
    #include "os_eventflag.h"

	#include "fsl_ftm.h"
	#include "peripherals.h"

	#include "FreeRTOS.h"
	#include "task.h"
#undef public

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
enum{
	TIMER_NOT_USED		= 0,
	TIMER_OS_NO,
	TIMER_OS_PERIODIC,
	TIMER_OS_ONE_SHOT,

	TIMER_COUNT
};

/******************************************************************************/
//  Variable declarations
/******************************************************************************/
#define MAX_TIMER       ( 9 )

typedef void(* t_p_timer_periodic_500_cb)(void);
static volatile struct {
	t_p_timer_periodic_500_cb	pFunc;
	uint32_t 					u32Id;
	uint32_t 					u32Type;
	uint32_t 					u32Time;
	uint32_t 					u32Cpt;
}g_sTimer[ MAX_TIMER ];

/******************************************************************************/
//  Function definitions
/******************************************************************************/
static int32_t DrvHdwTimer_Register(uint32_t u32TimerType, uint32_t u32Data, uint32_t u32Delay);
static int32_t DrvHdwTimer_DeRegister(uint32_t u32TimerType, uint32_t u32Data);
static void voTimer_Process(void);

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
/// \date           2020-11-04
///
/*----------------------------------------------------------------------------*/
void FTM2_DriverIRQHandler(void)
{
	UBaseType_t uxSavedInterruptStatus;		// Declare a variable in which the return value
											// from taskENTER_CRITICAL_FROM_ISR() will be saved.

	// This part of the ISR can be interrupted by any higher priority interrupt.
	// Use taskENTER_CRITICAL_FROM_ISR() to protect a region of this ISR. Save the value
	// returned from taskENTER_CRITICAL_FROM_ISR() so it can be passed into the matching call to
	// taskEXIT_CRITICAL_FROM_ISR().
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

	voTimer_Process();

	// This part of the ISR is between the call to taskENTER_CRITICAL_FROM_ISR() and
	// taskEXIT_CRITICAL_FROM_ISR(), so can only be interrupted by interrupts that have a
	// priority above that set by the configMAX_SYSCALL_INTERRUPT_PRIORITY constant.
	// Exit the critical section again by calling taskEXIT_CRITICAL_FROM_ISR(), passing in
	// the value returned by the matching call to taskENTER_CRITICAL_FROM_ISR().
	taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

	FTM_ClearStatusFlags(FTM2_PERIPHERAL, kFTM_TimeOverflowFlag);
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2020-11-04
///
/*----------------------------------------------------------------------------*/
static void voTimer_Process(void)
{
    uint8_t  i;
    uint32_t u32FreeFlag = 0;

    //------------------------------------------------------------------------------
	//
    //------------------------------------------------------------------------------
	for(i = 0; i < MAX_TIMER; i++)
	{
		if( g_sTimer[ i ].u32Type == TIMER_NOT_USED )
			continue;

		g_sTimer[ i ].u32Cpt++;
		if( g_sTimer[ i ].u32Cpt >= g_sTimer[ i ].u32Time )
		{
			g_sTimer[ i ].u32Cpt = 0;
			switch( g_sTimer[ i ].u32Type )
			{
				case TIMER_OS_NO :
					if( g_sTimer[ i ].pFunc != NULL )
						g_sTimer[ i ].pFunc();
					break;

				case TIMER_OS_PERIODIC :
					u32FreeFlag |= g_sTimer[ i ].u32Id;
					break;

				case TIMER_OS_ONE_SHOT :
					u32FreeFlag |= g_sTimer[ i ].u32Id;
					g_sTimer[ i ].u32Type = TIMER_NOT_USED;
					break;
			}
		}
	}

    //------------------------------------------------------------------------------
	//
    //------------------------------------------------------------------------------
	voOsEventFlag_uSecDlyTmrPost(u32FreeFlag);
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2020-11-04
///
/*----------------------------------------------------------------------------*/
static int32_t DrvHdwTimer_Register(uint32_t u32TimerType, uint32_t u32Data, uint32_t u32Delay)
{
    uint32_t i;
    int32_t  Error;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
    if( u32Delay == 0 )
    	return( 0 );

    if( (u32TimerType <= TIMER_NOT_USED) || (u32TimerType >= TIMER_COUNT) )
    {
    	return( -1 );
    }

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	voOsSemaphore_MsDlyTmrTake();

	// Call taskENTER_CRITICAL() to create a critical section.
	taskENTER_CRITICAL();

	// -------------------------------------------------------------------------
	// Check if it has already been registered
	// -------------------------------------------------------------------------
	Error = -1;
	for(i = 0; i < MAX_TIMER; i++)
	{
		if( Error == 0 )
			break;

		if( g_sTimer[ i ].u32Type == u32TimerType )
		{
			switch( u32TimerType )
			{
				case TIMER_OS_NO :
					if( g_sTimer[ i ].pFunc == (t_p_timer_periodic_500_cb)u32Data )
						Error = 0;
					break;

				case TIMER_OS_PERIODIC :
				case TIMER_OS_ONE_SHOT :
					if( g_sTimer[ i ].u32Id == (uint32_t)u32Data )
						Error = 0;
					break;
			}
		}
	}

	// -------------------------------------------------------------------------
	// Get an area
	// -------------------------------------------------------------------------
	if( Error == -1 )
	{
		for(i = 0; i < MAX_TIMER; i++)
		{
			if( g_sTimer[ i ].u32Type == TIMER_NOT_USED )
			{
				switch( u32TimerType )
				{
					case TIMER_OS_NO 		: g_sTimer[ i ].pFunc = (t_p_timer_periodic_500_cb)u32Data;
					break;

					case TIMER_OS_PERIODIC 	:
					case TIMER_OS_ONE_SHOT 	: g_sTimer[ i ].u32Id = (uint32_t)u32Data;
					break;
				}

				g_sTimer[ i ].u32Type = u32TimerType;
				g_sTimer[ i ].u32Cpt  = 0;
				g_sTimer[ i ].u32Time = u32Delay;
				Error = 0;
				break;
			}
		}
	}

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	// After this call to taskEXIT_CRITICAL(), the nesting depth
	// will be zero, so interrupts will have been re-enabled.
	taskEXIT_CRITICAL();

	voOsSemaphore_MsDlyTmrGive();

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
/// \date           2020-11-04
///
/*----------------------------------------------------------------------------*/
static int32_t DrvHdwTimer_DeRegister(uint32_t u32TimerType, uint32_t u32Data)
{
	int32_t Error;
    uint32_t i;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
    if( (u32TimerType <= TIMER_NOT_USED) || (u32TimerType >= TIMER_COUNT) )
    {
    	return( -1 );
    }

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	voOsSemaphore_MsDlyTmrTake();

	// Call taskENTER_CRITICAL() to create a critical section.
	taskENTER_CRITICAL();

	// -------------------------------------------------------------------------
	// Check if it has already been registered
	// -------------------------------------------------------------------------
	Error = -1;
	for(i = 0; i < MAX_TIMER; i++)
	{
		if( Error == 0 )
			break;

		if( g_sTimer[ i ].u32Type == u32TimerType )
		{
			switch( u32TimerType )
			{
				case TIMER_OS_NO :
					if( g_sTimer[ i ].pFunc == (t_p_timer_periodic_500_cb)u32Data )
					{
						memset((void *)&g_sTimer[ i ], 0, sizeof(g_sTimer[ 0 ]));
						Error = 0;
					}
					break;

				case TIMER_OS_PERIODIC :
				case TIMER_OS_ONE_SHOT :
					if( g_sTimer[ i ].u32Id == (uint32_t)u32Data )
					{
						memset((void *)&g_sTimer[ i ], 0, sizeof(g_sTimer[ 0 ]));
						Error = 0;
					}
					break;
			}
		}
	}

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	// After this call to taskEXIT_CRITICAL(), the nesting depth
	// will be zero, so interrupts will have been re-enabled.
	taskEXIT_CRITICAL();

	voOsSemaphore_MsDlyTmrGive();

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
/// \date           2018-10-11
///
/*----------------------------------------------------------------------------*/
int32_t DrvHdwTimer_SetDelayAndWait(uint32_t u32ProcessID, uint32_t u32Delay)
{
	int32_t Error = 0;

	if( u32Delay > 0 )
	{
		Error = DrvHdwTimer_Register(TIMER_OS_ONE_SHOT, u32ProcessID, u32Delay);
		while( Error == -1);

		voOsEventFlag_uSecDlyTmrWait(u32ProcessID);
	}

	return( Error );
}

int32_t DrvHdwTimer_RegisterPeriodicEvent(uint32_t u32ProcessID, uint32_t u32Delay)
{
	int32_t Error;

	Error = DrvHdwTimer_Register(TIMER_OS_PERIODIC, u32ProcessID, u32Delay);
	while( Error == -1);

	return( Error );
}

int32_t DrvHdwTimer_RegisterPeriodic500us( void(* p_CallBack)(void), uint32_t u32Period )
{
	int32_t Error;

	Error = DrvHdwTimer_Register(TIMER_OS_NO, (uint32_t)p_CallBack, u32Period);
	while( Error == -1);

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
/// \date           2018-10-11
///
/*----------------------------------------------------------------------------*/
int32_t DrvHdwTimer_DeRegisterPeriodic500us( void(* p_CallBack)(void) )
{
    return( DrvHdwTimer_DeRegister(TIMER_OS_NO, (uint32_t)p_CallBack) );
}

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
void voDrvHdwTimer_Init(void)
{
    memset((void *)&g_sTimer[0], 0, sizeof(g_sTimer));

    FTM_StartTimer(FTM2_PERIPHERAL, kFTM_SystemClock);
}
