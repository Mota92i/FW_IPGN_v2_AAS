/******************************************************************************/
/// \file           drv_eeprom.c
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
    #include "drv_debouncer.h"
#undef public

#define public extern
	#include <stdbool.h>
	#include <string.h>
    #include "drv_board_gpio.h"
	#include "drv_hdw_timer.h"
#undef public

/******************************************************************************/
//  Type declarations
/******************************************************************************/
typedef struct
{
	uint8_t u8Enable;

	uint8_t(* p_GetPortVal)(void);

	struct{
		uint8_t u8Inst;
		uint8_t u8Filter;
	}sVal;
	uint16_t u16CptMax;
    uint16_t u16Cpt;
} t_gpio_debouncer;

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static t_gpio_debouncer g_sGpioDebouncer;

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
void voDebouncer_Process(void)
{
	if( g_sGpioDebouncer.u8Enable == true )
	{
		g_sGpioDebouncer.sVal.u8Inst = g_sGpioDebouncer.p_GetPortVal();

		// No change on input
		if( g_sGpioDebouncer.sVal.u8Filter == g_sGpioDebouncer.sVal.u8Inst )
		{
			g_sGpioDebouncer.u16Cpt = 0;
		}
		else
		{
			// Input has changed
			g_sGpioDebouncer.u16Cpt++;
			if( g_sGpioDebouncer.u16Cpt >= g_sGpioDebouncer.u16CptMax )
			{
				g_sGpioDebouncer.sVal.u8Filter = g_sGpioDebouncer.sVal.u8Inst;
				g_sGpioDebouncer.u16Cpt = 0;
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
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
void voDebouncer_Init(void)
{
	g_sGpioDebouncer.u8Enable    	= true;
	g_sGpioDebouncer.p_GetPortVal   = DrvGpio_GetSwitchState;
	g_sGpioDebouncer.sVal.u8Inst 	= g_sGpioDebouncer.p_GetPortVal();
	g_sGpioDebouncer.sVal.u8Filter 	= g_sGpioDebouncer.sVal.u8Inst;
	g_sGpioDebouncer.u16Cpt      	= 0;
	g_sGpioDebouncer.u16CptMax   	= 20;	// 20ms

	DrvHdwTimer_RegisterPeriodic500us(voDebouncer_Process, DRV_HDW_TIMER_MILLI_SEC( 1 ));
}

uint8_t u8Debouncer_GetSwitchState(void)
{
	return( g_sGpioDebouncer.sVal.u8Filter );
}
