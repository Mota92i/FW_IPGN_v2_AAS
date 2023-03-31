/******************************************************************************/
/// \file           crc16.h
/// \version        1.0
/// \author         
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
#ifndef _CRC16_H_
#define _CRC16_H_

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern unsigned short CRC16_calc(unsigned short crc16,unsigned char car);
extern unsigned short CRC16_ComputeOnBuffer(unsigned char *pBuf, unsigned short u16Length);

/******************************************************************************/

#endif /* _CRC16_PUBLIC_H_ */
