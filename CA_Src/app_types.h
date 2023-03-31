//*****************************************************************************/
/// \file           app_types.h
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
#ifndef _APP_TYPE_DEF_H_
#define _APP_TYPE_DEF_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define MODBUS_ABONNE_COUNT 	(16)

/******************************************************************************/
//  constant declaration
/******************************************************************************/
#define PARAM_MANUFACTURE_DATE_SIZE             ( 10 )
#define PARAM_HDW_RELEASE_SIZE                  ( 2 )
#define PARAM_MAC_ADDRESS_SIZE                  ( 6 )
#define PARAM_FW_RELEASE_SIZE					( 22 )

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
    NETWORK_DHCP_DISABLE                        = 1,
    NETWORK_DHCP_ENABLE                         = 0,
     
    NETWORK_DHCP_COUNT
};


/******************************************************************************/
//  types declaration 
/******************************************************************************/
typedef struct
{
    uint8_t     bDhcpState;
    uint32_t    u32IpAddress;
    uint32_t    u32SubMask;
    uint32_t    u32DefaultGateway;
    uint16_t    u16PortNum;
}__attribute__((packed)) t_device_network;

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef struct
{
	uint8_t		u8DeviceType;
    uint16_t	u16SerialNumber;
    uint8_t		u8ManufactureDate[PARAM_MANUFACTURE_DATE_SIZE];
    uint8_t		u8HdwRelease[PARAM_HDW_RELEASE_SIZE];
    uint8_t     u8MacAddress[PARAM_MAC_ADDRESS_SIZE];
}__attribute__((packed)) t_device_info;

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef struct
{
	uint16_t	u16ZeroDir;
	uint16_t	u16ZeroEch;
	uint16_t	u16FinEch;
	uint16_t	u16Calage;
}__attribute__((packed)) t_reglage_moteur;

/******************************************************************************/
//  constant declaration
/******************************************************************************/
#define MASK_MODBUS_SLAVE_FCT_CODE_DYNAMIC_DATA		( 0x0001 )
#define MASK_MODBUS_SLAVE_FCT_CODE_3				( 0x0004 )
#define MASK_MODBUS_SLAVE_FCT_CODE_4      		  	( 0x0008 )
#define MASK_MODBUS_SLAVE_FCT_CODE_16     		  	( 0x8000 )

#define MASK_MODBUS_SLAVE_FCT_CODE     		  		( MASK_MODBUS_SLAVE_FCT_CODE_DYNAMIC_DATA 	| \
													  MASK_MODBUS_SLAVE_FCT_CODE_3 				| \
													  MASK_MODBUS_SLAVE_FCT_CODE_4 				| \
													  MASK_MODBUS_SLAVE_FCT_CODE_16 )

#define MODBUS_SLAVE_SPEED_B1200		( 1200 )
#define MODBUS_SLAVE_SPEED_B2400		( 2400 )
#define MODBUS_SLAVE_SPEED_B4800		( 4800 )
#define MODBUS_SLAVE_SPEED_B9600		( 9600 )
#define MODBUS_SLAVE_SPEED_B19200		( 19200 )
#define MODBUS_SLAVE_SPEED_B38400		( 38400 )
#define MODBUS_SLAVE_SPEED_B57600		( 57600 )
#define MODBUS_SLAVE_SPEED_B115200		( 115200 )

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef struct
{
	  uint32_t  u32Speed;
	  uint8_t   u8Parity;
}__attribute__((packed)) t_serial_link_cfg;

typedef struct
{
	uint16_t	u16Nab;
	uint16_t	u16TpRep;
	uint16_t	u16Fct;
	uint16_t	u16Addr;
	uint16_t	u16NbMot;
	uint16_t	u16DataArray[125];
}__attribute__((packed)) t_modbus_abonne_cfg;

typedef struct
{
	t_serial_link_cfg 	sLinkCfg;
	t_modbus_abonne_cfg sAbonne[ MODBUS_ABONNE_COUNT ];
}__attribute__((packed)) t_modbus_cfg;

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
    MODE_DEPLACEMENT_AIGUILLE_INDICATEUR_DIRECTION	= 0,
	MODE_DEPLACEMENT_AIGUILLE_GALVANOMETRE,

	MODE_DEPLACEMENT_COUNT
};

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
    SPEC_TYPE_CAP				= 0,
	SPEC_TYPE_BARRE_AVANT,
	SPEC_TYPE_BARRE_DIRECTION,
	SPEC_TYPE_BARRE_ARRIERE,

	SPEC_TYPE_ANEMOGIR_CA_CAP,
	SPEC_TYPE_ANEMOGIR_CA_VITESSE,

	SPEC_TYPE_COUNT
};

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef struct
{
	uint32_t u32Type;       // Type Indicateur (0:CAP / 1:BARRE AVANT / 2:BARRE de DIRECTION / 3:BARRE ARRIERE)
	uint32_t u32Rayon;		// Rayon du palonnier (1 à 2000 mm)
	int32_t	 s32Course4;	// Course pour I = 4 mA (-2000 à 2000 mm)
	int32_t	 s32Course20;	// Course pour I = 20 mA (-2000 à 2000 mm)
	int32_t	 s32Offset;		// Offset angulaire (-2,0° à +2,0°)
}__attribute__((packed)) t_specialisation;

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef struct
{
	float f32Cal_Current;
	float NU[ 4 ];
}__attribute__((packed)) t_etalonnage_voie_mesure;

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef struct{
	float	 f32Value;
	int8_t   s8Status;
}t_measure;

typedef enum{
	ANGLE_VALID 		 = 0,
	ANGLE_OVER_UNDERFLOW = -1,
	ANGLE_ERROR 		 = -2,
}t_angle_status;

typedef struct{
	float  f32Angle;
	float  f32Course;
	float  f32AngleMotor;
	int8_t s8Valid;
}t_angle;

typedef struct{
	t_measure sMeasureSa;
	t_angle	  sMotorAngle;
}t_app_measure;

/******************************************************************************/
//  Types declaration
/******************************************************************************/
typedef enum {
	APP_MEASURE_MSG_QUEUE_VOLTMETRE = 0,
	APP_MEASURE_MSG_QUEUE_SIMU		= 1,
	APP_MEASURE_MSG_QUEUE_SERIAL	= 2,
}t_app_measure_event;

typedef struct{
	t_app_measure_event eEventType;
	t_measure 			sMeasureSa;
}t_app_measure_msg;

/******************************************************************************/
//  Type declaration
/******************************************************************************/
typedef struct{
	uint16_t u16Step;
	uint16_t u16Angle;
}t_motor_position;

/******************************************************************************/
//  constant declaration
/******************************************************************************/
enum{
	MODE_FONCTIONNEMENT_NORMAL 		= 0,
	MODE_FONCTIONNEMENT_SIMULATION,
	MODE_FONCTIONNEMENT_TEST_RELAIS,

	MODE_FONCTIONNEMENT_COUNT
};

#endif // _APP_TYPE_DEF_H_ 
