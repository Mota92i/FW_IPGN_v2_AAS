/******************************************************************************/
/// \file           drv_eeprom.c
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
    #include "drv_i2c.h"
#undef public

#define public extern
    #include "fsl_i2c.h"
#undef public

/******************************************************************************/
//  Constant declaration
/******************************************************************************/
#define I2C_RETRY_MAX 		(5)

/******************************************************************************/
//  Function definitions
/******************************************************************************/
static int32_t DrvI2c_ReadWrite(bool ReadOrWrite, uint8_t u8SlaveAddress, I2CConfig_t *sI2CConfig);

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/*----------------------------------------------------------------------------*/
//  DrvI2c_ReadWrite
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t u8SlaveAddress
///                 bool ReadOrWrite
///                 I2CConfig_t *sI2CConfig
/// \param[out]     I2CConfig_t *sI2CConfig
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
static int32_t DrvI2c_ReadWrite(bool ReadOrWrite, uint8_t u8SlaveAddress, I2CConfig_t *sI2CConfig)
{
    int32_t 				ErrReturn = -1;
    uint8_t 				retry = I2C_RETRY_MAX;
    i2c_master_transfer_t 	masterXfer;
    status_t				status;

    while( (retry > 0) && (ErrReturn == -1) )
	{
		memset(&masterXfer, 0, sizeof(masterXfer));
		if( ReadOrWrite == true )
			masterXfer.direction = kI2C_Read;
		else
			masterXfer.direction = kI2C_Write;
		masterXfer.slaveAddress 	= u8SlaveAddress;
		masterXfer.subaddress 		= sI2CConfig->u32Cmd;
		masterXfer.subaddressSize 	= sI2CConfig->u8CmdLen;
		masterXfer.data 			= sI2CConfig->pDest;
		masterXfer.dataSize 		= sI2CConfig->u32DestLen;
		masterXfer.flags            = kI2C_TransferDefaultFlag;
		status = I2C_MasterTransferBlocking(I2C1, &masterXfer);

		if( status == kStatus_Success )
		{
			ErrReturn = 0;
		}
		else
		{
			I2C_MasterStop(I2C1);
			retry--;
		}
	}

    return( ErrReturn );
}

/*----------------------------------------------------------------------------*/
//  DrvI2c_Read
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t u8SlaveAddress
/// \param[out]     I2CConfig_t *sI2CConfig
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t DrvI2c_Write(uint8_t u8SlaveAddress, I2CConfig_t *sI2CConfig)
{    
    return( DrvI2c_ReadWrite(false, u8SlaveAddress, sI2CConfig) );
}

/*----------------------------------------------------------------------------*/
//  DrvI2c_Read
/*----------------------------------------------------------------------------*/
/// \param[in]      uint8_t u8SlaveAddress
///                 I2CConfig_t *sI2CConfig
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t DrvI2c_Read(uint8_t u8SlaveAddress, I2CConfig_t *sI2CConfig)
{   
    return( DrvI2c_ReadWrite(true, u8SlaveAddress, sI2CConfig) );
}
