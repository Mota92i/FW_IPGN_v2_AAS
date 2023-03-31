/******************************************************************************/
/// \file           drv_serial_break_detection.c
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
    #include "drv_serial_break_detection_private.h"
#undef public

#define public extern
	#include "drv_hdw_timer.h"
	#include "drv_bsp_uart_private.h"
#undef public

/******************************************************************************/
//  constant declarations
/******************************************************************************/
#define TIMER_PERIODE_MS     	(1)      //ms
#define BREAK_START_TIMEOUT		(uint32_t)(1000  / TIMER_PERIODE_MS)
#define BREAK_STOP_TIMEOUT      (uint32_t)(10000 / TIMER_PERIODE_MS)

/******************************************************************************/
//  variables declarations
/******************************************************************************/
typedef struct {
	struct{
		uint8_t  bEnable;
		uint32_t u32Cpt;
	}sBreak;
	struct{
		uint32_t u32TimeOut;
		uint32_t u32Cpt;
	}sIdle;
    uint8_t  u8ModeDiag;
    void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer);
    t_uart_msg_queue msgQueue;
}t_detection_var;

static t_detection_var g_sVarDetection[ SERIAL_CHANNEL_COUNT ];

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voDrvDetection_Open(t_detection_var *p_sdata,
								void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer));
static void voDrvDetection_Close(uint8_t u8Channel);
static void voDrvDetection_Process(void);

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
void voDrvSerialDetection_Init(void)
{
    uint32_t 	 	u8Channel;
    t_detection_var *p_sdata;
    
    memset(&g_sVarDetection[0], 0, sizeof(g_sVarDetection));

    for(u8Channel = 0; u8Channel < SERIAL_CHANNEL_COUNT; u8Channel++)
    {
    	p_sdata = &g_sVarDetection[u8Channel];

    	p_sdata->sBreak.bEnable		= false;

    	p_sdata->u8ModeDiag 		= false;
    	p_sdata->pIrq_App_cb 		= NULL;
        
    	p_sdata->msgQueue.u8Channel = u8Channel;
    	p_sdata->msgQueue.u8IdMsg	= UART_MSG_ID_NO_MSG;
    	p_sdata->msgQueue.pUartXfer = NULL;
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
static void voDrvDetection_Open(t_detection_var *p_sdata,
								      void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer))
{
	if( p_sdata->pIrq_App_cb == NULL )
	{
		p_sdata->pIrq_App_cb = pIrq_App_cb;

		DrvHdwTimer_RegisterPeriodic500us(voDrvDetection_Process, DRV_HDW_TIMER_MILLI_SEC( TIMER_PERIODE_MS ));
	}
}

void voDrvSerialDetectionBreak_Open(uint8_t  u8Channel,
								    void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer))
{
	g_sVarDetection[u8Channel].sBreak.bEnable = true;
	g_sVarDetection[u8Channel].sBreak.u32Cpt  = 0;

	voDrvDetection_Open(&g_sVarDetection[u8Channel], pIrq_App_cb);
}

void voDrvSerialDetectionIdle_Open(uint8_t  u8Channel,
								   uint32_t u32Value_ms,
								   void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer))
{
	if( u32Value_ms > 0 )
	{
		g_sVarDetection[u8Channel].sIdle.u32TimeOut = u32Value_ms / TIMER_PERIODE_MS;
		g_sVarDetection[u8Channel].sIdle.u32Cpt	 	= 0;

		voDrvDetection_Open(&g_sVarDetection[u8Channel], pIrq_App_cb);
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
static void voDrvDetection_Close(uint8_t u8Channel)
{
    uint8_t i;
    uint8_t flag;

    g_sVarDetection[u8Channel].pIrq_App_cb = NULL;
    
	flag = false;
	for(i = 0; i < SERIAL_CHANNEL_COUNT; i++)
	{
		if( (g_sVarDetection[i].sBreak.bEnable == true) ||
			(g_sVarDetection[i].sIdle.u32TimeOut > 0) )
		{
			flag = true;
		}
	}

	if( flag == false )
		DrvHdwTimer_DeRegisterPeriodic500us( voDrvDetection_Process );
}

void voDrvSerialDetectionBreak_Close(uint8_t u8Channel)
{
    if( u8Channel < SERIAL_CHANNEL_COUNT )
    {
    	g_sVarDetection[u8Channel].sBreak.bEnable = 0;
    	g_sVarDetection[u8Channel].sBreak.u32Cpt  = 0;
    	g_sVarDetection[u8Channel].u8ModeDiag 	  = false;

    	voDrvDetection_Close(u8Channel);
    }
}

void voDrvSerialDetectionIdle_Close(uint8_t u8Channel)
{
    if( u8Channel < SERIAL_CHANNEL_COUNT )
    {
    	g_sVarDetection[u8Channel].sIdle.u32TimeOut = 0;
    	g_sVarDetection[u8Channel].sIdle.u32Cpt	 = 0;
        
    	voDrvDetection_Close(u8Channel);
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
void voDrvSerialDetectionBreak_Refresh(uint8_t u8Channel)
{
    if( u8Channel < SERIAL_CHANNEL_COUNT )
    {
    	g_sVarDetection[u8Channel].sBreak.u32Cpt = 0;
    }
}

void voDrvSerialDetectionIdle_Refresh(uint8_t u8Channel)
{
    if( u8Channel < SERIAL_CHANNEL_COUNT )
    {
    	g_sVarDetection[u8Channel].sIdle.u32Cpt = 0;
    }
}

//----------------------------------------------------------------------------*/
//  Timer for break detection on Uart
//----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           
///                 
//----------------------------------------------------------------------------*/
static void voDrvDetection_Process(void)
{
	t_detection_var *p_sdata;
    uint8_t  		IoPortLevel;
    uint8_t         u8val;
    uint8_t         i;
    const struct{
        uint8_t         u8MsgQeue;
        uint8_t  		Level;
        uint32_t        u32TimeOut;
    }sBreakData[] = {
        [ 0 ] = { UART_MSG_ID_BREAK_START, 0, BREAK_START_TIMEOUT },
        [ 1 ] = { UART_MSG_ID_BREAK_END,   1, BREAK_STOP_TIMEOUT  } };
        
    //--------------------------------------------------------------------------
    // 
    //-------------------------------------------------------------------------- 
    for(i = 0; i < SERIAL_CHANNEL_COUNT; i++)
    {
    	p_sdata = &g_sVarDetection[i];

        //------------------------------------------------------------------
        // Traitement du break
        //------------------------------------------------------------------
    	if( p_sdata->sBreak.bEnable == true )
		{
			IoPortLevel = DrvBspUart_GetRxLevel( i );
			u8val = p_sdata->u8ModeDiag & 0x01;

			if( IoPortLevel == sBreakData[u8val].Level )
			{
				if( ++p_sdata->sBreak.u32Cpt > sBreakData[u8val].u32TimeOut )
				{
					p_sdata->u8ModeDiag = ~p_sdata->u8ModeDiag & 0x01;
					p_sdata->sBreak.u32Cpt = 0;

					p_sdata->msgQueue.u8IdMsg = sBreakData[u8val].u8MsgQeue;

					// Send Message for break detection
					if( p_sdata->pIrq_App_cb != NULL )
						p_sdata->pIrq_App_cb(&p_sdata->msgQueue);
				}
			}
		}

        //------------------------------------------------------------------
        // Traitement du idle
        //------------------------------------------------------------------
        if( p_sdata->sIdle.u32TimeOut > 0 )
		{
			if( p_sdata->u8ModeDiag == true )
			{
				p_sdata->sIdle.u32Cpt = 0;
			}
			else
			{
				if( ++p_sdata->sIdle.u32Cpt > p_sdata->sIdle.u32TimeOut )
				{
					p_sdata->sIdle.u32Cpt = 0;

					p_sdata->msgQueue.u8IdMsg = UART_MSG_ID_IDLE;

					// Send Message for break detection
                    if( p_sdata->pIrq_App_cb != NULL )
                    	p_sdata->pIrq_App_cb(&p_sdata->msgQueue);
				}
			}
		}
    } // end for
}
