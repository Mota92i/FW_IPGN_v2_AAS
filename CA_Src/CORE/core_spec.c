//*****************************************************************************/
/// \file           app_core_spec.c
/// \version        1.0
/// \author         Aseervatham
/// \date           2018-10-11
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
    #include "core_spec.h"
#undef public

#define public extern
    #include <string.h>
	#include <stddef.h>
    #include "drv_eeprom.h"
    #include "drv_display.h"
    #include "task_main.h"
	#include "drv_serial.h"
    #include "task_main.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define FLAG_EEPROM_OK			( 0xAA55 )
#define DEFAULT_UDP_PORT		( 2000 )
#define RETRY_EEPROM_READ_WRITE ( 3 )

/******************************************************************************/
//  constant declaration
/******************************************************************************/
static const t_device_info sDeviceInfoOnError = {
	.u8DeviceType    = 0,
	.u16SerialNumber = 300,
	.u8ManufactureDate = {'1', '7','/','1', '2', '/', '2', '0', '2', '1'},
	.u8HdwRelease = {0, 0},
	.u8MacAddress = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static int32_t s32ReadFromEeprom(uint8_t u8DataType, void *pvo_Data);
static int32_t s32WriteToEeprom(uint8_t u8DataType, void *pvo_Data);

static int32_t s32Consistency_DeviceInfo(int32_t Error, void *pvo_Data);
static int32_t s32Consistency_Brightness(int32_t Error, void *pvo_Data);
static int32_t s32Consistency_EtalonnageVoieDeMesure(int32_t Error, void *pvo_Data);
static int32_t s32Consistency_ReglageMoteur(int32_t Error, void *pvo_Data);
static int32_t s32Consistency_Spec(int32_t Error, void *pvo_Data);

/******************************************************************************/
//  constant declaration
/******************************************************************************/
typedef struct {
	int32_t  s32Adr;
	uint32_t u32DataSize;
	void     *pvoDataRam;
	int32_t  (*pConsistencyCheck)(int32_t Error, void *pvo_Data);
}t_spec_param;

const t_spec_param g_sparam[ EEPROM_IDX_COUNT ] = {
	[ EEPROM_IDX_DEVICE_INFO            ] = { EEPROM_ADDRESS_DEVICE_INFO, 			 sizeof(t_device_info),            &G_DeviceCfg.sInfo,				   &s32Consistency_DeviceInfo             },
	[ EEPROM_IDX_CPT_HORAIRE            ] = { -1, 									 0,                     		   NULL,							   NULL	                                  },
	[ EEPROM_IDX_BRIGHTNESS             ] = { EEPROM_ADDRESS_BRIGHTNESS,			 1, 					   		   &G_DeviceCfg.u8Brightness,		   &s32Consistency_Brightness             },
	[ EEPROM_IDX_ETALONNAGE_VOIE_MESURE ] = { EEPROM_ADDRESS_ETALONNAGE_VOIE_MESURE, sizeof(t_etalonnage_voie_mesure), &G_DeviceCfg.sEtalonnageVoieMesure, &s32Consistency_EtalonnageVoieDeMesure },
	[ EEPROM_IDX_REGLAGE_MOTEUR         ] = { EEPROM_ADDRESS_REGLAGE_MOTEUR, 		 sizeof(t_reglage_moteur), 		   &G_DeviceCfg.sReglageMoteur, 	   &s32Consistency_ReglageMoteur 		  },
	[ EEPROM_IDX_SPEC                   ] = { EEPROM_ADDRESS_SPEC, 					 sizeof(t_specialisation), 		   &G_DeviceCfg.sSpec,				   &s32Consistency_Spec                   },
};

/******************************************************************************/
//  constant declaration
/******************************************************************************/
static const t_app_device g_sAppDevice[ SPEC_TYPE_COUNT ] =
{
	[ SPEC_TYPE_CAP					] = {   .eInputData 						= INPUT_DATA_SERIAL,
											.u8protocol 						= DRV_SERIAL_NMEA_LISTEN,
											.sSerialCfg.u32Speed  				= 4800,
											.sSerialCfg.u8Parity				= SERIAL_PORT_PARITY_NONE,
											.u8ModeDeplacementAiguille			= MODE_DEPLACEMENT_AIGUILLE_INDICATEUR_DIRECTION },

	[ SPEC_TYPE_BARRE_AVANT     	] = { 	.eInputData 						= INPUT_DATA_ANALOG,
											.u8protocol 						= DRV_SERIAL_MODBUS_RTU,
											.sSerialCfg.u32Speed  				= 9600,
											.sSerialCfg.u8Parity				= SERIAL_PORT_PARITY_NONE,
											.u8ModeDeplacementAiguille			= MODE_DEPLACEMENT_AIGUILLE_GALVANOMETRE },

	[ SPEC_TYPE_BARRE_DIRECTION    	] = { 	.eInputData 						= INPUT_DATA_ANALOG,
											.u8protocol 						= DRV_SERIAL_MODBUS_RTU,
											.sSerialCfg.u32Speed  				= 9600,
											.sSerialCfg.u8Parity				= SERIAL_PORT_PARITY_NONE,
											.u8ModeDeplacementAiguille			= MODE_DEPLACEMENT_AIGUILLE_GALVANOMETRE },

	[ SPEC_TYPE_BARRE_ARRIERE    	] = { 	.eInputData 						= INPUT_DATA_ANALOG,
											.u8protocol 						= DRV_SERIAL_MODBUS_RTU,
											.sSerialCfg.u32Speed  				= 9600,
											.sSerialCfg.u8Parity				= SERIAL_PORT_PARITY_NONE,
											.u8ModeDeplacementAiguille			= MODE_DEPLACEMENT_AIGUILLE_GALVANOMETRE },

	[ SPEC_TYPE_ANEMOGIR_CA_CAP    	] = {   .eInputData 						= INPUT_DATA_SERIAL,
											.u8protocol 						= DRV_SERIAL_MODBUS_RTU,
											.sSerialCfg.u32Speed  				= 9600,
											.sSerialCfg.u8Parity				= SERIAL_PORT_PARITY_NONE,
											.u8ModeDeplacementAiguille			= MODE_DEPLACEMENT_AIGUILLE_INDICATEUR_DIRECTION },

	[ SPEC_TYPE_ANEMOGIR_CA_VITESSE	] = {   .eInputData 						= INPUT_DATA_SERIAL,
											.u8protocol 						= DRV_SERIAL_MODBUS_RTU,
											.sSerialCfg.u32Speed  				= 9600,
											.sSerialCfg.u8Parity				= SERIAL_PORT_PARITY_NONE,
											.u8ModeDeplacementAiguille			= MODE_DEPLACEMENT_AIGUILLE_GALVANOMETRE },
};

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
static int32_t s32ReadFromEeprom(uint8_t u8DataType, void *pvo_Data)
{
	uint16_t u16Flag;
    int32_t  Error;
    t_spec_param const *p_param;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
    if( pvo_Data == NULL )
    	return( 0 );

	// -------------------------------------------------------------------------
	// Verify Index
	// -------------------------------------------------------------------------
    if( u8DataType >= EEPROM_IDX_COUNT )
    	return( -1 );

	// -------------------------------------------------------------------------
	// No read
	// -------------------------------------------------------------------------
    p_param	= &g_sparam[ u8DataType ];
	if(  p_param->s32Adr < 0 )
		return( 0 );

	// -------------------------------------------------------------------------
	// Read flag validity
	// -------------------------------------------------------------------------
	Error = DrvEeprom_Read( p_param->s32Adr,
							(uint8_t *)&u16Flag,
							sizeof(u16Flag),
							RETRY_EEPROM_READ_WRITE );

	// -------------------------------------------------------------------------
	// Read data
	// -------------------------------------------------------------------------
	if( (Error == 0) && (u16Flag == FLAG_EEPROM_OK) )
	{
		Error = DrvEeprom_Read( ( p_param->s32Adr + sizeof(u16Flag)),
                             	(uint8_t *)pvo_Data,
								p_param->u32DataSize,
								RETRY_EEPROM_READ_WRITE );
	}
	else
	{
		Error = -1;
	}

	// -------------------------------------------------------------------------
	// Consistency Check
	// -------------------------------------------------------------------------
	if( p_param->pConsistencyCheck != NULL )
	{
		Error =  p_param->pConsistencyCheck(Error, pvo_Data);
	}

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
/// \date
///
//----------------------------------------------------------------------------*/
static int32_t s32WriteToEeprom(uint8_t u8DataType, void *pvo_Data)
{
	uint16_t u16Flag;
    int32_t  Error;
    t_spec_param const *p_param;

	// -------------------------------------------------------------------------
	//
	// -------------------------------------------------------------------------
    if( pvo_Data == NULL )
    	return( 0 );

	// -------------------------------------------------------------------------
	// Verify Index
	// -------------------------------------------------------------------------
    if( u8DataType >= EEPROM_IDX_COUNT )
    	return( -1 );

    p_param	= &g_sparam[ u8DataType ];

	// -------------------------------------------------------------------------
	// No read
	// -------------------------------------------------------------------------
	if(  p_param->s32Adr < 0 )
		return( 0 );

	// -------------------------------------------------------------------------
	// Write data
	// -------------------------------------------------------------------------
	Error = DrvEeprom_Write( (p_param->s32Adr + sizeof(u16Flag)),
							 (uint8_t *)pvo_Data,
							 p_param->u32DataSize,
							 RETRY_EEPROM_READ_WRITE );

	// -------------------------------------------------------------------------
	// Write flag validity
	// -------------------------------------------------------------------------
	if( Error == 0 )
	{
		Error = DrvEeprom_Read( p_param->s32Adr,
								(uint8_t *)&u16Flag,
								sizeof(u16Flag),
								RETRY_EEPROM_READ_WRITE );

		if( (Error == 0) && (u16Flag != FLAG_EEPROM_OK) )
		{
			u16Flag = FLAG_EEPROM_OK;
			Error = DrvEeprom_Write( p_param->s32Adr,
									(uint8_t *)&u16Flag,
									sizeof(u16Flag),
									RETRY_EEPROM_READ_WRITE );
		}
	}

	// -------------------------------------------------------------------------
	// Update RAM
	// -------------------------------------------------------------------------
	if( p_param->pvoDataRam != pvo_Data )
		memcpy(p_param->pvoDataRam, pvo_Data, p_param->u32DataSize);

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
/// \date
///
//----------------------------------------------------------------------------*/
static int32_t s32Consistency_DeviceInfo(int32_t Error, void *pvo_Data)
{
	t_device_info *p_Data = (t_device_info *)pvo_Data;

	if( Error == 0 )
	{
		if( (p_Data->u16SerialNumber < 300) || (p_Data->u16SerialNumber > 9999) )
		{
			p_Data->u16SerialNumber = sDeviceInfoOnError.u16SerialNumber;
			Error = -1;
		}
	}
	else
	{
		memcpy(pvo_Data, (void *)&sDeviceInfoOnError, sizeof(sDeviceInfoOnError));
    	voTaskMain_FlagErrorSetMacAdr();

		Error = -1;
	}

    ((t_device_info *)pvo_Data)->u8DeviceType = 0;

    return( Error );
}

int32_t CoreSpec_DeviceInfoWrite(t_device_info *p_Data)
{
    return( s32WriteToEeprom(EEPROM_IDX_DEVICE_INFO, p_Data) );
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
static int32_t s32Consistency_Brightness(int32_t Error, void *pvo_Data)
{
	t_device_cfg *p_Data = (t_device_cfg *)pvo_Data;

	if( (Error != 0) || (p_Data->u8Brightness > DISPLAY_BRIGHTNESS_MAX) )
	{
		p_Data->u8Brightness = DISPLAY_BRIGHTNESS_DEFAULT;

	    Error = -1;
	}

    return( Error );
}

int32_t CoreSpec_BrightnessWrite(void)
{
    return( s32WriteToEeprom(EEPROM_IDX_BRIGHTNESS, g_sparam[ EEPROM_IDX_BRIGHTNESS ].pvoDataRam) );
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
static int32_t s32Consistency_EtalonnageVoieDeMesure(int32_t Error, void *pvo_Data)
{
	t_etalonnage_voie_mesure *psRtalonnageVoie_Mesure = (t_etalonnage_voie_mesure *)pvo_Data;

	if( Error != 0 )
	{
		// Todo
		psRtalonnageVoie_Mesure->f32Cal_Current = 0.024970591;

	    Error = -1;
	}

    return( Error );
}

int32_t CoreSpec_EtalonnageVoieDeMesureWrite(t_etalonnage_voie_mesure *p_Data)
{
    return( s32WriteToEeprom(EEPROM_IDX_ETALONNAGE_VOIE_MESURE, p_Data) );
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
static int32_t s32Consistency_ReglageMoteur(int32_t Error, void *pvo_Data)
{
	t_reglage_moteur *psRaglageMoteur = (t_reglage_moteur *)pvo_Data;

	// -------------------------------------------------------------------------
	// Check consistency
	// -------------------------------------------------------------------------
	if( (psRaglageMoteur->u16ZeroDir >= 3600) 	||
		(psRaglageMoteur->u16ZeroEch >= 3600)	||
		(psRaglageMoteur->u16FinEch  >= 3600)	||
		(psRaglageMoteur->u16Calage  >= 3600) )
	{
		Error = -1;
	}

	// -------------------------------------------------------------------------
	// if error set to default value
	// -------------------------------------------------------------------------
	if( Error != 0 )
	{
		/*psRaglageMoteur->u16ZeroDir = 173;
		psRaglageMoteur->u16ZeroEch = 1208;
		psRaglageMoteur->u16FinEch  = 2393;
		psRaglageMoteur->u16Calage  = 2335;*/

		psRaglageMoteur->u16ZeroDir = 0;
		psRaglageMoteur->u16ZeroEch = 0;
		psRaglageMoteur->u16FinEch  = 2700;
		psRaglageMoteur->u16Calage  = 0;

	    Error = -1;
	}

    return( Error );
}

int32_t CoreSpec_ReglageMoteurWrite(t_reglage_moteur *p_Data)
{
    return( s32WriteToEeprom(EEPROM_IDX_REGLAGE_MOTEUR, p_Data) );
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
static int32_t s32Consistency_Spec(int32_t Error, void *pvo_Data)
{
	t_specialisation *psSpec = (t_specialisation *)pvo_Data;

	// -------------------------------------------------------------------------
	// Check consistency
	// -------------------------------------------------------------------------
	if( (psSpec->u32Type >= SPEC_TYPE_COUNT) 							||
		((psSpec->u32Rayon < 1)        || (psSpec->u32Rayon > 2000))	||
		((psSpec->s32Course4 < -2000)  || (psSpec->s32Course4 > 2000) )	||
		((psSpec->s32Course20 < -2000) || (psSpec->s32Course20 > 2000))	||
		((psSpec->s32Offset < -20)     || (psSpec->s32Offset > 20)) )
	{
		Error = -1;
	}

	// -------------------------------------------------------------------------
	// if error set to default value
	// -------------------------------------------------------------------------
	if( Error != 0 )
	{
		psSpec->u32Type 	= SPEC_TYPE_CAP;
		psSpec->u32Rayon 	= 435;
		psSpec->s32Course4 	= -173;
		psSpec->s32Course20 = 173;
		psSpec->s32Offset 	= 0;

		Error = -1;
	}

	return( Error );
}

int32_t CoreSpec_SpecWrite(t_specialisation *p_Data)
{
    return( s32WriteToEeprom(EEPROM_IDX_SPEC, p_Data) );
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
int32_t CoreSpec_Load(void)
{
    int32_t Error = 0;
    int32_t i;

//DrvEeprom_Erase();

    for(i = 0; i < EEPROM_IDX_COUNT; i++)
    	Error += s32ReadFromEeprom( i, g_sparam[ i ].pvoDataRam );

    if( Error < 0 )
    	Error = -1;
//debug Ajith
G_DeviceCfg.sSpec.u32Type = SPEC_TYPE_ANEMOGIR_CA_VITESSE;
//G_DeviceCfg.sSpec.u32Type = SPEC_TYPE_ANEMOGIR_CA_CAP;

    //
    G_DeviceCfg.psAppDeviceCfg = &g_sAppDevice[ G_DeviceCfg.sSpec.u32Type ];

    return( Error );
}
