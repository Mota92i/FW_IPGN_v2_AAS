/******************************************************************************/
/// \file           core_compteur_horaire.c
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
//  Included header file list
/******************************************************************************/
#define public
	#include <stdbool.h>
	#include <string.h>
	#include <stdlib.h>
    #include "core_compteur_horaire.h"
#undef public

#define public extern
    #include "drv_eeprom.h"
    #include "core_spec.h"
#undef public

/******************************************************************************/
//  constant declaration
/******************************************************************************/
#define PERIODIC_SAVE_CPT_HORAIRE_MINUTE		( 10 )	// 10mn

/******************************************************************************/
//  constant declaration
/******************************************************************************/
#define EE_ADR_DEB_CPTH		( EEPROM_ADDRESS_CPT_HORAIRE_DEBUT )
#define EE_ADR_FIN_CPTH		( EEPROM_ADDRESS_CPT_HORAIRE_FIN )
#define CPTH_BUFFER_SIZE	( EEPROM_ADDRESS_CPT_HORAIRE_FIN - EEPROM_ADDRESS_CPT_HORAIRE_DEBUT )

/******************************************************************************/
//  constant declaration
/******************************************************************************/
#define RETRY_EEPROM_READ_WRITE ( 3 )

/******************************************************************************/
//  macro
/******************************************************************************/
#define GET_MARQUEUR(u32CpthVal)		(u32CpthVal & 0x80000000)
#define REMOVE_MARQUEUR(u32CpthVal)		(u32CpthVal & 0x7FFFFFFF)
#define SET_MARQUEUR_TO_0(u32CpthVal)	(u32CpthVal & 0x7FFFFFFF)
#define SET_MARQUEUR_TO_1(u32CpthVal)	(u32CpthVal | 0x80000000)

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static int32_t  s32ReadDataFromEeprom(uint32_t u32Address, uint32_t *pu32_CpthVal, uint32_t *pu32_Marqueur);
static uint16_t u16GetCptAdress(void);
static int32_t  s32WriteCptValue(uint32_t val_cpt);

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
/// \date           2021-12-03
///
//----------------------------------------------------------------------------*/
static int32_t s32ReadDataFromEeprom(uint32_t u32Address, uint32_t *pu32_CpthVal, uint32_t *pu32_Marqueur)
{
	uint32_t u32CptValue;
	uint32_t u32MarqueurValue;
	int32_t  Error;

	// Lecture en Eeprom série de la valeur pointée
	Error = DrvEeprom_Read(u32Address, (uint8_t *)&u32CptValue, sizeof(u32CptValue), RETRY_EEPROM_READ_WRITE);
	if( Error != -1 )
	{
		u32MarqueurValue = GET_MARQUEUR(u32CptValue);
		u32CptValue      = REMOVE_MARQUEUR(u32CptValue);
	}
	else
	{
		u32MarqueurValue = SET_MARQUEUR_TO_0(0);
		u32CptValue      = 0;
	}

	// Output
	if( pu32_CpthVal != NULL )
		*pu32_CpthVal = u32CptValue;

	if( pu32_Marqueur != NULL )
		*pu32_Marqueur = u32MarqueurValue;

	return( Error );
}

//----------------------------------------------------------------------------*/
// Lecture de l'adresse où se situe le  compteur
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-12-03
///
//----------------------------------------------------------------------------*/
static uint16_t u16GetCptAdress(void)
{
	uint32_t u32AdrMemoCpt = (uint32_t)EE_ADR_DEB_CPTH;
	uint32_t u32Marqueur;
	uint32_t u32Marqueur_bis;
	uint32_t i;
	int32_t	 Error;

	Error = s32ReadDataFromEeprom(u32AdrMemoCpt, NULL, &u32Marqueur);
	if( Error != 1 )
	{
		for(i = 1; i < (CPTH_BUFFER_SIZE / 4); i++)
		{
			u32AdrMemoCpt += 4;
			Error = s32ReadDataFromEeprom(u32AdrMemoCpt, NULL, &u32Marqueur_bis);
			if( Error == -1 )
			{
				u32AdrMemoCpt -= 4;
				break;
			}
			else
			{
				if( u32Marqueur != u32Marqueur_bis )
				{
					u32AdrMemoCpt -= 4;
					break;
				}
			}
		}
	}

	return( u32AdrMemoCpt );
}

//----------------------------------------------------------------------------*/
// Lecture de la valeur du compteur
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-12-03
///
//----------------------------------------------------------------------------*/
uint32_t u32ReadCptValue(void)
{
	uint32_t u32CptValue;
	uint32_t u32AdrMemoCpt;
	int32_t  Error;

	u32AdrMemoCpt = (uint32_t)u16GetCptAdress();

	// Lecture en Eeprom série de la valeur pointée
	Error = s32ReadDataFromEeprom(u32AdrMemoCpt, &u32CptValue, NULL);
	if( Error == -1 )
		u32CptValue = 0;

	return( u32CptValue );
}

//----------------------------------------------------------------------------*/
// Ecriture de la valeur du compteur
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-12-03
///
//----------------------------------------------------------------------------*/
static int32_t s32WriteCptValue(uint32_t val_cpt)
{
	uint32_t u32Marqueur;
	uint32_t u32AdrMemoCpt;
	int32_t  Error;

	u32AdrMemoCpt = (uint32_t)u16GetCptAdress();

	// Lecture en Eeprom série de la valeur pointée
	Error = s32ReadDataFromEeprom(u32AdrMemoCpt, NULL, &u32Marqueur);
	if( Error != -1 )
	{
		u32AdrMemoCpt += 4;

		if( u32AdrMemoCpt >= EE_ADR_FIN_CPTH )
		{
			u32AdrMemoCpt = EE_ADR_DEB_CPTH;
			// Inversion du marqueur lors du retournement mémoire
			if( u32Marqueur == 0 )
				u32Marqueur = SET_MARQUEUR_TO_1(0);
			else
				u32Marqueur = SET_MARQUEUR_TO_0(0);
		}

		// Ajout du marqueur
		if( u32Marqueur == 0 )
			val_cpt = SET_MARQUEUR_TO_0(val_cpt);
		else
			val_cpt = SET_MARQUEUR_TO_1(val_cpt);

		Error = DrvEeprom_Write(u32AdrMemoCpt, (uint8_t *)&val_cpt, sizeof(val_cpt), RETRY_EEPROM_READ_WRITE);
	}

   return( Error );
}

//----------------------------------------------------------------------------*/
// Initialisation du compteur en EEPROM
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-12-03
///
//----------------------------------------------------------------------------*/
int32_t s32CoreCompteurHoraire_Init(uint32_t u32ValCpt)
{
	int32_t Error = 0;

	uint32_t u32Buffer[ CPTH_BUFFER_SIZE / 4 ];

	// Initialise le buffer à 0xFFFFFFFF
	memset(&u32Buffer[ 0 ], 0xFF, sizeof(u32Buffer));

	// Marqueur sur le bit de poids fort
	u32Buffer[ 0 ] = SET_MARQUEUR_TO_0(u32ValCpt);

	// Ecriture du tableau initialise a FF en EEPROM
	Error = DrvEeprom_Write((uint32_t)EE_ADR_DEB_CPTH, (uint8_t *)&u32Buffer[ 0 ], sizeof(u32Buffer), RETRY_EEPROM_READ_WRITE);

	return( Error );
}

//----------------------------------------------------------------------------*/
// Ecriture de la valeur du compteur horaire en Eeprom
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-12-03
///
//----------------------------------------------------------------------------*/
int32_t s32CoreCompteurHoraire_Process(uint32_t u32PeriodMs)
{
	static uint8_t  bInit 					= false;
	static uint32_t u32CptHoraireTickToSave = 0;
	static uint32_t u32CptHoraire 			= 0;
	uint32_t u32CptValue;
	int32_t Error = 0;

	if( bInit == false )
	{
		bInit = true;
		u32CptHoraireTickToSave = (uint32_t)((uint32_t)(PERIODIC_SAVE_CPT_HORAIRE_MINUTE * 60 * 1000) / u32PeriodMs);
	}

	if( ++u32CptHoraire >= u32CptHoraireTickToSave )
	{
		// Lecture valeur en EEPROM
		u32CptValue = u32ReadCptValue();

		// Incrément tous les 10min
		if( u32CptValue < (uint32_t)(0x7FFFFFFF - 10))
		{
			// Ecriture nouvelle valeur en EEPROM
			Error = s32WriteCptValue((uint32_t)(u32CptValue + 10));
		}

		u32CptHoraire = 0;
	}

	return( Error );
}

