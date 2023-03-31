/******************************************************************************/
/// \file           drv_i2c.h
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
#ifndef __DRV_I2C_H__
#define __DRV_I2C_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  Type declarations
/******************************************************************************/
typedef struct  {
    uint32_t u32Cmd;
    uint8_t  u8CmdLen;
    uint8_t *pDest;
    uint32_t u32DestLen;
} I2CConfig_t;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern int32_t DrvI2c_Write(uint8_t u8SlaveAddress, I2CConfig_t *sI2CConfig);
extern int32_t DrvI2c_Read(uint8_t u8SlaveAddress, I2CConfig_t *sI2CConfig);

#endif /* __DRV_I2C_H__ */
