/******************************************************************************/
/// \file           task_serial.c
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
    #include "task_serial.h"
#undef public

#define public extern
	#include <stdbool.h>
	#include <string.h>
    #include "common_function.h"
    #include "drv_serial.h"
    #include "drv_hdw_timer.h"
    #include "core_spec.h"
	#include "protocol_msg_nmea_parser.h"
    #include "protocol_modbus_diagnostic.h"
    #include "os_msg_queue.h"
    #include "os_task.h"
    #include "task_main.h"
    #include "task_display.h"
	#include "core_app_serial_msg.h"
#undef public      

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define IDLE_TIMEOUT	( 500 )	// ms
const uint8_t g_u8AnemoGirModbusTx[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x02, 0x71, 0xCB};
const uint8_t g_u8EnableAutomate = true;
static uint8_t g_u8RequestMbusSend = false;

/******************************************************************************/
//  Constant declarations
/******************************************************************************/
#define TASK_SERIAL_TIMEOUT_MILLI_SEC	( 250 )
#define TASK_SERIAL_TIMEOUT_TICK		( TASK_SERIAL_TIMEOUT_MILLI_SEC / portTICK_PERIOD_MS )	// 1sec

#define NO_MESSAGE_TIMEOUT_MILLI_SEC	( 500 )
#define NO_MESSAGE_TIMEOUT_TICK			( (NO_MESSAGE_TIMEOUT_MILLI_SEC / TASK_SERIAL_TIMEOUT_MILLI_SEC)  + 1 )

/******************************************************************************/
//  Varibales definitions
/******************************************************************************/
static struct{
    serial_interface_t 	*pSerial;
    uint8_t             modeDiag;
    t_uart_xfer 	   	*p_SerialBuffer;
    t_serial_link_cfg	sModbusLink;
}g_ctx;

/******************************************************************************/
//  Varibales definitions
/******************************************************************************/
#define INVALID_MEASURE		(0x8000)

static t_msg_nmea_parser g_sMeasure = {
	.f32_Value   = 0,
	.s16Value    = INVALID_MEASURE,
	.s8Res	  	 = 0,
	.bPrintSign  = false,
};

/******************************************************************************/
//  Function prototype declarations
/******************************************************************************/
static void SerialCom_Cb(t_uart_msg_queue *p_msgBuffer);
static void SetPortCom(void);
static int32_t Execute_Normal(t_uart_xfer *psSerialData);
static int32_t Execute_Diagnostique(t_uart_xfer *psSerialData);
static void voTraitementDisplay(uint8_t u8Status);
static void voAutomateModbus(void);
static int intTraitementMbus(t_uart_xfer *psSerialData, t_msg_nmea_parser *psValue, uint8_t u8Idx);

//----------------------------------------------------------------------------*/
// Call back function from serial driver 
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
static void SerialCom_Cb(t_uart_msg_queue *p_msgBuffer)
{
	voOsMsgQueue_SendToFrontFromISR(OS_MSG_QUEUE_SERIAL, (void *)p_msgBuffer);
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
static void SetPortCom(void)
{
	// Mode normale
	serial_config_t sSerialModeNormal;

	// diag mod default
	const serial_config_t sSerialModeDiag = {
			.u32BaudRate  			= 9600,
			.u8DataBits 			= 8,
			.u8Parity				= SERIAL_PORT_PARITY_NONE,
			.u8StopBits 			= SERIAL_STOP_BITS_1,
			.u8HalfFullDuplex 		= SERIAL_PORT_HALF_DUPLEX,
			.u8BreakProcessEnable 	= true,
			.u32IdleTimeOut_Ms		= 0 // Disable
	};

	serial_config_t const *pSerialCfg;
	uint8_t u8Protocol;

	// Get the protocol
	if( g_ctx.modeDiag == true )
	{
		u8Protocol = DRV_SERIAL_MODBUS_RTU;
		pSerialCfg = &sSerialModeDiag;
	}
	else
	{
		sSerialModeNormal.u32BaudRate  			= G_DeviceCfg.psAppDeviceCfg->sSerialCfg.u32Speed;
		sSerialModeNormal.u8DataBits 			= 8;
		sSerialModeNormal.u8Parity				= G_DeviceCfg.psAppDeviceCfg->sSerialCfg.u8Parity;
		sSerialModeNormal.u8StopBits 			= SERIAL_STOP_BITS_1;
		sSerialModeNormal.u8HalfFullDuplex 		= SERIAL_PORT_HALF_DUPLEX;
		sSerialModeNormal.u8BreakProcessEnable 	= true;
		sSerialModeNormal.u32IdleTimeOut_Ms		= IDLE_TIMEOUT; // 0 = Disable

		u8Protocol = G_DeviceCfg.psAppDeviceCfg->u8protocol;
		pSerialCfg = &sSerialModeNormal;
	}

	// Get the protocol interface
	g_ctx.pSerial  = psDrvSerial_GetInterface( u8Protocol );

    // Close port
    g_ctx.pSerial->Close( SERIAL_CHANNEL_1 );
    
    // Open port
    g_ctx.pSerial->Open( SERIAL_CHANNEL_1,					// Channel
    					 (serial_config_t *)pSerialCfg,		// Serial config
                         SerialCom_Cb,						// Cb func from serial
						 &g_ctx.p_SerialBuffer );			// Get Tx Rx buffer
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
static int intTraitementMbus(t_uart_xfer *psSerialData, t_msg_nmea_parser *psValue, uint8_t u8Idx)
{
	int ret = -1;
	Uartframe_t sTx;
	Uartframe_t sRx;
	int Status;
	t_modbus_data_decoder sModbusDataOut;
	uint16_t u16Array[128];

	sTx.u16Len = sizeof(g_u8AnemoGirModbusTx);
	sTx.pu8Buf = (uint8_t *)&g_u8AnemoGirModbusTx[ 0 ];

	sRx.u16Len = (uint8_t)psSerialData->sRx.u16Idx;
	sRx.pu8Buf = psSerialData->sRx.u8Buf;

	if( g_u8RequestMbusSend == false )
	{
		if( sTx.u16Len == sRx.u16Len )
		{
			Status = memcmp(sTx.pu8Buf, sRx.pu8Buf, sTx.u16Len);
			if( Status == 0 )
			{
				g_u8RequestMbusSend = true;
			}
		}
	}
	else
	{
		sModbusDataOut.pu16Data = &u16Array[ 0 ];
		sModbusDataOut.u8Size	= sizeof(u16Array) / sizeof(u16Array[ 0 ]);

		Status = intCoreAppSerialMsg_ModbusDecoder(&sTx, &sRx, &sModbusDataOut);
		if( Status == 0 )
		{
			psValue->u8Status	= true;
			psValue->s16Value	= sModbusDataOut.pu16Data[ u8Idx ];
			psValue->f32_Value  = psValue->s16Value;
			psValue->s8Res	  	= 0;
			psValue->bPrintSign = false;
		}

		g_u8RequestMbusSend = false;
		ret = 0;
	}

	return( ret );
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
static int32_t Execute_Normal(t_uart_xfer *psSerialData)
{
	t_msg_nmea_parser sValue = {
		.u8Status	= false,
		.f32_Value	= 0,
		.s16Value	= 0,
		.s8Res		= 0,
		.bPrintSign	= false
	};

	if( G_DeviceCfg.psAppDeviceCfg->eInputData == INPUT_DATA_ANALOG )
		return( 0 );

	switch( G_DeviceCfg.sSpec.u32Type )
	{
		case SPEC_TYPE_CAP : voProtocolMsgNmeaParser( NMEA_MSG_INHDT,
								 	 	 	 	 	  psSerialData->sRx.u8Buf,
													  (uint8_t)psSerialData->sRx.u16Idx,
													  &sValue );
		break;

		case SPEC_TYPE_ANEMOGIR_CA_CAP :
			if( intTraitementMbus(psSerialData, &sValue, 1) != 0 )
				return( 0 );

			sValue.f32_Value /= 10;
			sValue.s8Res	  = 1;
		break;

		case SPEC_TYPE_ANEMOGIR_CA_VITESSE :
			if( intTraitementMbus(psSerialData, &sValue, 0) != 0 )
				return( 0 );

			sValue.f32_Value /= 100;
			sValue.s8Res	  = 2;
		break;
	}

	if( sValue.u8Status == true )
	{
		memcpy(&g_sMeasure, &sValue, sizeof(sValue));
	}

	voTraitementDisplay( sValue.u8Status );

    return( 0 );
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
static int32_t Execute_Diagnostique(t_uart_xfer *psSerialData)
{
    int32_t     Error;
    Uartframe_t sInframe;
    Uartframe_t sOutframe;
    
    sInframe.pu8Buf  = psSerialData->sRx.u8Buf;
    sInframe.u16Len  = psSerialData->sRx.u16Idx;
    
    sOutframe.pu8Buf = psSerialData->sTx.u8Buf;

    Error = ProtocolModbusDiagnostic_Process(0, &sInframe, &sOutframe);

    if( Error >= 0 )
    {       
        //----------------------------------------------------------------------
        // Delay before sending reply
        //----------------------------------------------------------------------
      	DrvHdwTimer_SetDelayAndWait(TIMER_DELAY_ID_SERIAL_1, DRV_HDW_TIMER_MILLI_SEC( 25 ));
    
        //----------------------------------------------------------------------
        // Send data
        //----------------------------------------------------------------------
		psSerialData->sTx.u16Idx = sOutframe.u16Len;
		if( g_ctx.pSerial->WriteInProgress(SERIAL_CHANNEL_1) == false )
		{
			g_ctx.pSerial->Write( SERIAL_CHANNEL_1,
								  psSerialData->sTx.u8Buf,
								  psSerialData->sTx.u16Idx );
		}
    }
    else
    {
        psSerialData->sTx.u16Idx = 0;
    }

    return( 0 );
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
static void voAutomateModbus(void)
{
	if( g_ctx.pSerial->ReceiveInProgress(SERIAL_CHANNEL_1) == false )
	{
		g_ctx.pSerial->Write( SERIAL_CHANNEL_1,
							  (uint8_t  *)&g_u8AnemoGirModbusTx[0],
							  sizeof(g_u8AnemoGirModbusTx));

		g_u8RequestMbusSend = true;
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
static void voTraitementDisplay(uint8_t u8Status)
{
	static uint8_t u8CptError = 0;
	t_app_measure_msg sMsgMeasure = {
		.eEventType 		 = APP_MEASURE_MSG_QUEUE_SERIAL,
		.sMeasureSa.f32Value = 0,
		.sMeasureSa.s8Status = false
	};

	if( u8Status == false )
	{
		if( u8CptError < 255 )
			u8CptError++;

		// Send once only
		if( u8CptError == NO_MESSAGE_TIMEOUT_TICK )
		{
			OsMsgQueue_SendToBack(OS_MSG_QUEUE_MEASURE, &sMsgMeasure);
		}
	}
	else
	{
		sMsgMeasure.sMeasureSa.f32Value = g_sMeasure.f32_Value;
		sMsgMeasure.sMeasureSa.s8Status = true;
		OsMsgQueue_SendToBack(OS_MSG_QUEUE_MEASURE, &sMsgMeasure);

		u8CptError = 0;
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
void task_serial(void * pvParameters)
{
    int32_t (* pExecuteFunc)(t_uart_xfer *);
	int32_t					Err_MsgQueue;
	t_uart_msg_queue        MsgQueue;

    //--------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------
    g_ctx.modeDiag = false;
    SetPortCom();

    //--------------------------------------------------------------------------
    // Start main thread
    //--------------------------------------------------------------------------
    pExecuteFunc = &Execute_Normal;

    while(1)
    { 
        //----------------------------------------------------------------------
        // Wait for message
        //----------------------------------------------------------------------
    	Err_MsgQueue = OsMsgQueue_Receive( OS_MSG_QUEUE_SERIAL, (void *)&MsgQueue, TASK_SERIAL_TIMEOUT_TICK, OS_MSG_WAIT_MESSAGE );

        //----------------------------------------------------------------------
        // Time out
        //----------------------------------------------------------------------
        if( Err_MsgQueue != 0 )
        {
        	voTraitementDisplay( false );
        	continue;
        }

        //----------------------------------------------------------------------
        // Error channel
        //----------------------------------------------------------------------
        if( MsgQueue.u8Channel != SERIAL_CHANNEL_1 )
        	continue;

        //----------------------------------------------------------------------
        // Process message
        //----------------------------------------------------------------------
		switch( MsgQueue.u8IdMsg )
		{
			case UART_MSG_ID_RX_RECEIVED :
				pExecuteFunc(MsgQueue.pUartXfer);

				MsgQueue.pUartXfer->sRx.u16Idx = 0;
			break;

			case UART_MSG_ID_TX_COMPLETED :
			break;

			// Enter in diag mode
			case UART_MSG_ID_BREAK_START :
				if( g_ctx.modeDiag == false )
				{
					pExecuteFunc   = &Execute_Diagnostique;
					g_ctx.modeDiag = true;
					G_DeviceError.bit.IfDiagMode = true;
                    SetPortCom();
				 }
			break;

			// Exit from diag mode
			case UART_MSG_ID_BREAK_END :
				if( g_ctx.modeDiag == true )
				{
					voTaskMain_FlagResetSet();
					pExecuteFunc   = &Execute_Normal;
					g_ctx.modeDiag = true;
                    SetPortCom();
				}
			break;

			case UART_MSG_ID_IDLE :
				if( g_u8EnableAutomate == true )
					voAutomateModbus();
			break;

			case UART_MSG_ID_UART_CONFIG_CHANGED :
			break;
		} // endswitch
    } // end while
}
