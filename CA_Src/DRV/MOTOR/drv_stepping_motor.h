/******************************************************************************/
/// \file           drv_stepping_motor.h
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
//  Inclusion checking
/******************************************************************************/
#ifndef __DRV_STEPPING_MOTOR_H__
#define __DRV_STEPPING_MOTOR_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  constant declaration
/******************************************************************************/
#define MOTOR_FUNDAMENTAL_STEP_ANGLE 	( 9 )	// Datasheet = 0.9°
#define MOTOR_FUNDAMENTAL_STEP_MAX 		( 3600 / MOTOR_FUNDAMENTAL_STEP_ANGLE )

#define MOTOR_MAX_STEP					( MOTOR_FUNDAMENTAL_STEP_MAX * 2 )	// Half step
#define MOTOR_1_STEP_TO_ANGLE			( 4.5 ) // 3600/ 800 = 4.5 (1/10 °) => 0.45°


#define MOTOR_MAX_PHASE					( 8 )

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voDrvSteppingMotor_SetPhase(uint8_t u8Step);
extern uint16_t u16DrvSteppingMotor_AngleToStep(uint16_t u16Angle);
#endif /* __DRV_STEPPING_MOTOR_H__ */

