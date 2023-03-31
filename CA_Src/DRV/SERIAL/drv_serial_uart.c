/******************************************************************************/
/// \file           drv_serial_uart.c
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
    #include "drv_serial_uart_private.h"
    #include "drv_serial_uart.h"
#undef public

#define public extern
	#include <stdio.h>
    #include "drv_bsp_uart_private.h"
    #include "drv_serial_break_detection_private.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void voSetHalfDuplexMode(t_uart_custom *pDef);

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
static void voSetHalfDuplexMode(t_uart_custom *pDef)
{
	uint8_t u8HalfDuplex;

	if( pDef->ps_UartDef->sUart.pvoSetRs485HalfOrFull != NULL )
	{
		if( pDef->sSerialCfg.u8HalfFullDuplex == SERIAL_PORT_HALF_DUPLEX )
			u8HalfDuplex = true;	// Half duplex
		else
			u8HalfDuplex = false;	// Full duplex

		pDef->ps_UartDef->sUart.pvoSetRs485HalfOrFull(u8HalfDuplex);
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
void voDrvSerialUart_IrqUartCb(t_uart_custom * p_args, uint8_t u8Status)
{
    if( p_args == NULL )
    	return;

    switch( u8Status )
    {
    	case DRV_UART_TX_COMPLETE :
    		p_args->UartWriteInProgress = false;

			// Reset break detection timer
    		voDrvSerialDetectionBreak_Refresh(p_args->VirtualChannel);

			// Reset Idle detection timer
    		voDrvSerialDetectionIdle_Refresh(p_args->VirtualChannel);

			// Call back function
			if( p_args->sProtocolDrvCallback.voUart != NULL )
				p_args->sProtocolDrvCallback.voUart( p_args, u8Status );
		break;

    	case DRV_UART_RX_RECEIVE :
    		p_args->UartReceiveInProgress = true;

			//--------------------------------------------------------------------------
			// Rx data
			//--------------------------------------------------------------------------
			// Store the data received in buffer
			if( p_args->sRxTxBuf.sRx.u16Idx < p_args->sRxTxBuf.sRx.u16SizeMax )
			{
				p_args->sRxTxBuf.sRx.u8Buf[p_args->sRxTxBuf.sRx.u16Idx++] = p_args->u8DataRx;
			}
			else
			{
				 p_args->sRxTxBuf.sRx.u16Idx = 0;
			}

			// Reset break detection timer
			voDrvSerialDetectionBreak_Refresh(p_args->VirtualChannel);

			// Reset Idle detection timer
			voDrvSerialDetectionIdle_Refresh(p_args->VirtualChannel);

			// Call back function
			if( p_args->sProtocolDrvCallback.voUart != NULL )
				p_args->sProtocolDrvCallback.voUart( p_args, u8Status );
		break;
    }
}

void voDrvSerialUart_IrqTimerCb(t_uart_custom * p_args)
{
    if( (p_args != NULL) &&
		(p_args->sProtocolDrvCallback.voTimer != NULL) )
	{
    	p_args->sProtocolDrvCallback.voTimer( p_args );
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
int32_t DrvSerial_Open( uint8_t  		 u8Channel,
						serial_config_t  *psSerialCfg,
                        void (* pIrq_Uart_cb)(t_uart_custom * p_args, uint8_t u8Status),
                        void (* pIrq_Timer_cb)(t_uart_custom * p_args),
                        void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer),
                        t_uart_xfer **p_GetTheBuffer )
{
	t_uart_custom *pDef;
	uint32_t	  u32Tmp;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	if( u8Channel >= SERIAL_CHANNEL_COUNT )
		return( -1 );

    pDef = &uart_synergy_st[u8Channel];

    //----------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------
    // Init memory
	memset(pDef, 0, sizeof(t_uart_custom));
	//Save Uart config
	memcpy(&pDef->sSerialCfg, psSerialCfg, sizeof(serial_config_t));

    //----------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------
    // Serial buffer init
    pDef->sRxTxBuf.sRx.u16SizeMax = sizeof(pDef->sRxTxBuf.sRx.u8Buf);
    pDef->sRxTxBuf.sRx.u16Idx = 0;
    pDef->sRxTxBuf.sTx.u16SizeMax = sizeof(pDef->sRxTxBuf.sRx.u8Buf);
    pDef->sRxTxBuf.sTx.u16Idx = 0;

    // App cb and arg
    pDef->sArgApp.sMsgQueue.u8Channel  = u8Channel;
    pDef->sArgApp.sMsgQueue.u8IdMsg	   = 0xFF;
    pDef->sArgApp.sMsgQueue.pUartXfer  = &pDef->sRxTxBuf;
    pDef->sArgApp.p_App_Cb			   = pIrq_App_cb;

    // Call back for different protocol
    pDef->sProtocolDrvCallback.voUart  = pIrq_Uart_cb;
    pDef->sProtocolDrvCallback.voTimer = pIrq_Timer_cb;

    //
	pDef->VirtualChannel 	  	= u8Channel;	// Set virtual channel number
	pDef->UartWriteInProgress 	= false;		// Set write status to uart
	pDef->UartReceiveInProgress = false;

    //--------------------------------------------------------------------------
    // Temps de transmission pour un octet
    //--------------------------------------------------------------------------
	u32Tmp = pDef->sSerialCfg.u8DataBits + pDef->sSerialCfg.u8StopBits + 1; // 1 = startbit
	u32Tmp = (1000000 * u32Tmp) / pDef->sSerialCfg.u32BaudRate;
	pDef->sRS485.TimeOutForOneByte = u32Tmp;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	DrvBspUart_Open(pDef);

    //----------------------------------------------------------------------------
    // Set Duplex mode
    //----------------------------------------------------------------------------
	voSetHalfDuplexMode(pDef);

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	if( pDef->sSerialCfg.u8BreakProcessEnable == true )
	{
		voDrvSerialDetectionBreak_Open(u8Channel, pIrq_App_cb);
	}

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	voDrvSerialDetectionIdle_Open(u8Channel, pDef->sSerialCfg.u32IdleTimeOut_Ms, pIrq_App_cb);

	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	if( p_GetTheBuffer != NULL )
		*p_GetTheBuffer = &pDef->sRxTxBuf;
    
	//--------------------------------------------------------------------------
	//
	//--------------------------------------------------------------------------
	pDef->u8PortOpened = true;

    return( 0 );
}

int32_t DrvSerial_Close(uint8_t u8Channel)
{
	t_uart_custom *pDef;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	if( u8Channel >= SERIAL_CHANNEL_COUNT )
		return( -1 );

    pDef = &uart_synergy_st[u8Channel];

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
	if( pDef->u8PortOpened == true )
	{
		DrvBspUart_Close(pDef);

		//----------------------------------------------------------------------
		// Stop break process
		//----------------------------------------------------------------------
		voDrvSerialDetectionBreak_Close(u8Channel);

		//----------------------------------------------------------------------
		// Stop Idle process
		//----------------------------------------------------------------------
		voDrvSerialDetectionIdle_Close(u8Channel);

		//----------------------------------------------------------------------
		// No more call back : App and protocol
		//----------------------------------------------------------------------
        pDef->sArgApp.p_App_Cb			   = NULL;
        pDef->sProtocolDrvCallback.voUart  = NULL;
        pDef->sProtocolDrvCallback.voTimer = NULL;

		pDef->u8PortOpened = false;
	}

    return( 0 );
}

int32_t DrvSerial_Write(uint8_t  u8Channel, uint8_t *pDataTx, uint16_t u16LengthTx)
{   
	int32_t ErrReturn = -1;
	t_uart_custom *pDef;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
    if( u16LengthTx == 0 )
        return( 0 );

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
    if( u8Channel < SERIAL_CHANNEL_COUNT )
    {
        pDef = &uart_synergy_st[u8Channel];
    	ErrReturn = DrvBspUart_Write(pDef, pDataTx, u16LengthTx);
    	if( ErrReturn == 0 )
    		pDef->UartWriteInProgress = true;
    }

    return( ErrReturn );
}

int32_t DrvSerial_WriteInProgress(uint8_t u8Channel)
{
    int32_t ret_val = -1;

    if( u8Channel < SERIAL_CHANNEL_COUNT )
        ret_val = uart_synergy_st[ u8Channel ].UartWriteInProgress;

    return( ret_val );
}

int32_t DrvSerial_ReceiveInProgress(uint8_t u8Channel)
{
    int32_t ret_val = -1;

    if( u8Channel < SERIAL_CHANNEL_COUNT )
        ret_val = uart_synergy_st[ u8Channel ].UartReceiveInProgress;

    return( ret_val );
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
int32_t DrvSerial_SetTimeOut(uint8_t u8Channel, uint32_t TimeOutModbusEOF, uint32_t TimeOutFifo)
{
    int32_t ErrReturn = -1;

    if( u8Channel < SERIAL_CHANNEL_COUNT )
    {
        uart_synergy_st[u8Channel].sRS485.TimeOutModbusEOF = TimeOutModbusEOF;
        uart_synergy_st[u8Channel].sRS485.TimeOutFifo 	   = TimeOutFifo;

        ErrReturn = 0;
    }

    return( ErrReturn );
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
uint32_t DrvSerial_GetData(uint8_t u8Channel, uint8_t GetInfo)
{
    uint32_t u32Val = 0;

    switch( GetInfo )
    {
        case GET_SERIAL_TIMEOUT_FOR_ONE_BYTE :
            u32Val = uart_synergy_st[ u8Channel ].sRS485.TimeOutForOneByte;
        break;
    }

    return( u32Val );
}
