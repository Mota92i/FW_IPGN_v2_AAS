/******************************************************************************/
/// \file           app_thread_uart.c
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
    #include "drv_bsp_uart_private.h"
#undef public

#define public extern
	#include "drv_serial_uart_private.h"
    #include "drv_board_gpio.h"
#undef public

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static uint32_t u32GetBaudrate_Specific(uint32_t u32Baudrate);
static uint8_t  u8GetParity_Specific(uint8_t u8Parity);
static uint8_t  u8GetStopBits_Specific(uint8_t u8StopBits);
static void     voGetUartCfg_Specific(t_uart_custom *pDef);

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
const t_uart_feature sUartFeature[ SERIAL_CHANNEL_COUNT ] = {
		[ 0 ] = { { UART0_PERIPHERAL,
				    UART0_SERIAL_RX_TX_IRQN,
					2,							// Priority UART0_SERIAL_RX_TX_IRQ_PRIORITY
					UART0_CLK_SRC,
					NULL
				  },
				  { FTM1_PERIPHERAL,
				    FTM1_IRQN,
					kCLOCK_BusClk
				  },
				},
};

/******************************************************************************/
//  Hardware Interrupt
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
void Irq_Uart0_RxTx(void)
{
	t_uart_custom * p_args = &uart_synergy_st[ SERIAL_CHANNEL_1 ];
	uint32_t status_flag = UART_GetStatusFlags(p_args->ps_UartDef->sUart.base);
	uint8_t u8Temp;

	if( (status_flag & kUART_TransmissionCompleteFlag) &&
		(p_args->sRxTxBuf.sTx.u8TxInProgress == true) )
	{
		if( p_args->sRxTxBuf.sTx.u16Ptr < p_args->sRxTxBuf.sTx.u16Idx )
		{
			p_args->ps_UartDef->sUart.base->D = p_args->sRxTxBuf.sTx.u8Buf[p_args->sRxTxBuf.sTx.u16Ptr++];
		}
		else
		{
			p_args->sRxTxBuf.sTx.u8TxInProgress = false;
			UART_DisableInterrupts(p_args->ps_UartDef->sUart.base,
					              (kUART_TxDataRegEmptyInterruptEnable | kUART_TransmissionCompleteInterruptEnable));
			voDrvSerialUart_IrqUartCb(p_args, DRV_UART_TX_COMPLETE);
		}
	}
	else if( (status_flag & (kUART_RxDataRegFullFlag | kUART_FramingErrorFlag)) == kUART_RxDataRegFullFlag )
	{
		while( p_args->ps_UartDef->sUart.base->RCFIFO > 0U )
		{
			(void)p_args->ps_UartDef->sUart.base->S1;
			p_args->u8DataRx = p_args->ps_UartDef->sUart.base->D;
			voDrvSerialUart_IrqUartCb(p_args, DRV_UART_RX_RECEIVE);
		}
	}
	else
	{
		p_args->ps_UartDef->sUart.base->CFIFO |= UART_CFIFO_RXFLUSH_MASK;
		p_args->ps_UartDef->sUart.base->PFIFO &= ~UART_PFIFO_RXFE_MASK;
		u8Temp = p_args->ps_UartDef->sUart.base->D;
		(void)u8Temp;
		p_args->ps_UartDef->sUart.base->PFIFO |= UART_PFIFO_RXFE_MASK;
	}

    // Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
    //  exception return operation might vector to incorrect interrupt
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void FTM1_DriverIRQHandler(void)
{
	t_uart_custom * p_args = &uart_synergy_st[ SERIAL_CHANNEL_1 ];

	voDrvSerialUart_IrqTimerCb( p_args );

	FTM_StopTimer(p_args->ps_UartDef->sTimer.base);
	FTM_ClearStatusFlags(p_args->ps_UartDef->sTimer.base, kFTM_TimeOverflowFlag);
}


/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//  Specific microcontroller
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date
///
//----------------------------------------------------------------------------*/
static uint32_t u32GetBaudrate_Specific(uint32_t u32Baudrate)
{
    const uint32_t BaudRate[] = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };
    uint32_t u32VerifyBaudRate = 9600;
    uint32_t i;

    for(i = 0; i < sizeof(BaudRate) / sizeof(BaudRate[0]); i++)
    {
    	if( BaudRate[i] == u32Baudrate )
    	{
    		u32VerifyBaudRate = u32Baudrate;
    		break;
    	}
    }

    return( u32VerifyBaudRate );
}

static uint8_t u8GetParity_Specific(uint8_t u8Parity)
{
    uint8_t u8VerifyParity;

    if( u8Parity == SERIAL_PORT_PARITY_EVEN )
    	u8VerifyParity = kUART_ParityEven;
    else if( u8Parity == SERIAL_PORT_PARITY_ODD )
    	u8VerifyParity = kUART_ParityOdd;
    else
    	u8VerifyParity = kUART_ParityDisabled;

    return( u8VerifyParity );
}

static uint8_t u8GetStopBits_Specific(uint8_t u8StopBits)
{
    uint8_t u8VerifyStopBits;

    if( u8StopBits == SERIAL_STOP_BITS_2 )
    	u8VerifyStopBits = kUART_TwoStopBit;
    else
    	u8VerifyStopBits = kUART_OneStopBit;

    return( u8VerifyStopBits );
}

static void voGetUartCfg_Specific(t_uart_custom *pDef)
{
	pDef->sNxpSerialCfg.baudRate_Bps 	= u32GetBaudrate_Specific(pDef->sSerialCfg.u32BaudRate);
    pDef->sNxpSerialCfg.parityMode   	= u8GetParity_Specific(pDef->sSerialCfg.u8Parity);
    pDef->sNxpSerialCfg.stopBitCount 	= u8GetStopBits_Specific(pDef->sSerialCfg.u8StopBits);

    pDef->sNxpSerialCfg.txFifoWatermark = 0;
    pDef->sNxpSerialCfg.rxFifoWatermark = 1;

    pDef->sNxpSerialCfg.enableRxRTS 	= false;
	pDef->sNxpSerialCfg.enableTxCTS 	= false;

	pDef->sNxpSerialCfg.idleType		= kUART_IdleTypeStartBit;
	pDef->sNxpSerialCfg.enableTx		= true;
	pDef->sNxpSerialCfg.enableRx		= true;
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
int32_t DrvBspUart_Open(t_uart_custom *pDef)
{
    //----------------------------------------------------------------------------
    //
    //----------------------------------------------------------------------------
	pDef->ps_UartDef = &sUartFeature[ pDef->VirtualChannel ];

    //----------------------------------------------------------------------------
    // Timer frequ
    //----------------------------------------------------------------------------
	pDef->u32TimerClockFreq =  CLOCK_GetFreq(pDef->ps_UartDef->sTimer.u32ClkSrc) / (1U << (pDef->ps_UartDef->sTimer.base->SC & FTM_SC_PS_MASK));

    //----------------------------------------------------------------------------
    // Get uart config : specific microcontroller
    //----------------------------------------------------------------------------
	voGetUartCfg_Specific(pDef);

    //--------------------------------------------------------------------------
    // Setup the timer associate to uart
    //--------------------------------------------------------------------------
	//FTM_Init(FTM1_PERIPHERAL, &FTM1_config);
	// FTM_SetTimerPeriod(FTM1_PERIPHERAL, ((FTM1_CLOCK_SOURCE/ (1U << (FTM1_PERIPHERAL->SC & FTM_SC_PS_MASK))) / 10000) + 1);
	FTM_EnableInterrupts(pDef->ps_UartDef->sTimer.base, kFTM_TimeOverflowInterruptEnable);
	EnableIRQ(pDef->ps_UartDef->sTimer.u32NvicIrq);	// Enable interrupt FTM1_IRQn request in the NVIC

	//--------------------------------------------------------------------------
	// UART
	//--------------------------------------------------------------------------
	UART_Init( pDef->ps_UartDef->sUart.base,
			   &pDef->sNxpSerialCfg,
			   CLOCK_GetFreq(pDef->ps_UartDef->sUart.u32ClkSrc) );

	UART_DisableInterrupts(pDef->ps_UartDef->sUart.base, kUART_AllInterruptsEnable);

	// RTS handle on transmitt
	pDef->ps_UartDef->sUart.base->MODEM = UART_MODEM_TXRTSPOL_MASK | UART_MODEM_TXRTSE_MASK;

	//Workarounf found on internet : thes 4 lines
	pDef->ps_UartDef->sUart.base->CFIFO |= UART_CFIFO_RXFLUSH_MASK;
	pDef->ps_UartDef->sUart.base->PFIFO &= ~UART_PFIFO_RXFE_MASK;
	//uint8_t u8Temp = pDef->ps_UartDef->sUart.base->D;
	(void) pDef->ps_UartDef->sUart.base->D;
	pDef->ps_UartDef->sUart.base->PFIFO |= UART_PFIFO_RXFE_MASK;

	UART_EnableInterrupts(pDef->ps_UartDef->sUart.base,
			              (kUART_RxDataRegFullInterruptEnable));

	NVIC_SetPriority(pDef->ps_UartDef->sUart.u32NvicIrq, pDef->ps_UartDef->sUart.u32NvicIrqPriority);
	EnableIRQ(pDef->ps_UartDef->sUart.u32NvicIrq);	// Enable interrupt UART0_RX_TX_IRQn request in the NVIC
    
    return( 0 );
}

int32_t DrvBspUart_Close(t_uart_custom *pDef)
{
	//----------------------------------------------------------------------
	// Stop the timer
	//----------------------------------------------------------------------
	FTM_DisableInterrupts(pDef->ps_UartDef->sTimer.base, kFTM_TimeOverflowInterruptEnable);
	DisableIRQ(pDef->ps_UartDef->sTimer.u32NvicIrq);	// Enable interrupt FTM1_IRQn request in the NVIC
	FTM_StopTimer(pDef->ps_UartDef->sTimer.base);

	//----------------------------------------------------------------------
	// Disable the uart
	//----------------------------------------------------------------------
	UART_DisableInterrupts(pDef->ps_UartDef->sUart.base, kUART_AllInterruptsEnable);
	DisableIRQ(pDef->ps_UartDef->sUart.u32NvicIrq);	// Disable interrupt UART0_RX_TX_IRQn request in the NVIC
    
    return( 0 );
}

int32_t DrvBspUart_Write(t_uart_custom *pDef, uint8_t *pDataTx, uint16_t u16Size)
{
	// Write is prohibited when the line is in break
	if( DrvBspUart_GetRxLevel(pDef->VirtualChannel) == 0x01 )
	{
		memcpy(pDef->sRxTxBuf.sTx.u8Buf, pDataTx, u16Size);
		pDef->sRxTxBuf.sTx.u16Idx = u16Size;
		pDef->sRxTxBuf.sTx.u16Ptr = 0;
		pDef->sRxTxBuf.sTx.u8TxInProgress = true;
		pDef->ps_UartDef->sUart.base->D = pDef->sRxTxBuf.sTx.u8Buf[pDef->sRxTxBuf.sTx.u16Ptr++];

		UART_EnableInterrupts(pDef->ps_UartDef->sUart.base, kUART_TransmissionCompleteInterruptEnable);
	}

	return( 0 );
}

int32_t DrvBspUart_GetRxLevel(uint8_t u8Channel)
{
	int32_t Level = -1;

	if( u8Channel < SERIAL_CHANNEL_COUNT )
	{
		switch( u8Channel )
		{
			case SERIAL_CHANNEL_1 :
				Level = DrvGpio_GetUart0RxLevel();
			break;
		}
	}

    return( Level );
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
void DrvBspUart_SetTimeAndStartTimer(t_uart_custom *pDef, uint32_t u32Period)
{
	uint32_t u32Freq = 1000000 / u32Period;

	FTM_StopTimer(pDef->ps_UartDef->sTimer.base);
	FTM_SetTimerPeriod( pDef->ps_UartDef->sTimer.base, (pDef->u32TimerClockFreq / u32Freq) + 1 );
	FTM_StartTimer(pDef->ps_UartDef->sTimer.base, kFTM_SystemClock);

	pDef->ps_UartDef->sTimer.base->CNT = 0;
}

void DrvBspUart_StopTimer(t_uart_custom *pDef)
{
	FTM_StopTimer(pDef->ps_UartDef->sTimer.base);
}

int32_t DrvBspUart_GetNbOfDataInFifo(t_uart_custom *pDef)
{
	// todo
	return( 0 );
}

uint32_t DrvBspUart_GetModBusTimeout(t_uart_custom *pDef)
{
	uint32_t u32Timeout;

	u32Timeout = pDef->sRS485.TimeOutModbusEOF;

#if 0
    // Fifo status register
    uint8_t u32_SSR_FIFO_Reg = *pUartSynergy->pSciReg;

    // Compute silent timeout
    if( u32_SSR_FIFO_Reg & 0x01 )   // Timeout du fifo : FIFO_DR
        u32Timeout = pUartSynergy->sRS485.TimeOutFifo;
    else                            // Timeout for EOF
        u32Timeout = pUartSynergy->sRS485.TimeOutModbusEOF;
#endif

	return( u32Timeout );
}
