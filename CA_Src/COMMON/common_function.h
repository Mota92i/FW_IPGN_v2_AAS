/******************************************************************************/
/// \file           common_function.h
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
#ifndef __COMMON_FUNCTION_H__
#define __COMMON_FUNCTION_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include <stdio.h>

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern uint8_t  CmonFct_ByteToHexAscii(uint8_t *byteTab, uint8_t Val);
extern uint32_t CmonFct_HexAsciiToInt(char *hex);
extern uint8_t  CmonFct_BcdToBin(uint8_t val);
extern uint8_t  CmonFct_BinToBcd(uint8_t val);
extern int32_t  CmonFct_FileName_Get(char *pFileName, uint8_t fileLen, uint8_t *pLen);
extern int32_t  CmonFct_FileNameExt_Get(char *pFileName, uint8_t fileLen, uint8_t *pPos);
extern uint32_t CmonFct_powerOfTen(uint8_t digit, uint8_t power);
extern int32_t  CmonFct_NumberToString(uint32_t Val, char *pBuffer, uint8_t len);
extern int32_t  CmonFct_TimeOut(uint32_t new_val, uint32_t old_val, uint32_t comp_value);
extern void     CmonFct_SwapLittleBigEndianVar(uint8_t *PtrSrc, uint8_t Sze);
extern void     CmonFct_SwapLittleBigEndian(uint8_t *PtrSrc, uint8_t *PtrDest, uint8_t Sze);
extern uint32_t CmonFct_StrLen(const char *p_str);
extern uint32_t CmonFct_StrLenN(const char *p_str, uint32_t u32LenMax);
extern uint8_t  CmonFct_IpAdrLongToString(uint8_t *strTab, uint32_t IpAdr);
extern int32_t  CmonFct_NumberToDecimalString(int32_t Valeur, uint8_t NbreDecimal, uint8_t bPartieEntiereDecimal, char *Txt);
extern uint32_t CmonFct_GetTimeElapsed(uint32_t u32TickBefore);
extern uint32_t CmonFct_GetTickElapsed(uint32_t u32TickStart, uint32_t u32TickStop);
extern uint32_t CmonFct_GetTickElapsedNow(uint32_t u32TickBefore);
extern int32_t  CmonFct_StrCmpN( const char *p_Str1, uint8_t u8Str1LenMax,
                                 const char *p_Str2, uint8_t u8Str2LenMax );

extern uint32_t cmon_U8ArrayToU32MSB(uint8_t * const p_Byte);
extern uint32_t cmon_U8ArrayToU32LSB(uint8_t * const p_Byte);
extern uint16_t cmon_U8ArrayToU16MSB(uint8_t * const p_Byte);
extern uint16_t cmon_U8ArrayToU16LSB(uint8_t * const p_Byte);

extern void     cmon_U16ToU8ArrayMSB(uint16_t u16Val, uint8_t * const p_Byte);
extern void     cmon_U16ToU8ArrayLSB(uint16_t u16Val, uint8_t * const p_Byte);
extern void     cmon_U32ToU8ArrayMSB(uint32_t u32Val, uint8_t * const p_Byte);
extern void     cmon_U32ToU8ArrayLSB(uint32_t u32Val, uint8_t * const p_Byte);
extern uint32_t cmon_U8ToU32(uint8_t Byte0, uint8_t Byte1, uint8_t Byte2, uint8_t Byte3);

extern int32_t 	cmon_s32RoundFloatToInt(float x);
#endif /* __COMMON_FUNCTION_H__ */

