/******************************************************************************/
/// \file           protocol_nmea_helper.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2023-08-23
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
    #include "protocol_nmea_helper.h"
#undef public

#define public extern
    #include <string.h>
    #include "common_function.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static uint8_t u8ProtocolNmeaHelper_GetChecksum(char *p_msg, uint8_t u8Len);

/******************************************************************************/
//  constant declaration
/******************************************************************************/


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
static uint8_t u8ProtocolNmeaHelper_GetChecksum(char *p_msg, uint8_t u8Len)
{
    uint8_t u8Chk = 0;
    uint8_t i;
    
    for(i = 0; i < u8Len; i++)
    {
        u8Chk ^= (uint8_t)p_msg[i];
    }
    
    return( u8Chk );
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
int32_t s32ProtocolNmeaHelper_VerifyChecksum(uint8_t *p_msg)
{
	int32_t Error = 0;
	uint8_t u8Chk;
	char    AsciiChk[3];
	uint8_t u8FrameChk;
	char *pMsgStar;
    
	pMsgStar = strchr((char *)p_msg, '*');

	// Compute chk
	u8Chk = u8ProtocolNmeaHelper_GetChecksum((char *)&p_msg[1], (pMsgStar - (char *)p_msg - 1));

	// Get the frame chk
	memcpy(&AsciiChk[0], &pMsgStar[1], 2);
	AsciiChk[ 2 ] = 0;
	u8FrameChk  = (uint8_t)CmonFct_HexAsciiToInt(&AsciiChk[0]);

	if( u8Chk != u8FrameChk )
		Error = -1;

    return( Error );
}
