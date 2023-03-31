/******************************************************************************/
/// \file           drv_mlx90316.h
/// \version        1.0
/// \author         Aseervatham
/// \date           2021-09-03
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
    #include "drv_mlx90316.h"
#undef public

#define public extern
	#include "peripherals.h"
	#include "fsl_dspi.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void voDelay_60us(void);
static void voDSPI_MasterInit(void);

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-09-03
///
//----------------------------------------------------------------------------*/
static void voDelay_60us(void)
{
	SDK_DelayAtLeastUs(60, SystemCoreClock);
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-09-03
///
//----------------------------------------------------------------------------*/
static void voDSPI_MasterInit(void)
{
	dspi_master_config_t sMasterConfig;

	DSPI_MasterGetDefaultConfig(&sMasterConfig);
	sMasterConfig.ctarConfig.cpha						= kDSPI_ClockPhaseSecondEdge;
	sMasterConfig.ctarConfig.cpol						= kDSPI_ClockPolarityActiveHigh;
	sMasterConfig.ctarConfig.baudRate					= 100000UL;	// 145khz max
	sMasterConfig.ctarConfig.pcsToSckDelayInNanoSec 	= 10000UL;	// t6 = min =6.9us
	sMasterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 10000UL;	// t4 = min =6.9us

	DSPI_MasterInit(SPI0_PERIPHERAL, &sMasterConfig, SPI0_CLK_FREQ);
}

//----------------------------------------------------------------------------*/
// Output data :
//     - Error      : -1
//     - Data valid : 0 <= Angle <= 3599°
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2021-09-03
///
//----------------------------------------------------------------------------*/
int32_t s32DrvMlx90316_ReadAngle(void)
{
	const uint8_t   	bufTx[ 10 ] = { 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t   			bufRx[ 10 ];
	uint8_t 			u8ValRx;
	uint8_t				u8ValTx;
    dspi_transfer_t 	sMasterXfer;
    uint16_t			u16RR;
    uint16_t			u16CC;
    int8_t				i;
    int32_t 			s32Angle;
    int8_t 			Error;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
    voDSPI_MasterInit();

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
    sMasterXfer.txData 		= &u8ValTx;
    sMasterXfer.rxData 		= &u8ValRx;
    sMasterXfer.dataSize 	= 1;
	sMasterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterActiveAfterTransfer;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	Error = 0;
	for(i = 0; i < (sizeof(bufTx) / sizeof(bufTx[0])); i++)
	{
		// Last data
		if( i == ((sizeof(bufTx) / sizeof(bufTx[0])) - 1) )
			sMasterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

		// Read data
		u8ValTx = bufTx[ i ];
		if( DSPI_MasterTransferBlocking(SPI0, (dspi_transfer_t *)&sMasterXfer) == kStatus_Success )
			bufRx[ i ] = u8ValRx;
		else
			Error = -1 ;

		// Delay
		voDelay_60us();
	}

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	s32Angle = -1 ;
	if( Error == 0 )
	{
		u16RR = (uint16_t)((bufRx[2] << 8) + bufRx[3]); // receive 1. byte of mlx data
		u16CC = (uint16_t)((bufRx[4] << 8) + bufRx[5]); // receive 1. byte of mlx data (inverted)

		if( ((uint16_t)(u16RR + u16CC + 1) == 0) &&		// Checksum
			((u16RR & 0x03) == 1) )						// Valid data angle
		{
			u16RR = u16RR >> 2;							// Output data is 14bits width

			// u32Val * 3600 / (2^14)
			s32Angle = u16RR * 3600;
			// Round
			if( s32Angle & 0x2000 )
				s32Angle += 0x4000;
			s32Angle = s32Angle >> 14;
			s32Angle = 3600 - s32Angle;
			if( s32Angle >= 3600 )
				s32Angle = 0;
		}
	}

	return( s32Angle );
}


