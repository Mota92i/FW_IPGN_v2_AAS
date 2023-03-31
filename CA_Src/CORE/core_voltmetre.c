//*****************************************************************************/
/// \file           core_voltmetre.c
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
//  Included header file list
//*****************************************************************************/
#define public
	#include "core_voltmetre.h"
#undef public

#define public extern
	#include <string.h>
	#include <stdbool.h>
    #include "drv_board_gpio.h"
    #include "drv_hdw_timer.h"
    #include "drv_flex_timer.h"
    #include "os_msg_queue.h"
    #include "core_spec.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
enum{
	ACQUISITION_MESURE_READY 	=  1,
	ACQUISITION_MESURE_IDLE 	=  0,
	ACQUISITION_MESURE_OVERFLOW = -1,
	ACQUISITION_MESURE_ERROR 	= -2,
}t_acquisition_mesure;

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define TICK_IT_US						( PERIOD_TIMER_IT )
#define FREQUENCE_TIMER_MESURE_HZ		( 3750000 )

#define TEMPS_INTEGRATION_MS			( 100 )
#define TEMPS_INTEGRATION_US			( TEMPS_INTEGRATION_MS * 1000 )
#define TEMPS_INTEGRATION_TICK			(TEMPS_INTEGRATION_US / TICK_IT_US)  // Temps d'int�gration de la mesure en nb de coups d'horloge (Time out 2.5 KHz)

//#define TEMPS_INTEGRATION_MESURE_TICK	((uint64_t)(TEMPS_INTEGRATION_US * FREQUENCE_TIMER_MESURE_HZ / 1000000))	 // Temps d'int�gration de la mesure en nb de coups d'horloge (Timer 2.5 MHz)
#define TEMPS_INTEGRATION_MESURE_TICK	( (uint32_t)(FREQUENCE_TIMER_MESURE_HZ / 10) )

#define N_MIN							(TEMPS_INTEGRATION_TICK * 3 / 50)		// Nb min de comptage NVR pour ne pas etre en saturation
#define N_MAX							(TEMPS_INTEGRATION_TICK * 47 / 50)		// Nb max de comptage NVR pour ne pas etre en saturation

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define MONTE_INTEGRATEUR		( 0 )

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define VOIE_DE_MESURE_COUNT	( 1 )		// Une voie de mesure

#define NB_MESURE_PAR_CYCLE		( 20 )      // Nombre de mesure par cycle (1 autozero + NbVoieCycle mesures)
#define	NB_ACQ_ETAL_MAX			( 100 )     // Nb Max d'acquisitions � moyenner pour le calcul du coefficient d'�talonnage
#define	SEUIL_FILT_ZERO  		( 60 )      // Ecart max entre Zero filt et Zero instan pour acceptation
#define	CPT_CONF_ZERO_MAX  		( 3 )       // Nb de confirmation Zero > Zeromoy avant acceptation

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define NBP_I_THEO				( 20000 )
#define NBP_I_MAX	   			( 22000 )

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define MESURE_EN_DEFAUT		((float)(100000000.0))

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
static uint8_t	 g_u8StabilisationMesure = false;
static t_measure g_sMeasure;

typedef enum{
	ETALONNAGE_STATUS_ERROR			= -1,
	ETALONNAGE_STATUS_SUCCESS   	= 0,
	ETALONNAGE_STATUS_PROCESSING	= 1,
	ETALONNAGE_STATUS_IDLE			= 2,
}t_etalonnage_status;

typedef enum{
	CMD_ETALONNAGE_4_20MA    		= 2,
}t_etalonnage_cmd;

static struct{
	t_etalonnage_status		 eStatus;
	uint16_t	 			 u16Voie;
	uint8_t	 				 u8Cmd;
}g_Etalonnage = {0xFF, 0, 0};

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
#define PILE_MESURES
#ifdef PILE_MESURES
	#define MAX_PILE		( 20 )

	static struct
	{
		uint8_t u8Idx;

		struct{
			uint8_t  u8Voie;
			uint8_t  N;
			uint16_t t2;
			uint32_t temps;
			float    C;
			float    R;
			float    U;
			float    f32_Zero;
		}sVal[ MAX_PILE ];
	}g_debug;
#endif

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static float f32_VOLT_Calcul_U(t_voltmetre_mesure *psVoltmetre);
static void VOLT_Etalonnage(uint8_t u8Voie, int32_t r);

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
// Sous programme d'etalonnage
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-11
///
//----------------------------------------------------------------------------*/
static void VOLT_Etalonnage(uint8_t u8Voie, int32_t r)
{
	//const int32_t   Rmin[] = { 864270, -50000, 765000, 1440000};     // { 0,1/1/10V/20mA  ,  RSP100 , RSP137 , RSP230 }
	//const int32_t   Rmax[] = {1056330, +50000, 935000, 1760000};
	static struct{
		int32_t  Rmoy;
		uint16_t NbAcqEtal;
	}sVal[ VOLTMETRE_VOIE_COUNT ];
	static enum {
		SM_INIT = 0,
		SM_ETALONNAGE,
	}u8StateMachine = SM_INIT;
	int32_t i;
	uint8_t u8Idx;

	if( u8StateMachine == SM_INIT)
	{
		for(i = 0; i < VOLTMETRE_VOIE_COUNT; i++)
		{
			sVal[ i ].NbAcqEtal = NB_ACQ_ETAL_MAX;
			sVal[ i ].Rmoy      = 0;
		}
		u8StateMachine = SM_ETALONNAGE;
	}
	else
	{
		if( (g_Etalonnage.u16Voie >> (u8Voie - 1)) & 0x0001 )			// Voie calcul�e en �talonnage ?
		{
			if( g_Etalonnage.u8Cmd < 3 )
				u8Idx = 0;
			else
				u8Idx = g_Etalonnage.u8Cmd - 2;

//todo				if( (r >= Rmin[ u8Idx ]) && (r <= Rmax[ u8Idx ]) )
			{
				sVal[u8Voie - 1].Rmoy += r;
				if( --sVal[u8Voie - 1].NbAcqEtal == 0 )               				// Fin de l'etal pour cette voie ?
				{
					sVal[u8Voie - 1].Rmoy /= NB_ACQ_ETAL_MAX;

					if( g_Etalonnage.u8Cmd == CMD_ETALONNAGE_4_20MA )                    			// Etal courant ?
					{
						G_DeviceCfg.sEtalonnageVoieMesure.f32Cal_Current = (float)NBP_I_THEO / sVal[u8Voie - 1].Rmoy; // Oui : calcul du coeff pour cette voie (20000 Pts en courant)
						g_Etalonnage.u16Voie = g_Etalonnage.u16Voie ^ ( 0x0001 << (u8Voie - 1) );           // Bit d'�talonnage voie = 0
						if( g_Etalonnage.u16Voie == 0 )                      // Derniere voie a etalonner ?
						{
							CoreSpec_EtalonnageVoieDeMesureWrite(&G_DeviceCfg.sEtalonnageVoieMesure);

							u8StateMachine    	 = SM_INIT;           // Etalonnage termin� !
							g_Etalonnage.eStatus = ETALONNAGE_STATUS_SUCCESS;                         	// Etalonnage r�ussi !
						}
					}
				}
			}
/*				else
			{
				u8StateMachine    	  = SM_INIT;	// Niveau d'entr�e incorrect
				g_Etalonnage.eStatus = ETALONNAGE_STATUS_ERROR;
			}*/
		}
	}
}


//----------------------------------------------------------------------------*/
// Sous programme de calcul de U
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2021-10-11
///
//----------------------------------------------------------------------------*/
static float f32_VOLT_Calcul_U(t_voltmetre_mesure *psVoltmetre)
{
	static uint8_t 	u8CptStabilisationMesure = 50;  // Compteur de stabilisation mesure
	static uint8_t 	u8FiltrageCptMesureValide = 0;	// Compteur de confirmation de saut sur Zero (pour chaque calibre)
	static float    f32_Zero = 0;					// Tableau des valeurs des zeros pour chaque calibre
	float  			f32_temp;
	float 			C = 0;							// Case de calcul de U
	float 			R = 0;                			// Case de calcul de U
	float 			U = MESURE_EN_DEFAUT;			// Acquisition en  défaut

	if( psVoltmetre->s8Status == ACQUISITION_MESURE_READY )
	{
        //----------------------------------------------------------------------
        // C = 2^32 * N / temps
		// R Calcul du Nb de Pts
        //----------------------------------------------------------------------
		C = (float)(4294967296.0 * psVoltmetre->s16N) / psVoltmetre->u32temps;
		R = (float)((f32_Zero - C) * psVoltmetre->u32temps) / TEMPS_INTEGRATION_MESURE_TICK;

        //----------------------------------------------------------------------
        //
        //----------------------------------------------------------------------
		if( psVoltmetre->u8Voie == VOLTMETRE_VOIE_AUTOZERO )	// Acquisition de la valeur du ZERO
		{
			if( u8CptStabilisationMesure > 0 )				// Pas de filtrage au reset
			{
				f32_Zero = C;
				u8CptStabilisationMesure--;
			}
			else											// Moyenne du zero
			{
				g_u8StabilisationMesure = true;

				f32_temp = f32_Zero - C;
				if( (f32_temp > -SEUIL_FILT_ZERO) && (f32_temp < SEUIL_FILT_ZERO) )	// Acq Zero E : -SEUIL_FILT_ZERO < zero < +SEUIL_FILT_ZERO ?
				{
					f32_Zero = (CPT_CONF_ZERO_MAX * f32_Zero + C) / (CPT_CONF_ZERO_MAX + 1); 						// Moyenne du zero avec la derniere acq
					u8FiltrageCptMesureValide = 0;
				}
				else if( ++u8FiltrageCptMesureValide >= CPT_CONF_ZERO_MAX )	// Acq zero en dehors du seuil
				{													// Valeur confirmée ?
					f32_Zero = C;
					u8FiltrageCptMesureValide = 0;
				}
			}
		}
		else                              				// Non : alors traitement des mesures
		{
			// Etalonnage
			if( g_Etalonnage.eStatus == ETALONNAGE_STATUS_PROCESSING )
			{
				VOLT_Etalonnage(psVoltmetre->u8Voie, R);
			}

			// Traitement normal
			f32_temp = R * G_DeviceCfg.sEtalonnageVoieMesure.f32Cal_Current;			   // Calcul du COURANT
			if( (f32_temp >= -NBP_I_MAX) && (f32_temp <= +NBP_I_MAX) )	// Saturation ?
				U = f32_temp; 							      // U en saturation
		}
	}

#ifdef PILE_MESURES
	if( (psVoltmetre->s8Status == ACQUISITION_MESURE_READY) && (psVoltmetre->u8Voie == VOLTMETRE_VOIE_COURANT) )
	{
		g_debug.sVal[ g_debug.u8Idx ].u8Voie	= psVoltmetre->u8Voie;
		g_debug.sVal[ g_debug.u8Idx ].f32_Zero 	= f32_Zero;
		g_debug.sVal[ g_debug.u8Idx ].N 	    = psVoltmetre->s16N;
		g_debug.sVal[ g_debug.u8Idx ].t2 	    = psVoltmetre->u16t1;
		g_debug.sVal[ g_debug.u8Idx ].temps    	= psVoltmetre->u32temps;
		g_debug.sVal[ g_debug.u8Idx ].C 	    = C;
		g_debug.sVal[ g_debug.u8Idx ].R 	    = R;
		g_debug.sVal[ g_debug.u8Idx ].U 	    = U;

		g_debug.u8Idx = (g_debug.u8Idx + 1) % MAX_PILE;

		static uint8_t cpt = 0;
		cpt = (cpt + 1) % 100;
		if( cpt == 0 )
			cpt = 1;
	}
#endif

	return( U );
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
void voCoreVoltmetre_Init(void)
{
	voDrvFlexTimer_Init();
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-14
///
//----------------------------------------------------------------------------*/
void voCoreVoltmetre_ProcessIt(void)
{
	static uint8_t 				u8StateMachine 	 = 0;     						// Séquencement du voltmetre
	static uint8_t 				u8VoieEnAcqu   	 = VOLTMETRE_VOIE_AUTOZERO;   	// Indique la voie en cours d'acquisition
	static uint8_t 				u8Delay   	  	 = 0;							// Temporisation pour le séquencement du voltmetre
	static uint8_t 				u8SmVoieDeMesure = 0;     						// Indique le numéro de séquencement de la voie mesure suivante
	static t_voltmetre_mesure 	sMesure;
	static uint8_t NVR;                      	// Nb de coups d'horloge (Time out 2.5 KHz) avec la bascule = 0
	static uint16_t 	t2;          				// Temps de fin de mesure m�moris� par l'input capture
	uint8_t u8CaptureEventFlag;
	uint8_t s8IntegratorState;
	uint8_t u8SendSample = false;

/*	static uint32_t toto1 = 0;
	static uint32_t toto2 = 0;
	static uint32_t toto3 = 0;
	toto1 = toto2;
	toto2 = u32DrvFlexTimer_GetTimerCount();
	toto3 = toto2 - toto1;
	DrvGpio_DebugToggle();
	*/

	u8Delay--;
	s8IntegratorState = u32DrvGpio_VoltmetreReadBascule();

	switch( u8StateMachine )
	{
		case 0 : voDrvGpio_VoltmetreSelectVoie( u8VoieEnAcqu );								// Chargement voie sur prochain Time out (2 KHz)
				 u8Delay = TEMPS_INTEGRATION_TICK;
				 u8StateMachine++;
				 break;

		case 1 : if( u8Delay == 0 )
				 {
					voDrvGpio_VoltmetreSelectVoie( VOLTMETRE_VOIE_AUTOZERO );						// Chargement Autoz�ro sur prochain Time out (2 KHz)
					u8StateMachine++;
					u8Delay = 50;										// On attend la capture de fin de mesure pendant 50 TO max
				 }
				 break;

		case 2 : u32DrvFlexTimer_GetTimerCaptureValue( NULL );
				 if( (s8IntegratorState != MONTE_INTEGRATEUR) || (u8Delay == 0) )// = 0000000 !!!!!!!!!    Lecture entree Bascule (Mont�e de l'int�grateur)
					 u8StateMachine++;
				 break;

		case 3 : t2 = u32DrvFlexTimer_GetTimerCaptureValue( &u8CaptureEventFlag );
				 if( u8Delay == 0 )
				 {
					 sMesure.u16t1    = u32DrvFlexTimer_GetTimerCount();								// Valeur courante du Timer
					 sMesure.s8Status = ACQUISITION_MESURE_ERROR;   // Oui, pr�vient le Prog Principal D�faut Acquisition
					 u8SendSample       = true;
				 }
				 else if( (u8CaptureEventFlag == true) && (s8IntegratorState == MONTE_INTEGRATEUR) )
				 {// Capture Flag on FTM0 ch5 (fin de mesure) ?
					 // Lecture entree Bascule (Mont�e de l'int�grateur) => on valide la fin de mesure
					 sMesure.u32temps = (t2 - sMesure.u16t1) & 0x0000FFFF;
					while( sMesure.u32temps < TEMPS_INTEGRATION_MESURE_TICK )
						sMesure.u32temps += 65536;
					sMesure.u16t1 = t2;										// Temps de d�but de la mesure courante
					sMesure.s16N  = NVR;

					if( (sMesure.s16N >= N_MIN) && (sMesure.s16N <= N_MAX) ) 				// La plage de tension est acceptable ?
						sMesure.s8Status = ACQUISITION_MESURE_READY;                				// Oui, pr�vient le Prog Principal Acquisition Ok
					else
						sMesure.s8Status = ACQUISITION_MESURE_OVERFLOW;                				// Oui, pr�vient le Prog Principal Acquisition SATUREE

					u8SendSample = true;
				 }

				 if( u8SendSample == true )
				 {
					 sMesure.u8Voie = u8VoieEnAcqu;	// Voie à  calculer

					 if( g_u8StabilisationMesure == true )
					 {
						 // Aprés stabilisation en temperature
						 u8SmVoieDeMesure = (u8SmVoieDeMesure + 1) % NB_MESURE_PAR_CYCLE;
						 if( u8SmVoieDeMesure == 0 )
							 u8VoieEnAcqu = VOLTMETRE_VOIE_AUTOZERO;
						 else
							 u8VoieEnAcqu = VOLTMETRE_VOIE_COURANT;
					 }
					 else
					 {
						 // Au reset avant stabilisation en temperature
						 // on alterne Zero/Mesure1 /Zero/Mesure1...
						 u8VoieEnAcqu = (u8VoieEnAcqu + 1) % VOLTMETRE_VOIE_COUNT;
					 }

					 u8StateMachine = 0;              			// Init nouveau cycle de mesure
					 NVR 			= 0;
				 }
				 break;
	}

	if( s8IntegratorState == MONTE_INTEGRATEUR )				// Lecture entree Bascule
		NVR++;

	if( u8SendSample == true )
	{
		voOsMsgQueue_SendFromISR(OS_MSG_QUEUE_VOLTMETRE, &sMesure);
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
void voCoreVoltmetre_Process(t_voltmetre_mesure *psVoltmetre)
{
	g_sMeasure.f32Value = f32_VOLT_Calcul_U(psVoltmetre);

	if( g_sMeasure.f32Value == MESURE_EN_DEFAUT )
		g_sMeasure.s8Status = false;
	else
		g_sMeasure.s8Status = true;
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
void voCoreVoltmetre_GetMeasure(t_measure *psMeasure)
{
	memcpy(psMeasure, &g_sMeasure, sizeof(g_sMeasure));
}

int8_t s8CoreVoltmetre_SetEtalonnage(uint8_t u8Cmd, uint16_t u16Voie)
{
	static enum{
		SM_IDLE       = 0,
		SM_PROCESSING = 1,
	}u8StateMachine = SM_IDLE;
	int8_t s8Ret = -1;

	if( u8StateMachine == SM_IDLE )
	{
		if( (u8Cmd == CMD_ETALONNAGE_4_20MA) &&      		 // Demande d'etalonnage d'un calibre existant (0.2V/20mA/100 Ohm/240 Ohm/370 Ohm) ?
			(u16Voie > 0) 					 &&				 //	n° de voies correct ?
			(u16Voie <= ((1 << VOLTMETRE_VOIE_COUNT) - 1)) ) // n° de voies correct ?
		{
			g_Etalonnage.u16Voie  = u16Voie;           		// Stockage des n� de voies a etalonner
			g_Etalonnage.u8Cmd    = u8Cmd;					// Stockage status
			g_Etalonnage.eStatus  = ETALONNAGE_STATUS_PROCESSING;
			u8StateMachine    	  = SM_PROCESSING;
			s8Ret 			      = (int8_t)u8Cmd; 			// Reponse du status
		}
	}
	else
	{
		if( g_Etalonnage.eStatus == ETALONNAGE_STATUS_PROCESSING )
		{
			s8Ret = (int8_t)g_Etalonnage.u8Cmd;
		}
		else
		{
			s8Ret 				 = (int8_t)g_Etalonnage.eStatus;	// 0 => OK  /  -1 => KO
			g_Etalonnage.eStatus = ETALONNAGE_STATUS_IDLE;
			u8StateMachine 		 = SM_IDLE;
		}
	}

	return( s8Ret );
}

