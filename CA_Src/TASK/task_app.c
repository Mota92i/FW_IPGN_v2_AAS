/******************************************************************************/
/// \file           task_app.c
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
//  Included header file list
/******************************************************************************/
#define public
    #include "task_app.h"
#undef public

#define public extern
	#include <string.h>
	#include <stdbool.h>
    #include "core_spec.h"
    #include "task_voltmetre.h"
    #include "task_main.h"
    #include "os_short_timer.h"
	#include "os_delay.h"
    #include "os_msg_queue.h"
    #include "os_task.h"
	#include "drv_hdw_timer.h"
	#include "core_voltmetre.h"
	#include "core_app.h"
    #include "core_motor.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define TIMEOUT_TICK			( 1000 / portTICK_PERIOD_MS )	// 1sec
#define TIMEOUT_MODE_SIMU_MS	( 30000 )						// 30sec

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
// Task voltmetre
#define PRIORITY_TASK_VOLTMETRE 	(configMAX_PRIORITIES - 3)
#define TASK_VOLTMETRE_STACK_SIZE	( 2000 )
static StaticTask_t g_xTaskVoltmetreTask;
static StackType_t  g_xTaskVoltmetreStackType[ TASK_VOLTMETRE_STACK_SIZE ];

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static const t_task_cfg g_sTaskVoltmetreCfg = {
	.pxTaskCode 	= task_voltmetre,
	.pcName 		= "task_VOLTMETRE",
	.ulStackDepth 	= TASK_VOLTMETRE_STACK_SIZE,
	.pvParameters 	= ( void * ) 1,
	.uxPriority 	= PRIORITY_TASK_VOLTMETRE,
	.puxStackBuffer = &g_xTaskVoltmetreStackType[0],
	.pxTaskBuffer 	= &g_xTaskVoltmetreTask
};

static t_task_data g_sTaskVoltmetre = {
	.bCreate   = true,
	.psTaskCfg = &g_sTaskVoltmetreCfg,
};

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static t_app_measure g_sMeasure = {
		.sMeasureSa  = { .f32Value = 0x8000,
				 	 	 .s8Status = false },

		.sMotorAngle = { .f32Angle  		= 0x8000,
	     	 	 	 	 .f32Course 		= 0x8000,
						 .f32AngleMotor 	= 0x8000,
						 .s8Valid   		= ANGLE_ERROR }
};

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static uint8_t g_u8ModeFct = MODE_FONCTIONNEMENT_NORMAL;

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voModeSimu_ProcessTimeout(void);


/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-11
///
//----------------------------------------------------------------------------*/
static void voModeSimu_ProcessTimeout(void)
{
	g_u8ModeFct = MODE_FONCTIONNEMENT_NORMAL;
}

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
uint8_t u8TaskApp_GetModeFct(void)
{
	return( g_u8ModeFct );
}

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
void voTaskApp_GetSMeasure(t_app_measure *pMeasure)
{
	memcpy(pMeasure, &g_sMeasure, sizeof(g_sMeasure));
}

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
void task_app(void * pvParameters)
{
	uint8_t				u8CopyMeasure;
	int32_t	  			Err_MsgQueue;
	t_app_measure_msg	sMsgMeasure;

	// -------------------------------------------------------------------------
	// Init
	// -------------------------------------------------------------------------
	voCoreApp_Init();

	// -------------------------------------------------------------------------
	// Create task voltmetre
	// -------------------------------------------------------------------------
	if(G_DeviceCfg.psAppDeviceCfg->eInputData == INPUT_DATA_ANALOG )
		voOsTask_Create(&g_sTaskVoltmetre);

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	while(1)
	{
        //----------------------------------------------------------------------
        // Wait for message
        //----------------------------------------------------------------------
		Err_MsgQueue = OsMsgQueue_Receive( OS_MSG_QUEUE_MEASURE, (void *)&sMsgMeasure, TIMEOUT_TICK, OS_MSG_WAIT_MESSAGE );

        //----------------------------------------------------------------------
        // Msg from Voltmetre
        //----------------------------------------------------------------------
		if( Err_MsgQueue == 0 )
		{
	        //------------------------------------------------------------------
	        //
	        //------------------------------------------------------------------
			u8CopyMeasure = true;
			if( sMsgMeasure.eEventType == APP_MEASURE_MSG_QUEUE_SIMU )
			{
				intOsShortTimer_Register(TIMEOUT_MODE_SIMU_MS, voModeSimu_ProcessTimeout);
				g_u8ModeFct = MODE_FONCTIONNEMENT_SIMULATION;
			}
			else if( g_u8ModeFct == MODE_FONCTIONNEMENT_SIMULATION )
			{
				u8CopyMeasure = false;
			}

	        //------------------------------------------------------------------
	        //
	        //------------------------------------------------------------------
			if( u8CopyMeasure == true )
			{
				memcpy((void *)&g_sMeasure.sMeasureSa, (void *)&sMsgMeasure.sMeasureSa, sizeof(g_sMeasure.sMeasureSa));
			}

	        //------------------------------------------------------------------
	        //
	        //------------------------------------------------------------------
			if( g_sMeasure.sMeasureSa.s8Status == true )
			{
				//------------------------------------------------------------------
				// Calculate angle
				//------------------------------------------------------------------
				g_sMeasure.sMotorAngle = sCoreApp_Process(g_sMeasure.sMeasureSa.f32Value);

				//------------------------------------------------------------------
				// Set motor position
				//------------------------------------------------------------------
				if( G_DeviceError.bit.IfDiagModeMotorSetup == false )
				{
					voCoreMotor_SetPositionToMoveToInAngle(g_sMeasure.sMotorAngle.f32AngleMotor);
				}

				//------------------------------------------------------------------
				//
				//------------------------------------------------------------------
				if( g_sMeasure.sMotorAngle.s8Valid == ANGLE_VALID )
					G_sAppCfg.u8IfAngleError = false;
				else
					G_sAppCfg.u8IfAngleError = true;
			}
			else
			{
				G_sAppCfg.u8IfAngleError = true;
			}
		}
	}
}
