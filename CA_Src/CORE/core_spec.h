//*****************************************************************************/
/// \file           core_spec.h
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
//  Inclusion checking
//*****************************************************************************/
#ifndef _CORE_SPEC_H_
#define _CORE_SPEC_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include "app_types.h"
#include <stdint.h>

/******************************************************************************/
//  constant declaration
/******************************************************************************/
typedef enum {
	EEPROM_IDX_NULL						= -1,
	EEPROM_IDX_DEVICE_INFO  			= 0,
	EEPROM_IDX_CPT_HORAIRE,
	EEPROM_IDX_BRIGHTNESS,
	EEPROM_IDX_ETALONNAGE_VOIE_MESURE,
	EEPROM_IDX_REGLAGE_MOTEUR,
	EEPROM_IDX_SPEC,

	EEPROM_IDX_COUNT
}EEPROM_IDX;

typedef enum {
	// ZONE USINE
	EEPROM_ADDRESS_DEVICE_INFO                  = 0x0000,											// 2 + 21o

	// ZONE COMPTEUR HORAIRE
	EEPROM_ADDRESS_CPT_HORAIRE_DEBUT       		= 0x0020,											// 256
	EEPROM_ADDRESS_CPT_HORAIRE_FIN       		= (EEPROM_ADDRESS_CPT_HORAIRE_DEBUT + 256),

	// DISPLAY BRIGHTNESS
	EEPROM_ADDRESS_BRIGHTNESS             		= 0x0130,											// 2 + 1o

	// ZONE ETHERNET
	EEPROM_ADDRESS_NETWORK_INFO            		= 0x0140,											// 2 + 15o

	// SERIAL
	EEPROM_ADDRESS_SERIAL_LINK             		= 0x0160,											// 2 + 5o

	// VOIE DE MESURE ETALONNAGE
	EEPROM_ADDRESS_ETALONNAGE_VOIE_MESURE       = 0x0170,											// 2 + 20o

	// REGLAGE MESURE
	EEPROM_ADDRESS_REGLAGE_MOTEUR       		= 0x0190,											// 2 + 8o

	// SPEC
	EEPROM_ADDRESS_SPEC             			= 0x0300,											// 2 + 2080o
}EEPROM_ADDRESS;

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef enum{
	INPUT_DATA_ANALOG = 0,
	INPUT_DATA_SERIAL,

	INPUT_DATA_COUNT
}t_input_data;

typedef struct
{
	t_input_data 		eInputData;
	uint8_t				u8protocol;
	t_serial_link_cfg	sSerialCfg;
	uint8_t				u8ModeDeplacementAiguille;
}t_app_device;

typedef struct
{
	t_device_info		sInfo;
	uint8_t				u8Brightness;
	t_device_network	sNetwork;
	t_specialisation			sSpec;
	t_etalonnage_voie_mesure	sEtalonnageVoieMesure;
	t_reglage_moteur			sReglageMoteur;

	// Ceci est construit à partir des paramétres Eeprom
	t_app_device		const *psAppDeviceCfg;

	struct{
		t_modbus_cfg	sEeprom;	// Image de l'eeprom
		t_modbus_cfg	sUdp;		// Modification par les cmd UDP
		t_modbus_cfg	sSerial;	// Modification par les cmd Modbus écritures
	}sModbus;

	uint8_t				u8SerialReqStatus;
	uint8_t				u8SerialReqNumAbonneeLed;
}t_device_cfg;

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
public t_device_cfg      G_DeviceCfg;

//*****************************************************************************/
//  Multi-file-scope function prototype declarations
//*****************************************************************************/
extern int32_t CoreSpec_BrightnessWrite(void);

extern int32_t CoreSpec_SpecWrite(t_specialisation *p_Data);
extern int32_t CoreSpec_EtalonnageVoieDeMesureWrite(t_etalonnage_voie_mesure *p_Data);
extern int32_t CoreSpec_ReglageMoteurWrite(t_reglage_moteur *p_Data);

extern int32_t CoreSpec_NetworkInfoWrite(t_device_network *p_Data);
extern int32_t CoreSpec_DeviceInfoWrite(t_device_info *p_Data);
extern int32_t CoreSpec_ModbusLinkWrite(t_serial_link_cfg *p_Data);
extern int32_t CoreSpec_Load(void);

extern int32_t CoreSpec_SaveModbusSlaveSpec(void);
#endif // _CORE_SPEC_H_
