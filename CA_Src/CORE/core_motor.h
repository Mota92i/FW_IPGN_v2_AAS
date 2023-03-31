//*****************************************************************************/
/// \file           core_motor.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-10-18
/// \copyright      Chauvin Arnoux Energy
///
/// \ingroup        
/// \brief          
/// \details        
///                 
//*****************************************************************************/

//*****************************************************************************/
//  Inclusion checking
//*****************************************************************************/
#ifndef _CORE_MOTOR_H_
#define _CORE_MOTOR_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include "app_types.h"
#include <stdint.h>

//*****************************************************************************/
//  Multi-file-scope function prototype declarations
//*****************************************************************************/
extern void 			voCoreMotor_SetPositionToMoveToInAngle(uint16_t u16PosToMoveTo_Angle);
extern void 			voCoreMotor_Init(void);
extern void 			voCoreMotor_Process(void);
extern t_motor_position sCoreMotor_ProcessRecalage(uint8_t u8ModeDiag);
extern uint16_t 		u16CoreMotor_CheckAngle(int16_t s16Angle);
#endif // _CORE_MOTOR_H_
