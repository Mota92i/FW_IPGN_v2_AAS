//*****************************************************************************/
/// \file           core_app.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2022-01-19
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
//  Included header file list
//*****************************************************************************/
#define public
	#include "core_app.h"
#undef public

#define public extern
	#include <math.h>
	#include <string.h>
	#include <stdbool.h>
    #include "core_spec.h"
	#include "core_voltmetre.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define	ACQUISITION_ERROR_VALUE		( 100000000.0 )

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
#define	ECART						( 240 )				// Nb de uA d'�cart par rapport au 4 mA et 20 mA
#define COURANT_MIN					( 4000 )
#define COURANT_MAX					( 20000 )
#define ECART_MIN_4MA				( COURANT_MIN - ECART )
#define ECART_MAX_20MA				( COURANT_MAX + ECART )

/******************************************************************************/
//  Type declaration
/******************************************************************************/
typedef struct{
	int16_t s16OrigineEchelle;
	int16_t s16FinEchelle;
	int16_t s16DefautSaturationHaute;
	int16_t s16SaturationBasse;
}t_angle_indicateur_def;

typedef void(* t_scale_current)(t_angle_indicateur_def const *psAngleIndicateurDef,
								  uint8_t 						u8Type,
								  float 						f32_Mesure,
								  t_angle 						*psAngle);

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static struct{
	t_angle_indicateur_def const *psAngleIndicateurDef;
	void (* pf_voIndicateurProcessApp)(float f32_Mesure, t_angle *psAngle_Out);
}g_sData;

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void voIndicateurCap           (float f32_Mesure, t_angle *psAngle_Out);
static void voIndicateurBarreAvant    (float f32_Mesure, t_angle *psAngle_Out);
static void voIndicateurBarreDirection(float f32_Mesure, t_angle *psAngle_Out);
static void voIndicateurBarreArriere  (float f32_Mesure, t_angle *psAngle_Out);
static void voIndicateurVitesse		  (float f32_Mesure, t_angle *psAngle_Out);
static void voIndicateur(t_scale_current			  pScaleFunc,
						 t_angle_indicateur_def const *psAngleIndicateurDef,
						 uint8_t 					  u8Type,
		                 float 						  f32_Mesure,
						 t_angle 					  *psAngle_Out);

static void voScaleCurrentInput(t_angle_indicateur_def const *psAngleIndicateurDef,
								uint8_t 					 u8Type,
								float 						 f32_Mesure,
								t_angle 					 *psAngle);
static void voScaleCaAnemoGir(t_angle_indicateur_def const *psAngleIndicateurDef,
							  uint8_t 					   u8Type,
							  float 					   f32_Mesure,
							  t_angle 					   *psAngle);

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
void voCoreApp_Init(void)
{
	void (* const voProcessIndicateurApp[ SPEC_TYPE_COUNT ])(float f32_Mesure, t_angle *psAngle_Out) = {
		[ SPEC_TYPE_CAP   				] = voIndicateurCap,
		[ SPEC_TYPE_BARRE_AVANT      	] = voIndicateurBarreAvant,
		[ SPEC_TYPE_BARRE_DIRECTION     ] = voIndicateurBarreDirection,
		[ SPEC_TYPE_BARRE_ARRIERE       ] = voIndicateurBarreArriere,

		[ SPEC_TYPE_ANEMOGIR_CA_CAP		] = voIndicateurCap,
		[ SPEC_TYPE_ANEMOGIR_CA_VITESSE	] = voIndicateurVitesse,
	};

	g_sData.pf_voIndicateurProcessApp = voProcessIndicateurApp[ G_DeviceCfg.sSpec.u32Type ];
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
static void voIndicateurCap(float f32_Mesure, t_angle *psAngle_Out)
{
	if( f32_Mesure != ACQUISITION_ERROR_VALUE )
	{
	    //--------------------------------------------------------------------------
	    // Compute angle to motor
		// f32_Mesure * 10 au 1/10e
	    //--------------------------------------------------------------------------
		psAngle_Out->f32AngleMotor = (f32_Mesure * 10) + (float)G_DeviceCfg.sReglageMoteur.u16ZeroDir;
		psAngle_Out->s8Valid  	   = ANGLE_VALID;
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
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
static void voIndicateurBarreAvant(float f32_Mesure, t_angle *psAngle_Out)
{
	const t_angle_indicateur_def sAngleIndicateurCfg = {
		.s16OrigineEchelle		  = +220,
		.s16FinEchelle		  	  = -220,
		.s16DefautSaturationHaute = +220 + 30,
		.s16SaturationBasse 	  = -220 - 30,
	};

	voIndicateur(voScaleCurrentInput, &sAngleIndicateurCfg, SPEC_TYPE_BARRE_AVANT, f32_Mesure, psAngle_Out);
}

static void voIndicateurBarreDirection(float f32_Mesure, t_angle *psAngle_Out)
{
	const t_angle_indicateur_def sAngleIndicateurCfg = {
		.s16OrigineEchelle		  = -320,
		.s16FinEchelle		  	  = +320,
		.s16DefautSaturationHaute = +320 + 20,
		.s16SaturationBasse 	  = -320 - 20,
	};

	voIndicateur(voScaleCurrentInput, &sAngleIndicateurCfg, SPEC_TYPE_BARRE_DIRECTION, f32_Mesure, psAngle_Out);
}

static void voIndicateurBarreArriere(float f32_Mesure, t_angle *psAngle_Out)
{
	const t_angle_indicateur_def sAngleIndicateurCfg = {
		.s16OrigineEchelle		  = -220,
		.s16FinEchelle		  	  = +220,
		.s16DefautSaturationHaute = +220 + 30,
		.s16SaturationBasse 	  = -220 - 30,
	};

	voIndicateur(voScaleCurrentInput, &sAngleIndicateurCfg, SPEC_TYPE_BARRE_ARRIERE, f32_Mesure, psAngle_Out);
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
// en proto pour port de marseille
static void voIndicateurVitesse(float f32_Mesure, t_angle *psAngle_Out)
{
	const t_angle_indicateur_def sAngleIndicateurCfg = {
		.s16OrigineEchelle		  = 0, 			// 0   m/s,
		.s16FinEchelle		  	  = 100, 		// 100 m/s,
		.s16DefautSaturationHaute = 100 + 10, 	// 100 + 10 m/s,
		.s16SaturationBasse 	  = 0   - 10, 	// 0   - 10 m/s,
	};

	voIndicateur(voScaleCaAnemoGir, &sAngleIndicateurCfg, SPEC_TYPE_ANEMOGIR_CA_VITESSE, f32_Mesure, psAngle_Out);
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
static void voScaleCurrentInput(t_angle_indicateur_def const *psAngleIndicateurDef,
								uint8_t 					 u8Type,
								float 						 f32_Mesure,
								t_angle 					 *psAngle)
{
	float f32_temp;

	if( f32_Mesure < (float)ECART_MIN_4MA )
	{
		psAngle->f32Angle = (float)psAngleIndicateurDef->s16SaturationBasse;
	}
	else if( f32_Mesure > (float)ECART_MAX_20MA )
	{
		psAngle->f32Angle = (float)psAngleIndicateurDef->s16DefautSaturationHaute;
	}
	else
	{
		psAngle->f32Course  = (f32_Mesure - (float)COURANT_MIN) / (float)(COURANT_MAX - COURANT_MIN);
		f32_temp  = (float)G_DeviceCfg.sSpec.s32Course20 - (float)G_DeviceCfg.sSpec.s32Course4;
		psAngle->f32Course  = (psAngle->f32Course * f32_temp) + (float)G_DeviceCfg.sSpec.s32Course4;

		if( psAngle->f32Course <= (float)G_DeviceCfg.sSpec.u32Rayon )
		{
			f32_temp = psAngle->f32Course / (float)G_DeviceCfg.sSpec.u32Rayon;
			psAngle->f32Angle  = asin( f32_temp ) * (float)572.9578;				// Convert rad -> 1/10 �
			psAngle->f32Angle += (float)G_DeviceCfg.sSpec.s32Offset;

			if( psAngle->f32Angle < (float)psAngleIndicateurDef->s16SaturationBasse )
				psAngle->f32Angle = (float)psAngleIndicateurDef->s16SaturationBasse;
			else if( psAngle->f32Angle > (float)psAngleIndicateurDef->s16DefautSaturationHaute )
				psAngle->f32Angle = (float)psAngleIndicateurDef->s16DefautSaturationHaute;

			if( u8Type == SPEC_TYPE_BARRE_AVANT )
			{
				if( (psAngle->f32Angle >= (float)psAngleIndicateurDef->s16FinEchelle) &&
					(psAngle->f32Angle <= (float)psAngleIndicateurDef->s16OrigineEchelle) )
				{
					psAngle->s8Valid = ANGLE_VALID;
				}
			}
			else
			{
				if( (psAngle->f32Angle >= (float)psAngleIndicateurDef->s16OrigineEchelle) &&
					(psAngle->f32Angle <= (float)psAngleIndicateurDef->s16FinEchelle) )
				{
					psAngle->s8Valid = ANGLE_VALID;
				}
			}
		}
		else
		{
			psAngle->f32Angle = (float)psAngleIndicateurDef->s16SaturationBasse;
		}
	}
}

static void voScaleCaAnemoGir(t_angle_indicateur_def const *psAngleIndicateurDef,
							  uint8_t 					   u8Type,
							  float 					   f32_Mesure,
							  t_angle 					   *psAngle)
{
	psAngle->f32Angle = f32_Mesure;
	if( (psAngle->f32Angle <= (float)psAngleIndicateurDef->s16FinEchelle) &&
		(psAngle->f32Angle >= (float)psAngleIndicateurDef->s16OrigineEchelle) )
	{
		psAngle->s8Valid = ANGLE_VALID;
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
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
static void voIndicateur(t_scale_current			  pScaleFunc,
						 t_angle_indicateur_def const *psAngleIndicateurDef,
						 uint8_t 					  u8Type,
						 float 						  f32_Mesure,
						 t_angle 					  *psAngle_Out)
{
	t_angle sAngle = { .f32Angle  		= 0x8000,
					   .f32Course 		= 0x8000,
					   .f32AngleMotor 	= 0x8000,
					   .s8Valid   		= ANGLE_OVER_UNDERFLOW };
	float f32_temp;

    //--------------------------------------------------------------------------
    // Scale The measure
    //--------------------------------------------------------------------------
	if( f32_Mesure == ACQUISITION_ERROR_VALUE )
	{
		sAngle.f32Angle = (float)psAngleIndicateurDef->s16DefautSaturationHaute;
	}
	else
	{
		if( pScaleFunc != NULL )
			pScaleFunc(psAngleIndicateurDef, u8Type, f32_Mesure, &sAngle);
	}

    //--------------------------------------------------------------------------
    // Compute angle to motor
    //--------------------------------------------------------------------------
	f32_temp = (sAngle.f32Angle - (float)psAngleIndicateurDef->s16OrigineEchelle) / ((float)psAngleIndicateurDef->s16FinEchelle - (float)psAngleIndicateurDef->s16OrigineEchelle);
	if( G_DeviceCfg.sReglageMoteur.u16FinEch > G_DeviceCfg.sReglageMoteur.u16ZeroEch )
		f32_temp = f32_temp * ((float)G_DeviceCfg.sReglageMoteur.u16FinEch - (float)G_DeviceCfg.sReglageMoteur.u16ZeroEch);
	else
		f32_temp = f32_temp * ((float)G_DeviceCfg.sReglageMoteur.u16FinEch - (float)G_DeviceCfg.sReglageMoteur.u16ZeroEch + 3600.0);
	sAngle.f32AngleMotor = f32_temp + (float)G_DeviceCfg.sReglageMoteur.u16ZeroEch;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	memcpy(psAngle_Out, &sAngle, sizeof(sAngle));
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-17
///
//----------------------------------------------------------------------------*/
t_angle sCoreApp_Process(float f32_Mesure)
{
	t_angle sAngle = { .f32Angle  		= 0x8000,
					   .f32Course 		= 0x8000,
					   .f32AngleMotor 	= 0x8000,
					   .s8Valid   		= ANGLE_ERROR };

	g_sData.pf_voIndicateurProcessApp(f32_Mesure, &sAngle);
	if( (sAngle.s8Valid == ANGLE_VALID) || (sAngle.s8Valid == ANGLE_OVER_UNDERFLOW) )
	{
		if( sAngle.f32AngleMotor >= 3600 )
			sAngle.f32AngleMotor -= 3600;
	}

	return(sAngle);
}
