//*****************************************************************************/
/// \file           core_voltmetre.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2022-01-24
/// \copyright      Chauvin Arnoux Energy
///
/// \ingroup
/// \brief
/// \details
///
//*****************************************************************************/
//  Software configuration management
//*****************************************************************************/

//*****************************************************************************/
//  Inclusion checking
//*****************************************************************************/
#ifndef _CORE_VOLTMETRE_H_
#define _CORE_VOLTMETRE_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include "app_types.h"
#include <stdint.h>

/******************************************************************************/
//  Type declaration
/******************************************************************************/
typedef struct{
	uint8_t  u8Voie;
	int8_t   s8Status;
	int16_t  s16N;                       	// Case de transfert au PP de la variable NVR
	uint16_t u16t1;          				// Temps de D�but de mesure m�moris� par l'input capture
	uint32_t u32temps;          			// Temps global de la mesure (entre les 2 input capture)
}t_voltmetre_mesure;

//*****************************************************************************/
//  Multi-file-scope function prototype declarations
//*****************************************************************************/
extern void voCoreVoltmetre_Init(void);
extern void voCoreVoltmetre_ProcessIt(void);
extern void voCoreVoltmetre_Process(t_voltmetre_mesure *psVoltmetre);
extern void voCoreVoltmetre_GetMeasure(t_measure *psMeasure);
extern int8_t s8CoreVoltmetre_SetEtalonnage(uint8_t u8Cmd, uint16_t u16Voie);
#endif /* _CORE_VOLTMETRE_H_ */
