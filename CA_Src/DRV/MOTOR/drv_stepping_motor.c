/******************************************************************************/
/// \file           drv_stepping_motor.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-10-20
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
    #include "drv_stepping_motor.h"
#undef public

#define public extern
	#include "fsl_gpio.h"
	#include "pin_mux.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
typedef enum {
	PHASE_YELLOW = 0,
	PHASE_RED,
	PHASE_BLUE,
	PHASE_ORANGE,

	PHASE_COUNT
}t_motor_phase;

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/


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
/// \date           2021-10-20
///
//----------------------------------------------------------------------------*/
void voDrvSteppingMotor_SetPhase(uint8_t u8Step)
{
	// Rotation par demi-pas
	const uint8_t u8ArrayPhase[MOTOR_MAX_PHASE] = {
			0x01, // Pas n°1		=> Phase Yellow
			0x09, // Pas n°1 1/2	=> Phase Yellow - Orange
			0x08, // Pas n°2		=> Phase Orange
			0x0A, // Pas n°2 1/2	=> Phase Orange - Red
			0x02, // Pas n°3		=> Phase Red
			0x06, // Pas n°3 1/2	=> Phase Red    - Blue
			0x04, // Pas n°4		=> Phase Blue
			0x05  // Pas n°4 1/2	=> Phase Blue   - Yellow
	};
	const struct{
		GPIO_Type * const pPort;
		uint32_t  pin;
	} sgpio[] = {
		[ PHASE_YELLOW ] = { BOARD_INITPINS_MOTOR_YELLOW_GPIO, 	BOARD_INITPINS_MOTOR_YELLOW_PIN },
		[ PHASE_RED    ] = { BOARD_INITPINS_MOTOR_RED_GPIO,		BOARD_INITPINS_MOTOR_RED_PIN    },
		[ PHASE_BLUE   ] = { BOARD_INITPINS_MOTOR_BLUE_GPIO,	BOARD_INITPINS_MOTOR_BLUE_PIN   },
		[ PHASE_ORANGE ] = { BOARD_INITPINS_MOTOR_ORANGE_GPIO,	BOARD_INITPINS_MOTOR_ORANGE_PIN },
	};
	uint8_t u8Num;
	uint8_t u8Phase;

	if( u8Step < MOTOR_MAX_PHASE )
	{
		u8Phase = u8ArrayPhase[ u8Step ];
		for(u8Num = 0; u8Num < PHASE_COUNT; u8Num++)
		{
			GPIO_PinWrite(sgpio[ u8Num ].pPort, sgpio[ u8Num ].pin, (uint8_t)(u8Phase & 0x01));

			u8Phase = u8Phase >> 1;
		}
	}
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      u16Angle => accuracy 1/10°
/// \param[out]     none
/// \return         u16Step
///
/// \version        1.0
/// \date           2021-10-20
///
//----------------------------------------------------------------------------*/
uint16_t u16DrvSteppingMotor_AngleToStep(uint16_t u16Angle)
{
	float 	 f_Step;
	uint16_t u16Step;

	f_Step = (float)u16Angle / (float)MOTOR_1_STEP_TO_ANGLE;

	u16Step = f_Step;
	if( (f_Step - (float)u16Step) >= (float)0.5 )
		u16Step++;

	if( u16Step >= MOTOR_MAX_STEP )
		u16Step = 0;

	return( u16Step );
}

