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
//  Included header file list
/******************************************************************************/
#define public
    #include "task_display.h"
#undef public

#define public extern
	#include <string.h>
	#include <stdio.h>
	#include <stdbool.h>
    #include "drv_debouncer.h"
	#include "drv_hdw_timer.h"
    #include "os_eventflag.h"
	#include "drv_display.h"
    #include "core_spec.h"
    #include "task_main.h"

    #include "drv_board_gpio.h"
    #include "os_timer.h"
    #include "drv_pwm.h"
    #include "core_motor.h"
    #include "core_app.h"
#undef public

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define PERIODE_100MS		( 100 )
#define PERIODE_5000MS		( 5000 )
#define PERIODE_25000MS		( 25000 )

#define BLINKING_LED_RATE	( 200 )
#define BLINK_LED_TRUE		( 0x01 )
#define BLINK_LED_FALSE		( 0x00 )

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static t_motor_position g_sMotorPosition = {.u16Step = 0x8000, .u16Angle = 0x8000};

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voLuminositeProcess(void);
static void voPeriodicTimer_DiplayLed(void);
static void voSet_Luminosity(uint8_t u8Lum);
static void voDisplayMotor_Process(void);

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
static void voSet_Luminosity(uint8_t u8Lum)
{
	const uint16_t u6TabLum[] = {
// Luminosité (0 -> 30)  :  0      1      2      3      4      5      6      7      8      9     10     11     12     13     14     15
						  49998, 49920, 49826, 49717, 49589, 49440, 49266, 49063, 48825, 48548, 48224, 47845, 47403, 46886, 46282, 45577,
//                         16     17     18     19     20     21     22     23     24     25     26     27     28     29   30
						  44753, 43791, 42667, 41353, 39819, 38026, 35933, 33486, 30629, 27291, 23391, 18836, 13514, 7297, 35
	};

	if( u8Lum < (sizeof(u6TabLum) / sizeof(u6TabLum[0])) )
		voDrvPwm_SetLedBrightness( ((uint32_t)u6TabLum[u8Lum] * 3)  / 5);
//		FTM2_C1V = (uint32_t)TabLum[u8Lum];
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
static void voPeriodicTimer_DiplayLed(void)
{
	static uint8_t u8BlinkMask = 0;
	uint8_t 	   i;

	if( G_sAppCfg.u8IfAngleError == true )
		voSet_Luminosity( DISPLAY_BRIGHTNESS_MAX - 3 );
	else
		voSet_Luminosity(G_DeviceCfg.u8Brightness);

	u8BlinkMask	= ~u8BlinkMask & BLINK_LED_TRUE;

	for(i = 0; i < DISPLAY_LUM_COUNT; i++)
	{
		if( (u8DrvGpio_ReadLedCmd( i ) == 0)    		||
			((G_sAppCfg.u8IfAngleError & u8BlinkMask) == BLINK_LED_TRUE) )
		{
			voDrvGpio_OffLedLum( i );
		}
		else
		{
			voDrvGpio_OnLedLum( i );
		}
	}
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
static void voLuminositeProcess(void)
{
	static uint8_t u8IncOrDec			= 0;
	static uint8_t u8Cpt_5000ms    		= (PERIODE_5000MS / PERIODE_100MS);	//5sec
	static uint8_t u8SaveLumValueTempo	= 0;
	static uint8_t u8MemoToucheLum		= 0;

	uint8_t u8SwState = u8Debouncer_GetSwitchState();

	// Detection laché de la touche pour soit incrémenter ou décrementer
    if( (u8SwState == 0) && (u8MemoToucheLum == 1) )
    	u8IncOrDec = ~u8IncOrDec;

    // Memo l'etat courant
    u8MemoToucheLum = u8SwState;

	// Appuie sur touche
	if( u8SwState == 1 )
	{
		if( u8Cpt_5000ms == 0 )			// Valeur par défaut si appui > à 5sec
		{
			G_DeviceCfg.u8Brightness = DISPLAY_BRIGHTNESS_MIN;
			u8IncOrDec 				 = 0;
		}
		else
		{
			u8Cpt_5000ms--;

			if( u8IncOrDec )
			{
				if( G_DeviceCfg.u8Brightness < DISPLAY_BRIGHTNESS_MAX )
					G_DeviceCfg.u8Brightness++;
			}
			else
			{
				if( G_DeviceCfg.u8Brightness > DISPLAY_BRIGHTNESS_MIN )
					G_DeviceCfg.u8Brightness--;
			}

			voSet_Luminosity(G_DeviceCfg.u8Brightness);
		}

		// Mémorisation de la valeur au bout de 25sec aprés relachement
		u8SaveLumValueTempo = (PERIODE_25000MS / PERIODE_100MS);
	}
	else
	{
		u8Cpt_5000ms = (PERIODE_5000MS / PERIODE_100MS);	//5sec
	}

	// Sauvegarde de la luminosité
	if( u8SaveLumValueTempo > 0 )
	{
	    if( --u8SaveLumValueTempo == 0 )
	    {
	    	if( CoreSpec_BrightnessWrite() == -1 )
	    	{
	    		G_DeviceCfg.u8Brightness = DISPLAY_BRIGHTNESS_DEFAULT;
	    		voDrvDisplay_SetLum( G_DeviceCfg.u8Brightness );
	    	}
	    }
	}
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
static void voDisplayMotor_Process(void)
{
	voCoreMotor_Process();
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
void voTaskApp_GetMotorPosition(t_motor_position *pMeasure)
{
	memcpy(pMeasure, &g_sMotorPosition, sizeof(g_sMotorPosition));
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
void task_display(void * pvParameters)
{
	uint8_t i;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	for(i = 0; i < DISPLAY_LUM_COUNT; i++)
	{
		voDrvGpio_OffLedLum( i );
	}

	// -------------------------------------------------------------------------
	// Motor Process
	// -------------------------------------------------------------------------
	DrvHdwTimer_RegisterPeriodic500us(voDisplayMotor_Process, DRV_HDW_TIMER_MICRO_SEC( 500 ));
	voCoreMotor_Init();

	// -------------------------------------------------------------------------
	// Init
	// -------------------------------------------------------------------------
	voDrvDisplay_SetLum( G_DeviceCfg.u8Brightness );
	voDebouncer_Init();

	// -------------------------------------------------------------------------
	// Init
	// -------------------------------------------------------------------------
	if( intOsTimer_Create(BLINKING_LED_RATE, voPeriodicTimer_DiplayLed) != 0 )
		while( 1 );

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	DrvHdwTimer_RegisterPeriodicEvent(TIMER_PERIODIC_MILLI_SEC_DELAY_DISPLAY, DRV_HDW_TIMER_MILLI_SEC(PERIODE_100MS));

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	while(1)
	{
		voOsEventFlag_uSecDlyTmrWait(TIMER_PERIODIC_MILLI_SEC_DELAY_DISPLAY);

		// ---------------------------------------------------------------------
		// todo
		// ---------------------------------------------------------------------
		g_sMotorPosition = sCoreMotor_ProcessRecalage((uint8_t)G_DeviceError.bit.IfDiagMode);

		// ---------------------------------------------------------------------
		//
		// ---------------------------------------------------------------------
		voLuminositeProcess();
	}
}
