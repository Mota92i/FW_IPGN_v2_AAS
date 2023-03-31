/******************************************************************************/
/// \file           task_main.h
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
//  Inclusion checking
/******************************************************************************/
#ifndef _TASK_MAIN_H_
#define _TASK_MAIN_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
public struct
{
	__attribute__((packed)) union{
		uint8_t u8Val;
		struct {
			  uint8_t reset 				: 1;
			  uint8_t mac_addr 				: 1;
			  uint8_t IfDiagMode			: 1;
			  uint8_t IfDiagModeMotorSetup	: 1;

		}__attribute__((packed)) bit;
	};
}__attribute__((packed)) G_DeviceError;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void task_main(void * pvParameters);
extern void voTaskMain_FlagResetSet(void);
extern void voTaskMain_FlagErrorSetMacAdr(void);
extern void voTaskMain_SetBackGroundSave(void);
#endif /* _TASK_MAIN_H_ */
