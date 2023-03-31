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
//  Included header file list
//*****************************************************************************/
#define public
    #include "core_motor.h"
#undef public

#define public extern
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
    #include "core_spec.h"
    #include "drv_stepping_motor.h"
    #include "drv_mlx90316.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define MAX_MOMENTS_MOTEUR		( 8 )

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static t_reglage_moteur const * const g_psReglageMoteur = &G_DeviceCfg.sReglageMoteur;
static t_reglage_moteur g_sReglageMoteur_Step;

static t_motor_position g_sSensorPosition;
static t_motor_position g_sPositionToMoveTo;

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static int16_t	NbPas = 0;
static uint8_t 	Cpt_Fin;
static struct {
	uint8_t bRecal : 1;
}g_sFlag = {0};

static uint8_t	Flag_Recal;					// Flag autorisation recalage

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static int16_t s16CalculEcart(uint16_t u16CurrentPos_Step, t_motor_position *psPosToMoveTo);

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
static int16_t s16CalculEcart(uint16_t u16CurrentPos_Step, t_motor_position *psPosToMoveTo)
{
	uint8_t  u8ModeDeplacementAiguille  = G_DeviceCfg.psAppDeviceCfg->u8ModeDeplacementAiguille;
	int16_t  s16PosError_Step;

	// -------------------------------------------------------------------------
	// Calcul de l'ecart instantanne en pas
	// -------------------------------------------------------------------------
	s16PosError_Step = psPosToMoveTo->u16Step - u16CurrentPos_Step;

	if( s16PosError_Step != 0 )
	{
		// Deplacement mode "INDICATEUR DE DIRECTION"
		if( u8ModeDeplacementAiguille == MODE_DEPLACEMENT_AIGUILLE_INDICATEUR_DIRECTION )
		{
			// -400 <= deplacement <= +400
			if( s16PosError_Step < -(MOTOR_MAX_STEP / 2) )
				s16PosError_Step += MOTOR_MAX_STEP;
			if( s16PosError_Step > +(MOTOR_MAX_STEP / 2) )
				s16PosError_Step -= MOTOR_MAX_STEP;
		}
		else // Deplacement mode "GALVANOMETRE"
		{
			// Cas ou le calage est a l'interieur du zero et de la fin d'echelle x
			if( g_psReglageMoteur->u16FinEch <= g_psReglageMoteur->u16ZeroEch )
			{
				if( ((u16CurrentPos_Step - g_sReglageMoteur_Step.u16ZeroEch) <= -1) ||
					(psPosToMoveTo->u16Angle > g_psReglageMoteur->u16FinEch) )
				{
					if( ((u16CurrentPos_Step - g_sReglageMoteur_Step.u16FinEch) < 1) &&
						(psPosToMoveTo->u16Angle >= g_psReglageMoteur->u16ZeroEch) )
					{
						s16PosError_Step -= MOTOR_MAX_STEP;
					}
				}
				else
				{
					s16PosError_Step += MOTOR_MAX_STEP;
				}
			}
		}
	}

	return( s16PosError_Step );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
void voCoreMotor_Init(void)
{
	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	g_sSensorPosition.u16Angle   = 0;
	g_sSensorPosition.u16Step    = 0;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	voCoreMotor_SetPositionToMoveToInAngle( 0 );
	while(NbPas != 0);
	voCoreMotor_SetPositionToMoveToInAngle( 45 );	// 100 pas pour synchroniser le moteur (0.45 degre par pas)
	while(NbPas == 0);
	while(NbPas != 0);
	voCoreMotor_SetPositionToMoveToInAngle( G_DeviceCfg.sReglageMoteur.u16ZeroDir );
	while(NbPas == 0);
	while(NbPas != 0);

	Flag_Recal = 0;
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
void voCoreMotor_Process(void)
{
	const  uint8_t  TAB_TEMPO_TEMPS[ ]  = {21, 20, 12, 8, 7, 6, 5, 4, 3, 2, 1};
	const  uint8_t  TEMPO_TEMPS_IDX_MAX = (sizeof(TAB_TEMPO_TEMPS)/sizeof(TAB_TEMPO_TEMPS[0])) - 1;
	const  uint8_t  MAX_TEMPO_TEMPS_IDX = 0;
	static uint8_t 	u8MotorPhase 		= 0;
	static uint8_t  u8TempoPhase 		= 0;
	static uint8_t  u8NbpRampe   		= 0;
	static uint8_t  u8CptDemar   		= 0;
	static uint16_t	u16MemoPosition 	= 0;
	static uint8_t 	u8InitStart			= false;

	// -------------------------------------------------------------------------
	// Temporisation pour changement de phases du moteur
	// -------------------------------------------------------------------------
	if( u8TempoPhase > 0 )
	{
		u8TempoPhase--;
		return;
	}

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	NbPas = s16CalculEcart(u16MemoPosition, &g_sPositionToMoveTo);	// En nb de pas (0 � 799)
	if( NbPas == 0 )	// Mesure = Consigne => arret
	{
		u8InitStart = true;

		if( g_sFlag.bRecal == 1 )
		{
			int16_t  s16PosError_Step = g_sPositionToMoveTo.u16Step - g_sSensorPosition.u16Step;

			// -(MOTOR_MAX_STEP / 2) < s16PositionErrorInStep < +(MOTOR_MAX_STEP / 2)
			if( s16PosError_Step < -(MOTOR_MAX_STEP / 2) )
				s16PosError_Step += MOTOR_MAX_STEP;
			if( s16PosError_Step > +(MOTOR_MAX_STEP / 2) )
				s16PosError_Step -= MOTOR_MAX_STEP;
			if( (s16PosError_Step < -1) || (s16PosError_Step > 1) )			// Doit on bouger d'au moins 1 pas ?
			{
				u16MemoPosition = g_sSensorPosition.u16Step;
			}

			g_sFlag.bRecal = 0 ;
		}
		else
		{
			if( Cpt_Fin > 0 )
				Cpt_Fin--;
		}
	}
	else	// On doit deplacer l'aiguille d'au moins 1 pas
	{
		// -------------------------------------------------------------------------
		//
		// -------------------------------------------------------------------------
		if( u8InitStart == true )
		{
			// Nb de pas rampe = Nb pas/2
			uint16_t u16AbsErrorStep = (uint16_t)abs( (int)(NbPas / 2) );

			// 0 <= NbpRampe <= 10
			if( u16AbsErrorStep > TEMPO_TEMPS_IDX_MAX )
				u8NbpRampe = TEMPO_TEMPS_IDX_MAX;
			else
				u8NbpRampe = (uint8_t)u16AbsErrorStep;

			u8CptDemar  = 0;
			Cpt_Fin     = 3 * TAB_TEMPO_TEMPS[ MAX_TEMPO_TEMPS_IDX ];
			u8InitStart	= false;						// Reprise de la commande du moteur
		}

		// -------------------------------------------------------------------------
		//
		// -------------------------------------------------------------------------
		uint8_t u8MotorPhaseMemo = u8MotorPhase;

		if( NbPas > 0 )
		{
			if( ++u16MemoPosition >= MOTOR_MAX_STEP )
				u16MemoPosition = 0;

			if( --u8MotorPhase >= MAX_MOMENTS_MOTEUR )
				u8MotorPhase = MAX_MOMENTS_MOTEUR - 1;
		}
		else
		{
			if( --u16MemoPosition >= MOTOR_MAX_STEP )
				u16MemoPosition = MOTOR_MAX_STEP - 1;

			if( ++u8MotorPhase >= MAX_MOMENTS_MOTEUR )
				u8MotorPhase = 0;
		}

		if( u8MotorPhaseMemo != u8MotorPhase )
			voDrvSteppingMotor_SetPhase(u8MotorPhase);

		/* -------------------------------------------------------------------------
		        ________________________
		       /                        \        Rampe d'accélération
		      /                          \       et de décélération
		     /                            \      pour piloter le moteur.
		    /                              \
		  ------------------------------------------------------------------------- */

		uint16_t u16Idx;

		if( u8CptDemar <= u8NbpRampe )
		{
			u16Idx = u8CptDemar; 		// Rampe d'accélération
			u8CptDemar++;
		}
		else
		{
			u16Idx = (uint16_t)abs( (int)NbPas );
			if( u16Idx > u8NbpRampe )	// Plateau sinon Rampe décélération
				u16Idx = u8NbpRampe;	// Plateau

		}
		u8TempoPhase = TAB_TEMPO_TEMPS[ u16Idx ];
	}
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
t_motor_position sCoreMotor_ProcessRecalage(uint8_t u8ModeDiag)
{
	int16_t s16Val;

	// -------------------------------------------------------------------------
	// Get the current position from sensor and process if it is only consistent
	// -------------------------------------------------------------------------
	s16Val = (int16_t)s32DrvMlx90316_ReadAngle();
	if( s16Val >= 0 )
	{
		// ---------------------------------------------------------------------
		// Correct sensor angle by the factory value
		// ---------------------------------------------------------------------
		g_sSensorPosition.u16Angle = u16CoreMotor_CheckAngle( (int16_t)(s16Val - G_DeviceCfg.sReglageMoteur.u16Calage) );
		g_sSensorPosition.u16Step  = u16DrvSteppingMotor_AngleToStep(g_sSensorPosition.u16Angle);

		// ---------------------------------------------------------------------
		//
		// ---------------------------------------------------------------------
		if( ( (u8ModeDiag == 0) && (Flag_Recal == 0) ) ||
			( (u8ModeDiag == 1) && (Cpt_Fin == 0) ) )
		{
			Flag_Recal 	   = 1;
			g_sFlag.bRecal = 1;
		}
	}
	else
	{
		g_sSensorPosition.u16Angle = 0x8000;
		g_sSensorPosition.u16Step  = 0x8000;
	}

	return( g_sSensorPosition );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
void voCoreMotor_SetPositionToMoveToInAngle(uint16_t u16PosToMoveTo_Angle)
{
	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	g_sReglageMoteur_Step.u16Calage  = u16DrvSteppingMotor_AngleToStep(g_psReglageMoteur->u16Calage);
	g_sReglageMoteur_Step.u16FinEch  = u16DrvSteppingMotor_AngleToStep(g_psReglageMoteur->u16FinEch);
	g_sReglageMoteur_Step.u16ZeroDir = u16DrvSteppingMotor_AngleToStep(g_psReglageMoteur->u16ZeroDir);
	g_sReglageMoteur_Step.u16ZeroEch = u16DrvSteppingMotor_AngleToStep(g_psReglageMoteur->u16ZeroEch);

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
	if( u16PosToMoveTo_Angle != 0x8000 )
	{
		if( u16PosToMoveTo_Angle >= 3600 )
			u16PosToMoveTo_Angle = 0;

		g_sPositionToMoveTo.u16Angle = u16PosToMoveTo_Angle;
		g_sPositionToMoveTo.u16Step  = u16DrvSteppingMotor_AngleToStep(g_sPositionToMoveTo.u16Angle);

		// ---------------------------------------------------------------------
		//
		// ---------------------------------------------------------------------
		Flag_Recal = 0;
	}
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
uint16_t u16CoreMotor_CheckAngle(int16_t s16Angle)
{
	if( s16Angle >= 3600 )
		s16Angle -= 3600;
	else if( s16Angle < 0 )
		s16Angle += 3600;

	return( (uint16_t)s16Angle );
}
