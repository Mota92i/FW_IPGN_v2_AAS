//*****************************************************************************/
/// \file           protocol_msg_nmea_parser.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2022-03-11
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
//  Inclusion checking
//*****************************************************************************/
#ifndef _PROTOCOL_MSG_NMEA_PARSER_H_
#define _PROTOCOL_MSG_NMEA_PARSER_H_

//*****************************************************************************/
//  Included header file list
//*****************************************************************************/
#include "ca_prototype.h"

/******************************************************************************/
//  constant declaration
/******************************************************************************/
typedef enum{
	NMEA_MSG_INHDT 	= 0,
	NMEA_MSG_PSAG1	= 1,
	NMEA_MSG_VMVBW 	= 2,

	NMEA_MSG_COUNT
}t_nmea_msg_type;

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct
{
	uint8_t		u8Status;
	float		f32_Value;
    int16_t    	s16Value;
    int8_t    	s8Res;
    int8_t		bPrintSign;
}__attribute__((packed)) t_msg_nmea_parser;

//*****************************************************************************/
//  Multi-file-scope function prototype declarations
//*****************************************************************************/
extern void voProtocolMsgNmeaParser(t_nmea_msg_type eNmeaType, uint8_t *p_NmeaMsg, uint8_t u8NmeaMsgLen, t_msg_nmea_parser *psOutValue);
#endif // _PROTOCOL_MSG_NMEA_PARSER_H_
