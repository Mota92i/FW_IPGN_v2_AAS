/******************************************************************************/
/// \file           drv_serial_modbus_rtu.c
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
    #include "drv_serial_modbus_rtu_private.h"
#undef public

#define public extern
	#include "drv_bsp_uart_private.h"
    #include "drv_serial_uart_private.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void voSerialModbus_cb( t_uart_custom * p_args, uint8_t u8Status );
static void voTimerModbus_cb ( t_uart_custom * p_args );

static void voModbusTimeout( serial_config_t  const * const psSerialCfg,
                             uint32_t		  * const pTimeOutModbusEOF,
                             uint32_t 		  * const pTimeOutFifo );

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/******************************************************************************/
//  Call back function : UART
/******************************************************************************/
static void voSerialModbus_cb(t_uart_custom * p_args, uint8_t u8Status)
{
    uint32_t u32Timeout;

    if( u8Status == DRV_UART_TX_COMPLETE )
    {
    	p_args->sArgApp.sMsgQueue.u8IdMsg = UART_MSG_ID_TX_COMPLETED;
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

/******************************************************************************/
//  Call back function : TIMER
/******************************************************************************/
static void voTimerModbus_cb(t_uart_custom * p_args)
{
    // Stop the timer
	DrvBspUart_StopTimer(p_args);

    // Retrieve number of bytes in Fifo to check if data is incoming.
    // Fifo is empty there is no more data to wait for
    if( DrvBspUart_GetNbOfDataInFifo(p_args) == 0 )
	{
    	p_args->sArgApp.sMsgQueue.u8IdMsg = UART_MSG_ID_RX_RECEIVED;
    	p_args->sArgApp.p_App_Cb(&p_args->sArgApp.sMsgQueue);

    	p_args->UartReceiveInProgress = false;
    }
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
static void voModbusTimeout( serial_config_t  const * const psSerialCfg,
                             uint32_t		  * const pTimeOutModbusEOF,
                             uint32_t 		  * const pTimeOutFifo )
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
    
    // For baud rates greater than 19200 Bps, fixed values for the 2 timers 
    // should be used: it is recommended to use a value of 750�s for the 
    // inter-character time-out (t1.5) and a value of 1.750ms for inter-frame 
    // delay (t3.5).
    
    if( psSerialCfg->u32BaudRate <= 19200 )
    {
        // In microseconde * 3.5 character for EOF
        u32Value = ((uint32_t)3500000 * u8BitLength) / psSerialCfg->u32BaudRate;
        *pTimeOutModbusEOF = u32Value;
    }
    else
    {
        *pTimeOutModbusEOF = (uint32_t)1750;
    }
    
    // In microseconde * 1.5 interframe
    u32Value 	  = (uint32_t)15000000 / psSerialCfg->u32BaudRate;
    *pTimeOutFifo = *pTimeOutModbusEOF - u32Value;
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
int32_t DrvModbusRtu_Open( uint8_t  		u8Channel,
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
                          voSerialModbus_cb,
                          voTimerModbus_cb,
                          pIrq_App_cb,
                          p_GetTheBuffer );

    //--------------------------------------------------------------------------
    // Compute timeout
    //--------------------------------------------------------------------------
    voModbusTimeout( psSerialCfg, &TimeOutModbusEOF, &TimeOutFifo );
    DrvSerial_SetTimeOut(u8Channel, TimeOutModbusEOF, TimeOutFifo);

    return( ret );
}

inline __attribute__((always_inline)) int32_t DrvModbusRtu_Close(uint8_t u8Channel)
{
    return( DrvSerial_Close(u8Channel) );
}

inline __attribute__((always_inline)) int32_t DrvModbusRtu_Write(uint8_t  u8Channel, uint8_t *pDataTx, uint16_t u16LengthTx)
{   
    return( DrvSerial_Write(u8Channel, pDataTx, u16LengthTx) );
}

inline __attribute__((always_inline)) int32_t DrvModbusRtu_WriteInProgress(uint8_t u8Channel)
{
    return( DrvSerial_WriteInProgress(u8Channel) );
}

inline __attribute__((always_inline)) int32_t DrvModbusRtu_ReceiveInProgress(uint8_t u8Channel)
{
    return( DrvSerial_ReceiveInProgress(u8Channel) );
}

inline __attribute__((always_inline)) uint32_t DrvModbusRtu_GetData(uint8_t Channel, uint8_t GetInfo)
{       
    return( DrvSerial_GetData(Channel, GetInfo) );
}
