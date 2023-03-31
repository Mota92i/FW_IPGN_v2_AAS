//*****************************************************************************/
/// \file           protocol_msg_nmea_parser.c
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
//  Included header file list
//*****************************************************************************/
#define public
    #include "protocol_msg_nmea_parser.h"
#undef public

#define public extern
	#include <string.h>
	#include <stdlib.h>
	#include <stdbool.h>
    #include "protocol_nmea_helper.h"
    #include "common_function.h"
#undef public

/******************************************************************************/
//  Variable declaration
/******************************************************************************/
typedef struct{
	char    *pMsgHdr;
	char    *pMsgSubStr;
	uint8_t u8HdrLen;
	uint8_t u8LenMax;
	struct{
		uint8_t u8Res;
		int16_t s16Min;
		int16_t s16Max;
	}sValue;
	int8_t bPrintSign;
}t_supported_nmea_msg;

static t_supported_nmea_msg g_sSupportedMsg[ NMEA_MSG_COUNT ] = {
    [ NMEA_MSG_INHDT ] = { "$INHDT,", ",T*", 7, 21, {0,    0, 359}, false },
    [ NMEA_MSG_PSAG1 ] = { "$PSAG1,", "*",   7, 17, {1, -999, 999}, true  },
    [ NMEA_MSG_VMVBW ] = { "$VMVBW,", ",V*", 7, 31, {1, -999, 999}, true  }
};

/******************************************************************************/
//  Function prototype declarations
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
void voProtocolMsgNmeaParser(t_nmea_msg_type eNmeaType, uint8_t *p_NmeaMsg, uint8_t u8NmeaMsgLen, t_msg_nmea_parser *psOutValue)
{
	uint8_t 			 i;
	char 				 str[10];
	float   			 f_val;
	char    			 *pChar;
	int16_t 			 s16Value;
	t_supported_nmea_msg *psCfgMsg;

    //----------------------------------------------------------------------
    // Init out Data to false
    //----------------------------------------------------------------------
	psOutValue->u8Status = false;

    //----------------------------------------------------------------------
    // Verify apnx mode supported
    //----------------------------------------------------------------------
	if( eNmeaType >= NMEA_MSG_COUNT )
		return;

    //----------------------------------------------------------------------
    // Verify the checksum
    //----------------------------------------------------------------------
	if( s32ProtocolNmeaHelper_VerifyChecksum(p_NmeaMsg) != 0 )
		return;

    //----------------------------------------------------------------------
    // Check message size
    //----------------------------------------------------------------------
	psCfgMsg = &g_sSupportedMsg[ eNmeaType ];
	if( u8NmeaMsgLen > psCfgMsg->u8LenMax )
		return;

    //----------------------------------------------------------------------
    // Check the supported message : header
    //----------------------------------------------------------------------
	if( strncmp((char *)p_NmeaMsg, psCfgMsg->pMsgHdr, psCfgMsg->u8HdrLen) != 0 )
		return;

    //----------------------------------------------------------------------
    // Check the supported message : substring
    //----------------------------------------------------------------------
	if( strstr((char *)p_NmeaMsg, psCfgMsg->pMsgSubStr) == NULL)
		return;

    //----------------------------------------------------------------------
    // Extract the value in string and check numerical value
    //----------------------------------------------------------------------
	pChar = (char *)&p_NmeaMsg[ psCfgMsg->u8HdrLen ];
	for(i = 0; i < (sizeof(str) - 1); i++)
	{
		if( (pChar[ i ] == ',') ||  (pChar[ i ] == '*') )
			break;

		// Check the char
		if( ((pChar[ i ] >= '0') && (pChar[ i ] <= '9')) || (pChar[ i ] == '.') || (pChar[ i ] == '-') || (pChar[ i ] == '+'))
			str[ i ] = pChar[ i ];
		else
			return;
	}
	str[ i ] = 0; // end of string

	if( i == 0 )
		return;

    //----------------------------------------------------------------------
    // Convert the value in float
    //----------------------------------------------------------------------
	f_val = (float)atof(str);
	for(i = 0; i < psCfgMsg->sValue.u8Res; i++)
		f_val *= 10;

	s16Value = (int16_t)cmon_s32RoundFloatToInt(f_val);

    //----------------------------------------------------------------------
    // If overflow, invalid data
    //----------------------------------------------------------------------
	if( (s16Value > psCfgMsg->sValue.s16Max) ||
		(s16Value < psCfgMsg->sValue.s16Min) )
	{
		return;
	}

    //----------------------------------------------------------------------
    // Return the value
    //----------------------------------------------------------------------
	psOutValue->u8Status   = true;

	psOutValue->f32_Value  = f_val;
	psOutValue->s16Value   = s16Value;
	psOutValue->s8Res      = psCfgMsg->sValue.u8Res;
	psOutValue->bPrintSign = psCfgMsg->bPrintSign;
}
