/******************************************************************************/
/// \file           drv_wdog.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-12-16
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
    #include "drv_wdog.h"
#undef public

#define public extern
    #include "fsl_wdog.h"
    #include "peripherals.h"
#undef public

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]
/// \param[out]
/// \return
///
/// \version        1.0
/// \date           2021-12-16
///                 
/*----------------------------------------------------------------------------*/
void DrvWdog_Enable(void)
{
	WDOG_Type *base = WDOG_PERIPHERAL;
    uint32_t primaskValue = 0U;

    primaskValue = DisableGlobalIRQ();
    WDOG_Unlock(base);
    base->STCTRLH |= WDOG_STCTRLH_WDOGEN( 1 );
    EnableGlobalIRQ(primaskValue);
}

void DrvWdog_Refresh(void)
{
	WDOG_Type *base = WDOG_PERIPHERAL;
	uint32_t u32WindowValue;
	uint32_t u32TimerOutputValue;

	u32WindowValue = (uint32_t)((((uint32_t)base->WINH) << 16U) | (base->WINL));
	u32TimerOutputValue = (uint32_t)((((uint32_t)base->TMROUTH) << 16U) | (base->TMROUTL));

	if( u32TimerOutputValue >= u32WindowValue )
	{
		WDOG_Refresh(base);
	}
}
