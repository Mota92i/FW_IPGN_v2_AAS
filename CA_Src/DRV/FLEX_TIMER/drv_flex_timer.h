/******************************************************************************/
/// \file           drv_flex_timer.h
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
//  Inclusion checking
/******************************************************************************/
#ifndef __DRV_FLEX_TIMER_H__
#define __DRV_FLEX_TIMER_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "fsl_ftm.h"
#include "MK64F12.h"
#include "peripherals.h"

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define FTM0_PERIPHERAL	FTM0

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
static inline uint32_t u32DrvFlexTimer_GetTimerCount(void)
{
    return( FTM_GetCurrentTimerCount(FTM0_PERIPHERAL) );
}

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voDrvFlexTimer_Init(void);
extern uint32_t u32DrvFlexTimer_GetTimerCaptureValue(uint8_t *pu8CaptureEventFlag);
#endif /* __DRV_FLEX_TIMER_H__ */

