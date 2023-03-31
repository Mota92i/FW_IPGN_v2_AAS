/******************************************************************************/
/// \file           protocole_modbus_diagnostic.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2018-10-11
/// \copyright      Chauvin Arnoux Energy
///
/// \ingroup        
/// \brief          
/// \details        
///                 
/******************************************************************************/

/******************************************************************************/
//  Inclusion checking
/******************************************************************************/
#ifndef __PROTOCOL_MODBUS_DIAGNOSTIC_H__
#define __PROTOCOL_MODBUS_DIAGNOSTIC_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "app_types.h"
#include "ca_prototype.h"

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct
{
    //common frame structure
    struct __attribute__((packed)) frame_diag_cmd_st {
        uint8_t tag;
        uint8_t fct;
        uint8_t type;
        uint8_t serial_nb[2];
        uint8_t address[2];
        uint8_t nb_word[2];
    } common;

    __attribute__((packed)) union{
    	// others params
    	uint8_t param[460];

        // APNX : Write sn
        struct {
        	uint8_t  u8Type;
        	uint16_t u16SerialNumber;
        }__attribute__((packed)) cmd_0xf8;

        // APNX : Write cpth
        struct {
        	uint8_t  u8Cpth[4];
        }__attribute__((packed)) cmd_0xec;

        // APNX : Write spec
        struct {
        	t_specialisation sSpec;
        }__attribute__((packed)) cmd_0xf4;

        // Valeur simulé
        struct {
        	uint8_t  u8Measure[2];
        }__attribute__((packed)) cmd_0xe1;

        // Reglage
        struct {
        	uint8_t  u8Cmd;
        }__attribute__((packed)) cmd_0xe0;

        // Etalonnage
        struct {
        	uint8_t  u8Cmd;
        	uint32_t u32Voie;
        }__attribute__((packed)) cmd_0xf0;
    };
}__attribute__((packed)) t_frame_fct_PC_cmd;

typedef struct
{
    //common frame structure
    struct __attribute__((packed)) frame_diag_reply_st {
        uint8_t  tag;
        uint8_t  fct;
        uint8_t  type;
        uint8_t  serial_nb[2];
    } common;

    __attribute__((packed)) union{
    	// others params
    	uint8_t param[460];

        // APNX : Read firmware revision
        struct {
        	uint16_t u16Address;
        	uint16_t u16nb_word;

        	uint8_t  vers_firmware[PARAM_FW_RELEASE_SIZE];
        }__attribute__((packed)) reply_0xfc;

    	// Write cpth, Write spec, Simu mesure
		struct {
			uint8_t  u8Status;
		}__attribute__((packed)) reply_0xec, reply_0xf4, reply_0xe1, reply_0xf0;

		// APNX : Write spec
        struct {
        	uint16_t u16Address;
        	uint16_t u16nb_word;

        	t_specialisation sSpec;
        }__attribute__((packed)) reply_0xf5;

        // APNX : Read cpth
        struct {
        	uint16_t u16Address;
        	uint16_t u16nb_word;

        	uint8_t  u8Cpth[4];
        }__attribute__((packed)) reply_0xeb;

        // Read Measure
        struct {
        	uint16_t u16Address;
        	uint16_t u16nb_word;

        	uint8_t  u8ModeFct;
        	int16_t  s16MesureSa;
        	int16_t  s16Course;
        	int16_t  s16Angle;
        	uint16_t u16AngleMot;
        }__attribute__((packed)) reply_0xf6;

        // Reglage Sa
        struct {
        	uint8_t u8ValeurDeReglage[2];
        }__attribute__((packed)) reply_0xe0;
    };
}__attribute__((packed)) t_frame_fct_PC_reply;

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
typedef enum {
	FCT_READ_CPTH           = 0xEB,
	FCT_WRITE_CPTH    		= 0xEC,
	FCT_WRITE_SN            = 0xF8,
	FCT_READ_FW_REV         = 0xFC,
	FCT_READ_SPEC           = 0xF5,
	FCT_WRITE_SPEC          = 0xF4,
	FCT_ETALONNAGE     		= 0xF0,
	FCT_READ_ETALONNAGE     = 0xF9,
	FCT_WRITE_ETALONNAGE    = 0xFA,
	FCT_READ_MEASURE    	= 0xF6,
	FCT_SIMU_MEASURE    	= 0xE1,
	FCT_REGLAGE_SA    		= 0xE0,
}t_pc_code_cmd;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern int32_t ProtocolModbusDiagnostic_Process( uint8_t u8MBAddress,
                                                  Uartframe_t *psInframe, 
                                                  Uartframe_t *psOutframe );

#endif /* __PROTOCOL_MODBUS_DIAGNOSTIC_H__ */

