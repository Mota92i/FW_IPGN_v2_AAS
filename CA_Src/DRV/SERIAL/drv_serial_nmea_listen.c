/******************************************************************************/
/// \file           drv_serial_nmea_listen.c
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
    #include "drv_serial_nmea_listen_private.h"
#undef public

#define public extern
	#include "drv_bsp_uart_private.h"
    #include "drv_serial_uart_private.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void voSerial_cb( t_uart_custom * p_args, uint8_t u8Status );
static void voTimer_cb(t_uart_custom * p_args);

static void voGetTimeout( serial_config_t  const * const psSerialCfg,
                          uint32_t		   * const pTimeOutEOF,
                          uint32_t 		   * const pTimeOutFifo );

/******************************************************************************/
//  Function definitions
/******************************************************************************/


/******************************************************************************/
//  Call back function : UART
/******************************************************************************/
static void voSerial_cb(t_uart_custom * p_args, uint8_t u8Status)
{
    uint32_t u32Timeout;

    if( u8Status == DRV_UART_RX_RECEIVE )
    {
		if( p_args->sRxTxBuf.sRx.u16Idx >= 1 )
		{
			// String starts by '$'
			if( p_args->sRxTxBuf.sRx.u8Buf[ 0 ] != '$' )
			{
				DrvBspUart_StopTimer(p_args);

				p_args->sRxTxBuf.sRx.u16Idx = 0;
			}
			else
			{
				if( (p_args->sRxTxBuf.sRx.u16Idx >= 6)									  &&
					(p_args->sRxTxBuf.sRx.u8Buf[p_args->sRxTxBuf.sRx.u16Idx - 5] == '*')  &&	// star before checksum
					(p_args->sRxTxBuf.sRx.u8Buf[p_args->sRxTxBuf.sRx.u16Idx - 2] == 0x0D) &&	// Check for CR char
					(p_args->sRxTxBuf.sRx.u8Buf[p_args->sRxTxBuf.sRx.u16Idx - 1] == 0x0A) )		// Check for LF char
				{
					DrvBspUart_StopTimer(p_args);

			    	p_args->sArgApp.sMsgQueue.u8IdMsg = DRV_UART_RX_RECEIVE;
			    	p_args->sArgApp.p_App_Cb(&p_args->sArgApp.sMsgQueue);
				}
				else
				{
					// There is no more data in the fifo, launch the timer
					u32Timeout = DrvBspUart_GetModBusTimeout(p_args);
					// Set and restart the timer
					DrvBspUart_SetTimeAndStartTimer(p_args, u32Timeout);
				}
			}
		}
    }
}

/******************************************************************************/
//  Call back function : TIMER
/******************************************************************************/
static void voTimer_cb(t_uart_custom * p_args)
{
    // Stop the timer
	DrvBspUart_StopTimer(p_args);

	// Error on the nmea frame
	p_args->sRxTxBuf.sRx.u16Idx = 0;
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
static void voGetTimeout( serial_config_t  const * const psSerialCfg,
                          uint32_t		   * const pTimeOutEOF,
                          uint32_t 		   * const pTimeOutFifo )
{
    uint8_t u8BitLength;
    uint32_t u32Value;
    
    // UART START BIT
    u8BitLength = 1;

    // UART DATA LENGTH   
    if( (psSerialCfg->u8DataBits == 7) || (psSerialCfg->u8DataBits == 9) )
    	u8BitLength += psSerialCfg->u8DataBits;
    else
    	u8BitLength += 8;
    
    // UART STOP BIT
    if( psSerialCfg->u8StopBits == SERIAL_STOP_BITS_2 )
    	u8BitLength += 2;
    else
    	u8BitLength += 1;
    
    // UART PARITY
    if( psSerialCfg->u8Parity != 0 )
        u8BitLength += 1;

    // In microseconde * 2 character for EOF
    u32Value = ((uint32_t)2000000 * u8BitLength) / psSerialCfg->u32BaudRate;
    *pTimeOutEOF = u32Value;
    
    // In microseconde * 1.5 interframe
    u32Value 	  = (uint32_t)15000000 / psSerialCfg->u32BaudRate;
    *pTimeOutFifo = *pTimeOutEOF - u32Value;
}

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
int32_t DrvSerialNmeaListen_Open( uint8_t  		u8Channel,
						   	      serial_config_t  *psSerialCfg,
								  void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer),
								  t_uart_xfer **p_GetTheBuffer )
{
    int32_t ret;
    uint32_t TimeOutModbusEOF;
    uint32_t TimeOutFifo;

    //--------------------------------------------------------------------------
    // Open port
    //--------------------------------------------------------------------------    
    ret = DrvSerial_Open( u8Channel,
    					  psSerialCfg,
                          voSerial_cb,
                          voTimer_cb,
                          pIrq_App_cb,
                          p_GetTheBuffer );

    //--------------------------------------------------------------------------
    // Compute timeout
    //--------------------------------------------------------------------------
    voGetTimeout( psSerialCfg, &TimeOutModbusEOF, &TimeOutFifo );
    DrvSerial_SetTimeOut(u8Channel, TimeOutModbusEOF, TimeOutFifo);

    return( ret );
}

inline __attribute__((always_inline)) int32_t DrvSerialNmeaListen_Close(uint8_t u8Channel)
{
    return( DrvSerial_Close(u8Channel) );
}

inline __attribute__((always_inline)) int32_t DrvSerialNmeaListen_Write(uint8_t  u8Channel, uint8_t *pDataTx, uint16_t u16LengthTx)
{   
    return( DrvSerial_Write(u8Channel, pDataTx, u16LengthTx) );
}

inline __attribute__((always_inline)) int32_t DrvSerialNmeaListen_WriteInProgress(uint8_t u8Channel)
{
    return( DrvSerial_WriteInProgress(u8Channel) );
}

inline __attribute__((always_inline)) int32_t DrvSerialNmeaListen_ReceiveInProgress(uint8_t u8Channel)
{
    return( DrvSerial_ReceiveInProgress(u8Channel) );
}

inline __attribute__((always_inline)) uint32_t DrvSerialNmeaListen_GetData(uint8_t Channel, uint8_t GetInfo)
{
    return( DrvSerial_GetData(Channel, GetInfo) );
}
