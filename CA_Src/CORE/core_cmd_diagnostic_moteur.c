//*****************************************************************************/
/// \file           core_cmd_diagnostic_moteur.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-10-13
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
    #include "core_cmd_diagnostic_moteur.h"
#undef public

#define public extern
	#include <stdio.h>
	#include <stdbool.h>
    #include "os_short_timer.h"
    #include "core_spec.h"
    #include "core_motor.h"
    #include "task_main.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
enum{
	SA_TYPE_FIN_REGLAGE 		= 0,
	SA_TYPE_SA0_ADRESSE 		= 1,
	SA_TYPE_SA1_ADRESSE 		= 2,
	SA_TYPE_SA2_ADRESSE 		= 3,
	SA_TYPE_SA3_ADRESSE 		= 4,
	SA_TYPE_NU0 				= 5,
	SA_TYPE_NU1 				= 6,
	SA_TYPE_MOTEUR_GALVANOMETRE	= 7,

	SA_TYPE_COUNT,
};

enum{
	SA_PARAM_ZERO_COURANT 		= 0,
	SA_PARAM_FE_COURANT,
	SA_PARAM_ZERO_TENSION,
	SA_PARAM_FE_TENSION,

	SA_PARAM_COUNT,
};

enum{
	SA_CMDE_FIN_REGLAGE 		= 0,
	SA_CMDE_LECT_TYP 			= 1,
	SA_CMDE_DEBUT_REG 			= 2,
	SA_CMDE_ECR_REG 			= 3,
	SA_CMDE_MEMO 				= 4,
	SA_CMDE_REG_PLUS_MOT 		= 5,
	SA_CMDE_REG_MOINS_MOT 		= 6,
	SA_CMDE_NU0					= 7,

	SA_CMDE_COUNT,
};

#define MASK_SA_TYPE		(0x07)		// 3bits
#define MASK_SA_PARAM		(0x18)		// 2bits
#define MASK_SA_CMD			(0xE0)		// 3bits

typedef union{
	struct{
		uint8_t Type  : 3;	// lsb bits
		uint8_t Param : 2;
		uint8_t Cmd   : 3;
	}bitSa;
	uint8_t u8Byte;
}t_cmd_diag;

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static uint16_t g_u16IncDec = 1;			// Incrément positionnement moteur en 1/10 �
static uint16_t g_u16SaValeurDeReglage;		// Valeur courante de réglage de la Sortie Analogique
											// Consigne de positionnement du moteur en 1/10�

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void voProcessTimeout(void);
static int intCmdReglageMoteurPlusMoins(t_cmd_diag sCmdDiag, uint8_t u8MemoCmdSa, int16_t s16IncDec, int16_t s16ValReglage);
static int intCmdDebutReglage	   (t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa);
static int intCmdReglageMoteurPlus (t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa);
static int intCmdReglageMoteurMoins(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa);
static int intCmdMemorisation      (t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa);
static int intCmdFinReglage        (t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa);

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
/// \date           2021-10-11
///
//----------------------------------------------------------------------------*/
static void voProcessTimeout(void)
{
	g_u16IncDec = 1;
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-13
///
//----------------------------------------------------------------------------*/
static int intCmdDebutReglage(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa)
{
	switch( sCmdDiag.bitSa.Param )
	{
		case SA_PARAM_ZERO_COURANT 	:
		case SA_PARAM_FE_COURANT	:	g_u16SaValeurDeReglage = G_DeviceCfg.sReglageMoteur.u16ZeroDir;
		break;

		case SA_PARAM_ZERO_TENSION	:	g_u16SaValeurDeReglage = G_DeviceCfg.sReglageMoteur.u16ZeroEch;
		break;

		case SA_PARAM_FE_TENSION	:	g_u16SaValeurDeReglage = G_DeviceCfg.sReglageMoteur.u16FinEch;
		break;
	}

	voCoreMotor_SetPositionToMoveToInAngle( g_u16SaValeurDeReglage );
	g_u16IncDec = 1;
	*pu8MemoCmdSa = sCmdDiag.u8Byte & (MASK_SA_TYPE | MASK_SA_PARAM);	// Memo N� SA et Type r�glage(ZeI,FeI,ZeU,FeU)

	return( 0 );	// Response to request
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-13
///
//----------------------------------------------------------------------------*/
static int intCmdReglageMoteurPlusMoins(t_cmd_diag sCmdDiag, uint8_t u8MemoCmdSa, int16_t s16IncDec, int16_t s16ValReglage)
{
	// M�me Cde Moteur et Type réglage(zero_ech,fin_ech) ?
	if( u8MemoCmdSa == (sCmdDiag.u8Byte & (MASK_SA_TYPE | MASK_SA_PARAM)) )
	{
		g_u16SaValeurDeReglage += s16IncDec;
		if( g_u16SaValeurDeReglage >= 3600 )
			g_u16SaValeurDeReglage += s16ValReglage;

		if( g_u16IncDec < 50 )
			g_u16IncDec++;

		intOsShortTimer_Register(500, voProcessTimeout);// Tempo 5 s avant que IncDec = 1
		voCoreMotor_SetPositionToMoveToInAngle( g_u16SaValeurDeReglage );
	}

	return( 1 );	// No response
}

static int intCmdReglageMoteurPlus(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa)
{
	return( intCmdReglageMoteurPlusMoins(sCmdDiag, *pu8MemoCmdSa, g_u16IncDec, (int16_t)(-3600)) );
}

static int intCmdReglageMoteurMoins(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa)
{
	return( intCmdReglageMoteurPlusMoins(sCmdDiag, *pu8MemoCmdSa, (int16_t)(-g_u16IncDec), 3600) );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-13
///
//----------------------------------------------------------------------------*/
static int intCmdMemorisation(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa)
{
	t_reglage_moteur * const psReglageMoteur = &G_DeviceCfg.sReglageMoteur;
	int16_t s16Res;
	int16_t s16Val;
	int Error = 0;

    // M�me N� SA et Type réglage(ZeI,FeI,ZeU,FeU)
	if( *pu8MemoCmdSa != (sCmdDiag.u8Byte & (MASK_SA_TYPE | MASK_SA_PARAM)) )
		return( 1 );

	//
	switch( sCmdDiag.bitSa.Param )
	{
		case SA_PARAM_ZERO_COURANT 	:
		case SA_PARAM_FE_COURANT	:	psReglageMoteur->u16ZeroDir = g_u16SaValeurDeReglage;
		break;

		case SA_PARAM_ZERO_TENSION	:	psReglageMoteur->u16ZeroEch = g_u16SaValeurDeReglage;
		break;

		case SA_PARAM_FE_TENSION	:	psReglageMoteur->u16FinEch  = g_u16SaValeurDeReglage;

										// Ajout pour calage zero a l'opposé du centre de la dynamique
										s16Res = (psReglageMoteur->u16FinEch + psReglageMoteur->u16ZeroEch)/2 - 1800;
										s16Res = u16CoreMotor_CheckAngle( s16Res );

										if( psReglageMoteur->u16FinEch < psReglageMoteur->u16ZeroEch )
										{
											s16Res += 1800;
											s16Res = u16CoreMotor_CheckAngle( s16Res );
										}

										// 0,0° < Calage < 359,9°
										psReglageMoteur->u16Calage += s16Res;
										psReglageMoteur->u16Calage = u16CoreMotor_CheckAngle( (int16_t)psReglageMoteur->u16Calage );

										// 0,0° < zero echelle < 359,9°
										s16Val = psReglageMoteur->u16ZeroEch - s16Res;
										psReglageMoteur->u16ZeroEch = u16CoreMotor_CheckAngle( s16Val );

										// 0,0° < Fin echelle < 359,9°
										s16Val = psReglageMoteur->u16FinEch - s16Res;
										psReglageMoteur->u16FinEch = u16CoreMotor_CheckAngle( s16Val );

										g_u16SaValeurDeReglage = psReglageMoteur->u16FinEch;
		break;

		default 					:	Error = 1;
		break;
	}

	if( Error == 0 )
		CoreSpec_ReglageMoteurWrite(psReglageMoteur);

	return( Error );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-13
///
//----------------------------------------------------------------------------*/
static int intCmdFinReglage(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa)
{
	*pu8MemoCmdSa = 0;		// Retour au mode normal

	return( 0 );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-13
///
//----------------------------------------------------------------------------*/
int intCoreCmdDiagnosticMoteur(uint8_t u8Cmd, uint16_t *pu16Data)
{
	static uint8_t u8MemoCmdSa = 0;
	int (* const pfCmdMoteur[ SA_CMDE_COUNT ])(t_cmd_diag sCmdDiag, uint8_t *pu8MemoCmdSa) = {
		[ SA_CMDE_FIN_REGLAGE   ] = intCmdFinReglage,
		[ SA_CMDE_LECT_TYP      ] = NULL,
		[ SA_CMDE_DEBUT_REG     ] = intCmdDebutReglage,
		[ SA_CMDE_ECR_REG       ] = NULL,
		[ SA_CMDE_MEMO          ] = intCmdMemorisation,
		[ SA_CMDE_REG_PLUS_MOT  ] = intCmdReglageMoteurPlus,
		[ SA_CMDE_REG_MOINS_MOT ] = intCmdReglageMoteurMoins,
		[ SA_CMDE_NU0           ] = NULL,
	};

	int Error = 1;
	t_cmd_diag sCmdDiag;

	sCmdDiag.u8Byte = u8Cmd;

	if( sCmdDiag.bitSa.Type == SA_TYPE_MOTEUR_GALVANOMETRE )
	{
		if( pfCmdMoteur[ sCmdDiag.bitSa.Cmd ] != NULL )
			Error = pfCmdMoteur[ sCmdDiag.bitSa.Cmd ](sCmdDiag, &u8MemoCmdSa);

		if( Error == 0 )
			*pu16Data = g_u16SaValeurDeReglage;
	}

	if( u8MemoCmdSa != 0 )
		G_DeviceError.bit.IfDiagModeMotorSetup = true;
	else
		G_DeviceError.bit.IfDiagModeMotorSetup = false;

	if( Error != 0 )
		Error = -1;

	return( Error );
}
