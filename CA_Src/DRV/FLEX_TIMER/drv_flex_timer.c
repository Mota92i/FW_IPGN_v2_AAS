/******************************************************************************/
/// \file           drv_flex_timer.c
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
//  Included header file list
/******************************************************************************/
#define public
    #include "drv_flex_timer.h"
#undef public

#define public extern
	#include "fsl_ftm.h"
	#include "peripherals.h"
#undef public

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voDrvFlexTimer2_Init(void);

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
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
void voDrvFlexTimer_Init(void)
{
	//FTM0_PERIPHERAL->SC &= ~0x80;
	FTM0_PERIPHERAL->MOD = 0;
	//FTM0_PERIPHERAL->CONTROLS[5].CnSC = 0x84;
	//FTM0_PERIPHERAL->SYNC &= ~FTM_SYNC_SYNCHOM_MASK;
	FTM0_PERIPHERAL->MODE &= ~FTM_MODE_FTMEN_MASK;
	//FTM0_PERIPHERAL->SYNCONF &= ~(FTM_SYNCONF_SYNCMODE_MASK | FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_CNTINC_MASK);
	FTM0_PERIPHERAL->SYNCONF &= ~FTM_SYNCONF_SYNCMODE_MASK; // Legacy synchronisation
	//FTM0_PERIPHERAL->COMBINE &= ~0x20202020;

#if 0
	ftm_config_t sFtmInfo;

	FTM_GetDefaultConfig(&sFtmInfo);
	sFtmInfo.prescale = kFTM_Prescale_Divide_16;

	// Initialize FTM module
	FTM_Init(FTM0_PERIPHERAL, &sFtmInfo);
    // Setup input capture on channel 5 on falling edge with no filter
	FTM_SetupInputCapture( FTM0_PERIPHERAL, kFTM_Chnl_5, kFTM_FallingEdge, 0 );

	(void)(FTM0_PERIPHERAL->SC == 0U);          // Dummy read of the FTM0_PERIPHERAL->SC register to clear the interrupt flag
	FTM0_PERIPHERAL->SC &= ~((uint32_t)0x18);          		// CLKS=0  Stop the counter
	(void)(FTM0_PERIPHERAL->CONTROLS[5].CnSC == 0U);        // Dummy read of the FTM0_PERIPHERAL->C5SC register to clear the interrupt flag
	FTM0_PERIPHERAL->SC   &= ~((uint32_t)0xA0);				// TOF=0,CPWMS=0
	FTM0_PERIPHERAL->MODE |= (uint32_t)0x04UL;				// WPDIS=1 Disable write protection
	FTM0_PERIPHERAL->MODE &= (uint32_t)~0x01UL;			// FTMEN=0
	// FAULTEN3=0,SYNCEN3=0,DTEN3=0,COMP3=0,FAULTEN2=0,SYNCEN2=0,DTEN2=0,DECAPEN2=0,COMP2=0,COMBINE2=0,FAULTEN1=0,SYNCEN1=0,DTEN1=0,COMP1=0,FAULTEN0=0,SYNCEN0=0,DTEN0=0,COMP0=0
	FTM0_PERIPHERAL->COMBINE &= (uint32_t)~0x72777272UL;
	FTM0_PERIPHERAL->INVCTRL &= (uint32_t)~0x0FUL;			// INV3EN=0,INV2EN=0,INV1EN=0,INV0EN=0
	FTM0_PERIPHERAL->CONTROLS[5].CnSC = (uint32_t)0x04UL;				// CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=1,DMA=0
	FTM0_PERIPHERAL->CONTROLS[5].CnV &= (uint32_t)~0xFFFF0000UL;
	FTM0_PERIPHERAL->OUTINIT &= (uint32_t)~0xFFFFFF00UL;
	FTM0_PERIPHERAL->FLTCTRL &= (uint32_t)~0x0F00UL;		// FFVAL=0
	FTM0_PERIPHERAL->FLTPOL &= (uint32_t)~0x0FUL;			// FLT3POL=0,FLT2POL=0,FLT1POL=0,FLT0POL=0
	// FFLTR3EN=0,FFLTR2EN=0,FFLTR1EN=0,FFLTR0EN=0,FAULT3EN=0,FAULT2EN=0,FAULT1EN=0,FAULT0EN=0
	FTM0_PERIPHERAL->FLTCTRL &= (uint32_t)~0xFFUL;
	FTM0_PERIPHERAL->SYNC &= (uint32_t)~0xFFUL;			// SWSYNC=0,TRIG2=0,TRIG1=0,TRIG0=0,SYNCHOM=0,REINIT=0,CNTMAX=0,CNTMIN=0
	FTM0_PERIPHERAL->EXTTRIG &= (uint32_t)~0x7FUL;			// INITTRIGEN=0,CH1TRIG=0,CH0TRIG=0,CH5TRIG=0,CH4TRIG=0,CH3TRIG=0,CH2TRIG=0
	FTM0_PERIPHERAL->MOD &= (uint32_t)~0xFFFFUL;			// MOD=0
	FTM0_PERIPHERAL->CNTIN &= (uint32_t)~0xFFFFUL;			// INIT=0
	FTM0_PERIPHERAL->DEADTIME &= (uint32_t)~0xFFUL;		// DTPS=0,DTVAL=0
	FTM0_PERIPHERAL->CNT &= (uint32_t)~0xFFFFUL;			// COUNT=0
	FTM0_PERIPHERAL->MODE &= (uint32_t)~0xFFFFFFEBUL;		// FAULTIE=0,FAULTM=0,PWMSYNC=0,INIT=0,FTMEN=0
	FTM0_PERIPHERAL->SYNCONF &= (uint32_t)~0xFFE0E0FFUL;	// SYNCMODE=0,SWOC=0,INVC=0,CNTINC=0,HWTRIGMODE=0
	// TOF=0,TOIE=0,CPWMS=0,CLKS=1,PS=5
//	FTM0_PERIPHERAL->SC = (uint32_t)((FTM0_PERIPHERAL->SC & (uint32_t)~0xF2UL) | (uint32_t)0x0DUL);		// Clock = clock / 32 = 3125000 Hz
	FTM0_PERIPHERAL->SC |= 0x08;							//System clock
	FTM0_PERIPHERAL->FMS &= (uint32_t)~0xCFUL;				// FAULTF=0,WPEN=0,FAULTF3=0,FAULTF2=0,FAULTF1=0,FAULTF0=0
	FTM0_PERIPHERAL->SWOCTRL &= (uint32_t)~0x20UL;			// CH5OC=0
	FTM0_PERIPHERAL->CONF &= (uint32_t)~0x06DFUL;			// GTBEOUT=0,GTBEEN=0,BDMMODE=0,NUMTOF=0
	// LDOK=0,CH7SEL=0,CH6SEL=0,CH5SEL=0,CH4SEL=0,CH3SEL=0,CH2SEL=0,CH1SEL=0,CH0SEL=0
	FTM0_PERIPHERAL->PWMLOAD &= (uint32_t)~0x02FFUL;
#endif

	voDrvFlexTimer2_Init();
}

static void voDrvFlexTimer2_Init(void)
{
//	FTM2_PERIPHERAL->SYNC &= ~FTM_SYNC_SYNCHOM_MASK;
	FTM2_PERIPHERAL->MODE &= ~FTM_MODE_FTMEN_MASK;
	//FTM2_PERIPHERAL->SYNCONF &= ~(FTM_SYNCONF_SYNCMODE_MASK | FTM_SYNCONF_SWOC_MASK | FTM_SYNCONF_INVC_MASK | FTM_SYNCONF_CNTINC_MASK);
	FTM2_PERIPHERAL->SYNCONF &= ~FTM_SYNCONF_SYNCMODE_MASK; // Legacy synchronisation
//	FTM2_PERIPHERAL->COMBINE &= ~0x20;


#if 0
	 /* ### Init_FTM2 init code (PWM) */
 (void)(FTM2_PERIPHERAL->SC == 0U);               		/* Dummy read of the FTM2_SC register to clear the interrupt flag */
 FTM2_PERIPHERAL->SC = (uint32_t)0x00UL;          		/* TOF=0,TOIE=0,CPWMS=0,CLKS=0,PS=0  Stop the counter */
 (void)(FTM2_PERIPHERAL->CONTROLS[0].CnSC == 0U);             		/* Dummy read of the FTM2_C0SC register to clear the interrupt flag */
 (void)(FTM2_PERIPHERAL->CONTROLS[1].CnSC == 0U);             		/* Dummy read of the FTM2_C1SC register to clear the interrupt flag */
 FTM2_PERIPHERAL->CONTROLS[0].CnSC = (uint32_t)0x00UL; 				/* CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,DMA=0  for CH0 */
 FTM2_PERIPHERAL->CONTROLS[1].CnSC = (uint32_t)0x00UL;				/* CHF=0,CHIE=0,MSB=0,MSA=0,ELSB=0,ELSA=0,DMA=0  for CH1 */
 FTM2_PERIPHERAL->SC &= (uint32_t)~0xA0UL;				/* TOF=0,CPWMS=0 */
 FTM2_PERIPHERAL->MODE |= (uint32_t)0x04UL;       		/* WPDIS=1  Disable write protection */
 FTM2_PERIPHERAL->MODE &= (uint32_t)~0x01UL;			/* FTMEN=0 */
 FTM2_PERIPHERAL->INVCTRL &= (uint32_t)~0x01UL;			/* INV0EN=0 */
 FTM2_PERIPHERAL->COMBINE &= (uint32_t)~0x77UL;			/* FAULTEN0=0,SYNCEN0=0,DTEN0=0,DECAPEN0=0,COMP0=0,COMBINE0=0 */
 /* CH0 :  CHF=0,CHIE=1,MSB=1,ELSB=0,ELSA=1,DMA=0 */
 FTM2_PERIPHERAL->CONTROLS[0].CnSC = (uint32_t)((FTM2_PERIPHERAL->CONTROLS[0].CnSC & (uint32_t)~0xFFFFFF8BUL) | (uint32_t)0x24UL);
 /* CH1 :  CHF=0,CHIE=0,MSB=1,ELSB=0,ELSA=1,DMA=0 */
 FTM2_PERIPHERAL->CONTROLS[1].CnSC = (uint32_t)((FTM2_PERIPHERAL->CONTROLS[1].CnSC & (uint32_t)~0xFFFFFFCBUL) | (uint32_t)0x24UL);
 FTM2_PERIPHERAL->CONTROLS[0].CnV = 15000; //(uint32_t)25000UL;				/* 50% car modulo 50000 */
 FTM2_PERIPHERAL->CONTROLS[1].CnV = 30000 - 2; //(uint32_t)49998L;				/* Lum = 0 */
 FTM2_PERIPHERAL->FILTER &= (uint32_t)~0x0FUL;			/* CH0FVAL=0 */
 FTM2_PERIPHERAL->OUTINIT &= (uint32_t)~0xFFFFFF00UL;	/* OUTINIT:  */
 FTM2_PERIPHERAL->FLTCTRL &= (uint32_t)~0x0F00UL;		/* FFVAL=0 */
 FTM2_PERIPHERAL->FLTPOL &= (uint32_t)~0x07UL;			/* FLT2POL=0,FLT1POL=0,FLT0POL=0 */
 FTM2_PERIPHERAL->POL = (uint32_t)0x03UL;				/* POL0=1, POL1=1 */
 /* FTM2_FLTCTRL: FFLTR2EN=0,FFLTR1EN=0,FFLTR0EN=0,FAULT2EN=0,FAULT1EN=0,FAULT0EN=0 */
 FTM2_PERIPHERAL->FLTCTRL &= (uint32_t)~0x77UL;
 /* FTM2_SYNC: SWSYNC=0,TRIG2=0,TRIG1=0,TRIG0=0,SYNCHOM=0,REINIT=0,CNTMAX=0,CNTMIN=0 */
 FTM2_PERIPHERAL->SYNC &= (uint32_t)~0xFFUL;
 FTM2_PERIPHERAL->EXTTRIG &= (uint32_t)~0x70UL;			/* INITTRIGEN=0,CH1TRIG=0,CH0TRIG=0 */
 FTM2_PERIPHERAL->MOD &= (uint32_t)~0xFFFFUL;			/* MOD=0 */
 FTM2_PERIPHERAL->MOD |= 30000 - 1; //(uint32_t)49999UL;				/* MOD=50000 */
 FTM2_PERIPHERAL->CNTIN &= (uint32_t)~0xFFFFUL;			/* INIT=0 */
 FTM2_PERIPHERAL->DEADTIME &= (uint32_t)~0xFFUL;		/* DTPS=0,DTVAL=0 */
 FTM2_PERIPHERAL->CNT &= (uint32_t)~0xFFFFUL;			/* COUNT=0 */
 FTM2_PERIPHERAL->MODE &= (uint32_t)~0xFFFFFFEBUL;		/* FAULTIE=0,FAULTM=0,PWMSYNC=0,INIT=0,FTMEN=0 */
 FTM2_PERIPHERAL->SYNCONF &= (uint32_t)~0xFFE0E0FFUL;	/* SYNCMODE=0,SWOC=0,INVC=0,CNTINC=0,HWTRIGMODE=0 */
 FTM2_PERIPHERAL->SC = (uint32_t)((FTM2_PERIPHERAL->SC & (uint32_t)~0xB7UL) | (uint32_t)0x48UL);	/* TOF=0,TOIE=1,CPWMS=0,CLKS=1,PS=0 */
 FTM2_PERIPHERAL->FMS &= (uint32_t)~0xCFUL;				/* FAULTF=0,WPEN=0,FAULTF3=0,FAULTF2=0,FAULTF1=0,FAULTF0=0 */
 /* FTM2_QDCTRL: PHAFLTREN=0,PHBFLTREN=0,PHAPOL=0,PHBPOL=0,QUADMODE=0,QUADIR=0,TOFDIR=0,QUADEN=0 */
 FTM2_PERIPHERAL->QDCTRL = (uint32_t)0x00UL;			/* CH0OC=0 */
 FTM2_PERIPHERAL->SWOCTRL &= (uint32_t)~0x01UL;
 FTM2_PERIPHERAL->CONF &= (uint32_t)~0x06DFUL;			/* GTBEOUT=0,GTBEEN=0,BDMMODE=0,NUMTOF=0 */
 FTM2_PERIPHERAL->PWMLOAD &= (uint32_t)~0x0203UL;		/* LDOK=0,CH1SEL=0,CH0SEL=0 */

 FTM_EnableInterrupts(FTM2_PERIPHERAL, kFTM_TimeOverflowInterruptEnable);
   /* Interrupt vector FTM2_IRQn priority settings in the NVIC. */
   NVIC_SetPriority(FTM2_IRQN, FTM2_IRQ_PRIORITY);
   /* Enable interrupt FTM2_IRQn request in the NVIC. */
   EnableIRQ(FTM2_IRQN);
   FTM_StartTimer(FTM2_PERIPHERAL, kFTM_SystemClock);
#endif
}
//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
uint32_t u32DrvFlexTimer_GetTimerCaptureValue(uint8_t *pu8CaptureEventFlag)
{
	uint32_t u32Val;

	u32Val = FTM_GetInputCaptureValue(FTM0_PERIPHERAL, kFTM_Chnl_5);	// Lecture de la valeur capture

	if( pu8CaptureEventFlag != NULL )
	{
		if( (FTM_GetStatusFlags(FTM0_PERIPHERAL) & kFTM_Chnl5Flag) == 0 )
			*pu8CaptureEventFlag = false;
		else
			*pu8CaptureEventFlag = true;
	}

	FTM_ClearStatusFlags(FTM0_PERIPHERAL, kFTM_Chnl5Flag); // Capture Flag on FTM0 ch5 = 0

	return( u32Val );
}
