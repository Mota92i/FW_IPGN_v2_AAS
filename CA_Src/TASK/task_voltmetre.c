/******************************************************************************/
/// \file           task_app.c
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
    #include "task_voltmetre.h"
#undef public

#define public extern
	#include "drv_hdw_timer.h"
	#include "core_voltmetre.h"
    #include "os_msg_queue.h"
    #include "drv_board_gpio.h"
	#include "FreeRTOS.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define TIMEOUT		( 1000 / portTICK_PERIOD_MS )	// 1sec

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
void task_voltmetre(void * pvParameters)
{
	int32_t				Err_MsgQueue;
	t_voltmetre_mesure 	sVoltmetre;
	t_app_measure_msg	sMsgMeasure;

	// -------------------------------------------------------------------------
	// Init
	// -------------------------------------------------------------------------
	voCoreVoltmetre_Init();

	// -------------------------------------------------------------------------
	// Traitement périodic du voltmétre à 500us
	// -------------------------------------------------------------------------
	DrvHdwTimer_RegisterPeriodic500us(voCoreVoltmetre_ProcessIt, DRV_HDW_TIMER_MICRO_SEC( 500 ));

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	while(1)
	{
        //----------------------------------------------------------------------
        // Wait for message
        //----------------------------------------------------------------------
        Err_MsgQueue = OsMsgQueue_Receive( OS_MSG_QUEUE_VOLTMETRE, (void *)&sVoltmetre, TIMEOUT, OS_MSG_WAIT_MESSAGE );
        if( Err_MsgQueue >= 0 )
			voCoreVoltmetre_Process(&sVoltmetre);

        //----------------------------------------------------------------------
        // Send measure
        //----------------------------------------------------------------------
        if( Err_MsgQueue == 0 )
        {
        	if( sVoltmetre.u8Voie == VOLTMETRE_VOIE_COURANT )
        	{
        		sMsgMeasure.eEventType = APP_MEASURE_MSG_QUEUE_VOLTMETRE;
        		voCoreVoltmetre_GetMeasure(&sMsgMeasure.sMeasureSa);
        		OsMsgQueue_SendToBack(OS_MSG_QUEUE_MEASURE, &sMsgMeasure);
        	}
        }
	}
}
