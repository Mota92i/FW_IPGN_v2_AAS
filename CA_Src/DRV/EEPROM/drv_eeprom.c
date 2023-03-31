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
    #include "drv_eeprom.h"
#undef public

#define public extern
	#include <string.h>
    #include "drv_i2c.h"
    #include "os_semaphore.h"
    #include "os_delay.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define BIT_PER_PAGE	                (6)
#define	BYTES_PER_PAGE	                ( (uint32_t)((uint32_t)0x0001 << BIT_PER_PAGE) )
#define	PAGE_MASK	                	( (uint32_t)( BYTES_PER_PAGE - 1 ) )

#define	MSB_ADDRESS_MASK                ((uint32_t)0x010000)
#define	MSB_ADDRESS_A16                 ((uint8_t)0x02)

#define INTERNAL_WRITE_CYCLE_DURATION   (6)  // en ms

#define I2C_SLAVE_ADDRESS               ((uint8_t)0xA0)

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/*----------------------------------------------------------------------------*/
//  DrvEeprom_Read
/*----------------------------------------------------------------------------*/
/// \param[in]      u32Adr    : Address to read data
///                 pDest     : Buffer dest
///                 u8NbRead : Number of data to read
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t DrvEeprom_Read(uint32_t u32Adr, uint8_t *pDest, uint32_t u32NbRead, uint8_t u8Retry)
{
    int32_t  Status = -1;
    uint8_t SlaveAddress;
    I2CConfig_t sI2cCfg;
    uint8_t u8Len;
    uint8_t retry;
    int err;
    
    if( u32NbRead == 0 )
        return 0;

    if( u8Retry > 10 )
        u8Retry = 10;
 
    if( u8Retry == 0 )
        u8Retry = 1;

    voOsSemaphore_EepromTake();
    err = 0;
    if( err == 0 )
    { 
        Status = 0;
        while( u32NbRead > 0 )
        {
            if( u32NbRead > 250 )
                u8Len = 255;
            else
                u8Len = u32NbRead;
                  
            //----------------------------------------------------------------------
            // The MSB A16 is hold in slave address
            //----------------------------------------------------------------------
            SlaveAddress = I2C_SLAVE_ADDRESS;
            if( u32Adr & (uint32_t)MSB_ADDRESS_MASK )
                SlaveAddress |= MSB_ADDRESS_A16;

            //----------------------------------------------------------------------
            // 
            //----------------------------------------------------------------------
            SlaveAddress = (uint8_t)((uint8_t)SlaveAddress >> 1);
            sI2cCfg.u8CmdLen   = 2;
            sI2cCfg.u32Cmd 	   = u32Adr;
            sI2cCfg.u32DestLen = u8Len;
            sI2cCfg.pDest 	   = pDest;
            
            for(retry = 0; retry < u8Retry; retry++)
            {
                Status = DrvI2c_Read(SlaveAddress, &sI2cCfg);
                
                if( Status == 0 )
                    break;
                else
                	voOsDelay_Delay(100);
            }
            
            if( Status == - 1 )
                break;
            
            //----------------------------------------------------------------------
            // 
            //----------------------------------------------------------------------
            u32NbRead -= u8Len;
            pDest     += u8Len;
            u32Adr    += u8Len;
        }
        
        voOsSemaphore_EepromGive();
        err = 0;
    }

    return( Status );
}

/*----------------------------------------------------------------------------*/
//  DrvEeprom_Write
/*----------------------------------------------------------------------------*/
/// \param[in]      u32Adr    : Address to store data
///                 pSrc      : Buffer source
///                 u8NbWrite : Number of data to write
/// \param[out]     None
/// \return         (-1) if error else 0
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
int32_t DrvEeprom_Write(uint32_t u32Adr, uint8_t *pSrc, uint32_t u32NbWrite, uint8_t u8Retry)
{
    int32_t  Status = 0;
    uint32_t NbBloc;
    uint32_t CurrentAdress;
    uint32_t Page, PageStart, PageEnd;
    uint32_t AdrEeprom;
    I2CConfig_t sI2cCfg;
    uint8_t SlaveAddress;
    uint8_t retry;
    
    if( u32NbWrite == 0 )
        return 0;

    if( u8Retry > 10 )
        u8Retry = 10;
        
    if( u8Retry == 0 )
        u8Retry = 1;
        
    // Put semaphore if needed

    //--------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------
    PageStart  = u32Adr;
    PageEnd    = u32Adr + u32NbWrite - 1 ;   
    PageStart  = PageStart >> BIT_PER_PAGE;
    PageEnd    = PageEnd >> BIT_PER_PAGE;	
    CurrentAdress = u32Adr & PAGE_MASK;

    for(Page = PageStart; Page <= PageEnd; Page++)
    {
        NbBloc = BYTES_PER_PAGE - CurrentAdress;
        if( NbBloc > u32NbWrite )
        {
            NbBloc = u32NbWrite;
        }
        
        AdrEeprom = (uint32_t)((uint32_t)Page << BIT_PER_PAGE) | (uint32_t)CurrentAdress;
        
        //----------------------------------------------------------------------
        // The MSB A16 is hold in slave address
        //----------------------------------------------------------------------
        SlaveAddress = I2C_SLAVE_ADDRESS;
        if( AdrEeprom & (uint32_t)MSB_ADDRESS_MASK )
            SlaveAddress |= MSB_ADDRESS_A16;
          
        //----------------------------------------------------------------------
        // Call I2c interface to access eeprom
        //---------------------------------------------------------------------- 
        SlaveAddress = (uint8_t)((uint8_t)SlaveAddress >> 1);
        sI2cCfg.u8CmdLen   = 2;
        sI2cCfg.u32Cmd 	   = AdrEeprom;
        sI2cCfg.u32DestLen = NbBloc;
        sI2cCfg.pDest 	   = &pSrc[0];
        
        for(retry = 0; retry < u8Retry; retry++)
        {
            Status = DrvI2c_Write(SlaveAddress, &sI2cCfg);
            
            if( Status == 0 )
                break;
            else
            	voOsDelay_Delay(100);
        }
        
        if( Status == - 1 )
            break;
        
        //----------------------------------------------------------------------
        // Update data
        //----------------------------------------------------------------------  
        pSrc += NbBloc;
        u32NbWrite -= NbBloc;
        CurrentAdress = 0;

        //----------------------------------------------------------------------
        // Let the eeprom to write data when changing the page
        //----------------------------------------------------------------------  
        voOsDelay_Delay(INTERNAL_WRITE_CYCLE_DURATION);
    }

    // Release semaphore if needed

    return(Status);
}
