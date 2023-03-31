/******************************************************************************/
/// \file           drv_pwm.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-09-27
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
    #include "drv_pwm.h"
#undef public

#define public extern
	#include "fsl_ftm.h"
	#include "peripherals.h"
#undef public

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
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
void voDrvPwm_SetLedBrightness(uint32_t u32CptDutyCycle)
{
	// Update counter value
    FTM2_PERIPHERAL->CONTROLS[kFTM_Chnl_1].CnV = u32CptDutyCycle;

    // Software trigger to update registers
    FTM_SetSoftwareTrigger(FTM2_PERIPHERAL, true);
}

