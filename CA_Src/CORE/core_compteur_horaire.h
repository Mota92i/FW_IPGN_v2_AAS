/******************************************************************************/
/// \file           core_compteur_horaire.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-12-03
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
#ifndef __CORE_COMPTEUR_HORAIRE_H__
#define __CORE_COMPTEUR_HORAIRE_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern int32_t  s32CoreCompteurHoraire_Process(uint32_t u32PeriodMs);
extern int32_t  s32CoreCompteurHoraire_Init(uint32_t u32ValCpt);
extern uint32_t u32ReadCptValue(void);
#endif /* __CORE_COMPTEUR_HORAIRE_H__ */
