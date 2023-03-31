/******************************************************************************/
/// \file           task_main.c
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
    #include "task_main.h"
#undef public

#define public extern
	#include <stdbool.h>
	#include "MK64F12.h"

    #include "drv_serial.h"
	#include "drv_board_gpio.h"
	#include "drv_hdw_timer.h"
	#include "task_display.h"
	#include "task_serial.h"
	#include "task_voltmetre.h"
	#include "task_app.h"
	#include "os_delay.h"
	#include "os_eventflag.h"
	#include "os_semaphore.h"
	#include "os_msg_queue.h"
	#include "core_spec.h"
	#include "core_cmd_diagnostic.h"
	#include "core_compteur_horaire.h"
    #include "os_short_timer.h"
    #include "os_task.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#undef public      

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
uint8_t g_u8ResetCpt = 0;

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define DELAY_TO_RESET							( 20 )
#define MAIN_TASK_PERIODIC_DELAY_MS				( 100 )
#define MAIN_TASK_BACKGROUND_SAVE_EEPROMMS		( 1000 )

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
enum{
	TASK_DISPLAY = 0,
	TASK_SERIAL,
	TASK_APP,

	TASK_COUNT
};

// Task priorities.
#define PRIORITY_TASK_DISPLAY 	(configMAX_PRIORITIES - 1)
// Dimensions the buffer that the task being created will use as its stack.
// NOTE:  This is the number of words the stack will hold, not the number of
// bytes.  For example, if each stack item is 32-bits, and this is set to 100,
// then 400 bytes (100 * 32-bits) will be allocated.
#define TASK_DISPLAY_STACK_SIZE	( 2000 )
// Structure that will hold the TCB of the task being created.
static StaticTask_t g_xTaskDisplayTask;
// Buffer that the task being created will use as its stack.  Note this is
// an array of StackType_t variables.  The size of StackType_t is dependent on
// the RTOS port.
static StackType_t g_xTaskDisplayStackType[ TASK_DISPLAY_STACK_SIZE ];

// Task display
#define PRIORITY_TASK_SERIAL 	(configMAX_PRIORITIES - 2)
#define TASK_SERIAL_STACK_SIZE	( 2000 )
static StaticTask_t g_xTaskSerialTask;
static StackType_t g_xTaskSerialStackType[ TASK_SERIAL_STACK_SIZE ];

// Task app
#define PRIORITY_TASK_APP 	(configMAX_PRIORITIES - 3)
#define TASK_APP_STACK_SIZE	( 2000 )
static StaticTask_t g_xTaskAppTask;
static StackType_t g_xTaskAppStackType[ TASK_APP_STACK_SIZE ];

static const t_task_cfg g_sTaskCfg[ TASK_COUNT ] = {
	[ TASK_DISPLAY ] = {
			.pxTaskCode 	= task_display,
			.pcName 		= "task_display",
			.ulStackDepth 	= TASK_DISPLAY_STACK_SIZE,
			.pvParameters 	= ( void * ) 1,
			.uxPriority 	= PRIORITY_TASK_DISPLAY,
			.puxStackBuffer = &g_xTaskDisplayStackType[0],
			.pxTaskBuffer 	= &g_xTaskDisplayTask
	},
	[ TASK_SERIAL ] = {
			.pxTaskCode 	= task_serial,
			.pcName 		= "task_serial",
			.ulStackDepth 	= TASK_SERIAL_STACK_SIZE,
			.pvParameters 	= ( void * ) 1,
			.uxPriority 	= PRIORITY_TASK_SERIAL,
			.puxStackBuffer = &g_xTaskSerialStackType[0],
			.pxTaskBuffer 	= &g_xTaskSerialTask
	},
	[ TASK_APP     ] = {
			.pxTaskCode 	= task_app,
			.pcName 		= "task_app",
			.ulStackDepth 	= TASK_APP_STACK_SIZE,
			.pvParameters 	= ( void * ) 1,
			.uxPriority 	= PRIORITY_TASK_APP,
			.puxStackBuffer = &g_xTaskAppStackType[0],
			.pxTaskBuffer 	= &g_xTaskAppTask
	},
};

static t_task_data g_sTask[ TASK_COUNT ] = {
	[ TASK_DISPLAY ] = {
			.bCreate   = true,
			.psTaskCfg = &g_sTaskCfg[ TASK_DISPLAY ],
	},
	[ TASK_SERIAL ] = {
			.bCreate   = true,
			.psTaskCfg = &g_sTaskCfg[ TASK_SERIAL ],
	},
	[ TASK_APP      ] = {
			.bCreate   = true,
			.psTaskCfg = &g_sTaskCfg[ TASK_APP ],
	},
};

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static struct{
	uint8_t bRequestSave;
	uint8_t u8Tick;
}g_sSaveEepromBackGround;

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voCreateAllTask(void);

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
static void voCreateAllTask(void)
{
	uint8_t i;

	for(i = 0; i < TASK_COUNT; i++)
	{
		voOsTask_Create( &g_sTask[ i ] );
	}
}

/*----------------------------------------------------------------------------*/
//  main
/*----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return			none
///
/// \brief         main loop
///
/// \details
/*----------------------------------------------------------------------------*/
void voTaskMain_FlagResetSet(void)
{
	G_DeviceError.bit.reset = 1;
	g_u8ResetCpt = 0;
}

void voTaskMain_FlagErrorSetMacAdr(void)
{
	G_DeviceError.bit.mac_addr = 1;
}

/*----------------------------------------------------------------------------*/
//  main
/*----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return			none
///
/// \brief         main loop
///
/// \details
/*----------------------------------------------------------------------------*/
void voTaskMain_SetBackGroundSave(void)
{
	g_sSaveEepromBackGround.u8Tick = MAIN_TASK_BACKGROUND_SAVE_EEPROMMS / MAIN_TASK_PERIODIC_DELAY_MS;
	g_sSaveEepromBackGround.bRequestSave = true;
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
void task_main(void * pvParameters)
{
	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	g_sSaveEepromBackGround.bRequestSave = false;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	voOsEventFlag_Init();
	voOsSemaphore_Init();
	voOsMsgQueue_Init();
	voDrvHdwTimer_Init();

	intOsShortTimer_Init();

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	voDrvSerial_Init();

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
/*	for(i = 0; i < DISPLAY_LUM_COUNT; i++)
		voDrvGpio_OffLedLum( i );

	for(i = 0; i < DISPLAY_LUM_COUNT; i++)
	{
	voDrvGpio_OnLedLum( i );
	voDrvGpio_OffLedLum( i );
	}
*/
	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	G_DeviceError.u8Val = 0;
	CoreSpec_Load();
	voCoreCmdDiagnostic_Init();

    G_DeviceCfg.u8SerialReqStatus 		 = 0;
	G_DeviceCfg.u8SerialReqNumAbonneeLed = 0;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	voCreateAllTask();

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	while(1)
	{
        //----------------------------------------------------------------------
        // Rfresh wdog
        //----------------------------------------------------------------------
//		DrvWdog_Refresh();

        //----------------------------------------------------------------------
        // Reset the MCU
        //----------------------------------------------------------------------
		if( G_DeviceError.bit.reset )
		{
			g_u8ResetCpt = (g_u8ResetCpt + 1) % DELAY_TO_RESET;
			if( g_u8ResetCpt == 0 )
			{
				NVIC_SystemReset();
			}
		}

        //----------------------------------------------------------------------
        //
        //----------------------------------------------------------------------
		s32CoreCompteurHoraire_Process( MAIN_TASK_PERIODIC_DELAY_MS );

        //----------------------------------------------------------------------
        //
        //----------------------------------------------------------------------
		voOsDelay_Delay( MAIN_TASK_PERIODIC_DELAY_MS );
	}
}
