/******************************************************************************/
/// \file           drv_hdw_timer.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-11-04
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
#ifndef __DRV_HDW_TIMER_H__
#define __DRV_HDW_TIMER_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define PERIOD_TIMER_IT		( 500 )		// 500us == 2KHz

#define DRV_HDW_TIMER_MILLI_SEC(millisec)		(uint32_t)(millisec * (1000 / PERIOD_TIMER_IT))
#define DRV_HDW_TIMER_MICRO_SEC(microsec)		(uint32_t)(microsec / PERIOD_TIMER_IT)

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum
{
    // Handled by the Usec timer
    TIMER_DELAY_ID_SERIAL_1                  = (0x01 << 0),


	TIMER_PERIODIC_MILLI_SEC_DELAY_DISPLAY   = (0x01 << 20),
};

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voDrvHdwTimer_Init(void);
extern int32_t DrvHdwTimer_RegisterPeriodic500us( void(* p_CallBack)(void), uint32_t u32Period );
extern int32_t DrvHdwTimer_DeRegisterPeriodic500us( void(* p_CallBack)(void) );
extern int32_t DrvHdwTimer_SetDelayAndWait(uint32_t u32ProcessID, uint32_t u32Delay);
extern int32_t DrvHdwTimer_RegisterPeriodicEvent(uint32_t u32ProcessID, uint32_t u32Delay);

#endif /* __DRV_HDW_TIMER_H__ */
