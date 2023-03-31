/******************************************************************************/
/// \file           common_function.c
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
//  Included header file list
/******************************************************************************/
#define public
    #include "common_function.h"
#undef public

#define public extern
    #include <stdbool.h>
#undef public

/******************************************************************************/
//  Constant definitions
/******************************************************************************/
#define MAX_STRING_LEN          (uint32_t)(512)

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/*----------------------------------------------------------------------------*/
//  CmonFct_BcdToBin
/*----------------------------------------------------------------------------*/
/// \param[in]       
/// \param[out]      
/// \return          
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
uint8_t CmonFct_ByteToHexAscii(uint8_t *byteTab, uint8_t Val)
{
    uint8_t i;

    byteTab[0] = Val >> 4;
    byteTab[1] = Val;

    for(i = 0; i < 2; i++)
    {
        byteTab[i] &= 0x0F;

        if( byteTab[i] > 9 )
            byteTab[i] += 'A' - 10;
        else
            byteTab[i] += '0';
    }
    
    return( 2 );
}

uint32_t CmonFct_HexAsciiToInt(char *hex) 
{
    uint32_t val = 0;
    uint8_t byte;
    
    while( *hex ) 
    {
        // get current character then increment
        byte = *hex++; 
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if( (byte >= '0') && (byte <= '9') ) 
        {
            byte = byte - '0';
        }
        else if( (byte >= 'a') && (byte <='f') ) 
        {
            byte = byte - 'a' + 10;
        }
        else if( (byte >= 'A') && (byte <='F') ) 
        {
            byte = byte - 'A' + 10;   
        }
        // shift 4 to make space for new digit, and add the 4 bits of the new digit 
        val = ((val << 4) & 0xFFFFFFF0) | (byte & 0xF);
    }
    
    return( val );
}

/*----------------------------------------------------------------------------*/
//  CmonFct_BcdToBin
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t BCD value
/// \param[out]     None
/// \return         uint8_t Binary value
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
uint8_t CmonFct_BcdToBin(uint8_t val)
{    
    val = (uint8_t)((uint8_t)((val >> 4) & 0x0F) * 10) + (uint8_t)(val & 0x0F);
        
    return( val );
}

/*----------------------------------------------------------------------------*/
//  CmonFct_BcdToBin
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t BCD value
/// \param[out]     None
/// \return         uint8_t Binary value
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
uint8_t CmonFct_BinToBcd(uint8_t val)
{ 
    uint8_t val1;
    uint8_t val2;
    
    val1 = (uint8_t)((uint8_t)((val / 10) << 4) & 0xF0);
    val2 = (uint8_t)((val % 10) & 0x0F);
    
    val = val1 | val2;
      
    return( val );
}

/*----------------------------------------------------------------------------*/
//  CmonFct_FileName_Get
/*----------------------------------------------------------------------------*/
/// \param[in]      char *pFileName
///                 uint8_t fileLen
/// \param[out]     uint8_t *pLen
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t CmonFct_FileName_Get(char *pFileName, uint8_t fileLen, uint8_t *pLen)
{ 
    uint32_t i;
    int32_t Error = -1;
    
    for(i = 1; i < fileLen; i++)
    {
        if( pFileName[i] == '.' )
        {
            *pLen = i;
            Error = 0;
            break;
        }
    }
      
    return( Error );
}

/*----------------------------------------------------------------------------*/
//  CmonFct_FileName_Ext_Get
/*----------------------------------------------------------------------------*/
/// \param[in]      char *pFileName
///                 uint8_t fileLen
/// \param[out]     uint8_t *pLen
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t CmonFct_FileNameExt_Get(char *pFileName, uint8_t fileLen, uint8_t *pPos)
{ 
    uint8_t u8Pos;
    int32_t Error = -1;
    
    if( CmonFct_FileName_Get(pFileName, fileLen, &u8Pos) == 0 )
    {       
        if( fileLen >= (u8Pos + 3) )
        {
            Error = 0;
            *pPos = u8Pos + 1;  // position du 1er caract�re
        }
    }
      
    return( Error );
}

/*----------------------------------------------------------------------------*/
//  CmonFct_powerOfTen
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t digit
///                 uint8_t power
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
uint32_t CmonFct_powerOfTen(uint8_t digit, uint8_t power)
{
    uint32_t val = 1;

    if( power > 0 )
    {
        while( power-- > 0)
            val = 10 * val;
    }

    return( digit * val );
}

/*----------------------------------------------------------------------------*/
//  CmonFct_NumberToString
/*----------------------------------------------------------------------------*/
/// \param[in]      uint32_t Val
///                 uint8_t len
/// \param[out]     char *pBuffer
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t CmonFct_NumberToString(uint32_t Val, char *pBuffer, uint8_t len)
{
    uint32_t Radix;
    uint8_t i;

    if( (pBuffer == NULL) || (len == 0) )
        return( -1 );
    
    if( len == 1 )
    {
        i = 0;
        pBuffer[i] = '0' + (uint8_t)Val;
        i++;
    }
    else
    {
        for(i = 0; i < len; i++)
        {
            Radix = CmonFct_powerOfTen(1, (len - 1) - i);
            pBuffer[i] = '0' + (Val / Radix);
            Val = Val % Radix;
        }
    }
    
    pBuffer[i] = 0;
    
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
/// \date           
///                 
//----------------------------------------------------------------------------*/
void RemoveZeroFromStringNumber(char *pBuffer)
{
    uint8_t NbDigit, Pos, i;

    NbDigit = 0;
    while( pBuffer[NbDigit] != 0 )
        NbDigit++;

    if( NbDigit > 0 )
    {
        Pos = 0;
        while( pBuffer[Pos] == '0' )
            Pos++;

        if( Pos > 0 )
        {
            for(i = 0; i < ((NbDigit - Pos) + 1); i++)
                pBuffer[i] = pBuffer[Pos + i];
        }
    }
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///                 
//----------------------------------------------------------------------------*/
int32_t CmonFct_NumberToDecimalString(int32_t Valeur, uint8_t NbreDecimal, uint8_t bPartieEntiereDecimal, char *Txt)
{
    uint8_t Digit, ucTmp, Index, FlagDigit;
    uint32_t MesureAbsolue, ushortTmp;
    const uint32_t Div[] = {    1,
                                10,
                                100,
                                1000,
                                10000,
                                100000,
                                1000000,
                                10000000,
                                100000000,
                                1000000000 };
    const uint8_t DIV_ARRAY_SZE = sizeof(Div) / sizeof(Div[0]);
    
    if( bPartieEntiereDecimal == false )
    {
        if( NbreDecimal == 1 )
        {
            if( (Valeur > 999999999) || (Valeur < -999999999) )
            {
                Valeur = (int32_t)(Valeur / 10);
                NbreDecimal = 0;
            }
        }
        else if( NbreDecimal >= 2 )
        {
            NbreDecimal = NbreDecimal - 1;
            if( NbreDecimal > DIV_ARRAY_SZE )
                NbreDecimal = DIV_ARRAY_SZE;

            Valeur = (int32_t)(Valeur / Div[NbreDecimal]);
            NbreDecimal = 1;
        }
    }

    Index = 0;
    MesureAbsolue = (uint32_t)Valeur;
//    Txt[Index] = '+';
    if( Valeur < 0 )
    {
        MesureAbsolue = (uint32_t)(~Valeur + 1);
        Txt[Index] = '-';
        Index++;
    }

    ushortTmp = 1000000000;
    FlagDigit = 0;
    for(Digit = 1; Digit <= 10; Digit++)
    {
        // Hexa vers BCD
        ucTmp = (unsigned char)(MesureAbsolue / ushortTmp);
        MesureAbsolue %= ushortTmp;
        ushortTmp /= 10;
                
        if( (ucTmp > 0) || (FlagDigit == 1) || (NbreDecimal == (10 - Digit)) )
        {
            FlagDigit = 1;
            Txt[Index] = '0' + ucTmp;	Index++;
                                    
            if( NbreDecimal != 0 )
            {
                if( NbreDecimal == (10 - Digit) )
                {
                    Txt[Index] = '.';	
                    Index++;
                }
            }
        }
    }

    Txt[Index] = '\0';

    return(Index);
}

/*----------------------------------------------------------------------------*/
//  CmonFct_TimeOut
/*----------------------------------------------------------------------------*/
/// \param[in]      uint32_t new_val
///                 uint32_t old_val
///                 uint32_t comp_value
/// \return         true for timeout
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t CmonFct_TimeOut(uint32_t new_val, uint32_t old_val, uint32_t comp_value)
{
    int32_t ret = true;
      
    if( new_val > old_val )
    {
        if( (new_val - old_val) < comp_value )
        {
            ret = false;
        }
    }
    
    return( ret );
}

/*----------------------------------------------------------------------------*/
//  
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t *PtrSrc
///                 uint8_t Sze
/// \return         Nono
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
void CmonFct_SwapLittleBigEndian(uint8_t *PtrSrc, uint8_t *PtrDest, uint8_t Sze)
{	
    unsigned char Buf[10], sveSze, Index;

    sveSze = Sze;
    Index = 0;
    while( Sze > 0 )
        Buf[Index++] = PtrSrc[--Sze];

    for(Sze = 0; Sze < sveSze; Sze++)
        PtrDest[Sze] = Buf[Sze];
}

/*----------------------------------------------------------------------------*/
//  
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t *PtrSrc
///                 uint8_t Sze
/// \return         Nono
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
void CmonFct_SwapLittleBigEndianVar(uint8_t *PtrSrc, uint8_t Sze)
{	
	unsigned char Buf[6], sveSze, Index;

	sveSze = Sze;
	Index = 0;
	while( Sze > 0 )
		Buf[Index++] = PtrSrc[--Sze];
	
	for(Sze = 0; Sze < sveSze; Sze++)
		PtrSrc[Sze] = Buf[Sze];
}

/*----------------------------------------------------------------------------*/
//  
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t *PtrSrc
///                 uint8_t Sze
/// \return         Nono
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
uint32_t CmonFct_StrLen(const char *p_str)
{
    uint32_t u32len;

    u32len = CmonFct_StrLenN( p_str, MAX_STRING_LEN );

    return( u32len );
}

uint32_t CmonFct_StrLenN(const char *p_str, uint32_t u32LenMax)
{
    const char *p_str_len;
    uint32_t   u32len;


    p_str_len = p_str;
    u32len    = 0u;
    while( ( p_str_len != (const char *)  0 ) &&
           (*p_str_len != (      char  )'\0') &&
           ( u32len    <  (    uint32_t)u32LenMax) ) 
    {
        p_str_len++;
        u32len++;
    }

    return (u32len);
}

/*----------------------------------------------------------------------------*/
//  
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t *PtrSrc
///                 uint8_t Sze
/// \return         Nono
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t CmonFct_StrCmpN( const char *p_Str1, uint8_t u8Str1LenMax,
                         const char *p_Str2, uint8_t u8Str2LenMax)
{
    uint8_t i;
    uint8_t u8Str1Len;
    uint8_t u8Str2Len;
    int32_t Error = 0;
    
    u8Str1Len = CmonFct_StrLenN(p_Str1, u8Str1LenMax);
    u8Str2Len = CmonFct_StrLenN(p_Str2, u8Str2LenMax); 
    
    if( u8Str1Len != u8Str2Len )
    {
        Error = -1;
    }
    else
    {
        for(i = 0; i < u8Str1LenMax; i++)
        {
            if( p_Str1[i] != p_Str2[i] )
            {
                Error = -1;
                break;
            }
        }
    }
    
    return( Error );
}

/*----------------------------------------------------------------------------*/
//  
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t *PtrSrc
///                 uint8_t Sze
/// \return         Nono
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
uint8_t CmonFct_IpAdrLongToString(uint8_t *strTab, uint32_t IpAdr)
{
    uint8_t Val, i, j;
    const uint8_t IpAdrTab[]    = {24,   16, 8, 0};
    const uint8_t PosStoreTab[] = {0,     4, 8, 12};
    const uint8_t DigitsTab[]   = {100,  10, 1};

    for(j = 0; j < sizeof(IpAdrTab); j++)
    {
        Val = (uint8_t)((IpAdr >> IpAdrTab[j]) & 0xFF);

        for(i = 0; i < sizeof(DigitsTab); i++)
        {
                strTab[PosStoreTab[j] + i] = '0' + (uint8_t)(Val / DigitsTab[i]);
                Val %= DigitsTab[i];
        }
        strTab[PosStoreTab[j] + i] = '.';
    }

    strTab[15] = 0;

    return( 15 );
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///                 
//----------------------------------------------------------------------------*/
inline __attribute__((always_inline)) void cmon_U16ToU8ArrayMSB(uint16_t u16Val, uint8_t * const p_Byte)
{
    uint8_t *pu8Val = (uint8_t *)&u16Val;
    
    p_Byte[ 0 ] = pu8Val[ 1 ];
    p_Byte[ 1 ] = pu8Val[ 0 ];
}
inline __attribute__((always_inline)) void cmon_U16ToU8ArrayLSB(uint16_t u16Val, uint8_t * const p_Byte)
{
    uint8_t *pu8Val = (uint8_t *)&u16Val;
    
    p_Byte[ 0 ] = pu8Val[ 0 ];
    p_Byte[ 1 ] = pu8Val[ 1 ];
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///                 
//----------------------------------------------------------------------------*/
inline __attribute__((always_inline)) void cmon_U32ToU8ArrayMSB(uint32_t u32Val, uint8_t * const p_Byte)
{
    uint8_t *pu8Val = (uint8_t *)&u32Val;
    
    p_Byte[ 0 ] = pu8Val[ 3 ];
    p_Byte[ 1 ] = pu8Val[ 2 ];
    p_Byte[ 2 ] = pu8Val[ 1 ];
    p_Byte[ 3 ] = pu8Val[ 0 ];
}
inline __attribute__((always_inline)) void cmon_U32ToU8ArrayLSB(uint32_t u32Val, uint8_t * const p_Byte)
{
    uint8_t *pu8Val = (uint8_t *)&u32Val;
      
    p_Byte[ 0 ] = pu8Val[ 0 ];
    p_Byte[ 1 ] = pu8Val[ 1 ];
    p_Byte[ 2 ] = pu8Val[ 2 ];
    p_Byte[ 3 ] = pu8Val[ 3 ];
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///                 
//----------------------------------------------------------------------------*/
inline __attribute__((always_inline)) uint32_t cmon_U8ArrayToU32MSB(uint8_t * const p_Byte)
{
    uint32_t u32Val;
    uint8_t  *pu8Val = (uint8_t *)&u32Val;
    
    pu8Val[ 0 ] = p_Byte[ 3 ];
    pu8Val[ 1 ] = p_Byte[ 2 ];
    pu8Val[ 2 ] = p_Byte[ 1 ];
    pu8Val[ 3 ] = p_Byte[ 0 ];
    
    return( u32Val );
}

inline __attribute__((always_inline)) uint32_t cmon_U8ArrayToU32LSB(uint8_t * const p_Byte)
{
    uint32_t u32Val;
    uint8_t  *pu8Val = (uint8_t *)&u32Val;
    
    pu8Val[ 0 ] = p_Byte[ 0 ];
    pu8Val[ 1 ] = p_Byte[ 1 ];
    pu8Val[ 2 ] = p_Byte[ 2 ];
    pu8Val[ 3 ] = p_Byte[ 3 ];
    
    return( u32Val );
}

inline __attribute__((always_inline)) uint16_t cmon_U8ArrayToU16MSB(uint8_t * const p_Byte)
{
    uint16_t u16Val;
    uint8_t  *pu8Val = (uint8_t *)&u16Val;
    
    pu8Val[ 0 ] = p_Byte[ 1 ];
    pu8Val[ 1 ] = p_Byte[ 0 ];
    
    return( u16Val );
}

inline __attribute__((always_inline)) uint16_t cmon_U8ArrayToU16LSB(uint8_t * const p_Byte)
{
    uint16_t u16Val;
    uint8_t  *pu8Val = (uint8_t *)&u16Val;
    
    pu8Val[ 0 ] = p_Byte[ 0 ];
    pu8Val[ 1 ] = p_Byte[ 1 ];
    
    return( u16Val );
}

//----------------------------------------------------------------------------*/
//  
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///                 
//----------------------------------------------------------------------------*/
inline __attribute__((always_inline)) uint32_t cmon_U8ToU32(uint8_t Byte0, uint8_t Byte1, uint8_t Byte2, uint8_t Byte3)
{
    union{
        uint8_t  u8byte[4];
        uint32_t u32Data;
    }sVal;

    sVal.u8byte[0] = Byte3;
    sVal.u8byte[1] = Byte2;
    sVal.u8byte[2] = Byte1;
    sVal.u8byte[3] = Byte0;

    return( sVal.u32Data );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
int32_t cmon_s32RoundFloatToInt(float x)
{
	int32_t s32Val = (int32_t)x;

	 if( (x - s32Val) >= 0.5 )
		 s32Val++;
	 if( (x - s32Val) < -0.5 )
		 s32Val--;

	 return( s32Val );
}
