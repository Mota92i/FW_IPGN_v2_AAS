/******************************************************************************/
/// \file           os_msg_queue.c
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
    #include "os_msg_queue.h"
#undef public

#define public extern
	#include "FreeRTOS.h"
	#include "queue.h"
#undef public

/******************************************************************************/
//  types declaration
/******************************************************************************/
// The queue is to be created to hold a maximum of 10 uint64_t variables.
#define SERIAL_QUEUE_LENGTH    		( 10 )
#define SERIAL_ITEM_SIZE       		( 8 )
static uint8_t g_SerialQueueStorageArea[ SERIAL_QUEUE_LENGTH * SERIAL_ITEM_SIZE ];

// The queue is to be created to hold a maximum of 10 uint64_t variables.
#define VOLTMETRE_QUEUE_LENGTH    	( 10 )
#define VOLTMETRE_ITEM_SIZE       	( 12 )
static uint8_t g_VoltmetreQueueStorageArea[ VOLTMETRE_QUEUE_LENGTH * VOLTMETRE_ITEM_SIZE ];

// The queue is to be created to hold a maximum of 10 uint64_t variables.
#define MEASURE_QUEUE_LENGTH ( 10 )
#define MEASURE_ITEM_SIZE    ( 12 )
static uint8_t g_MeasureQueueStorageArea[ MEASURE_QUEUE_LENGTH * MEASURE_ITEM_SIZE ];

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
	uint8_t 	  u8QueueLength;
	uint8_t 	  u8ItemSize;
	uint8_t		  *pu8QueueStorageArea;
	QueueHandle_t xHandle;
	StaticQueue_t xStaticQueue;
}t_list_msg_queue;

static t_list_msg_queue g_sListMsgQueue[ OS_MSG_QUEUE_COUNT ] = {
	[ OS_MSG_QUEUE_SERIAL         ] = {
			.u8QueueLength  	 = SERIAL_QUEUE_LENGTH,
			.u8ItemSize		 	 = SERIAL_ITEM_SIZE,
			.pu8QueueStorageArea = &g_SerialQueueStorageArea[ 0 ],
	},
	[ OS_MSG_QUEUE_VOLTMETRE      ] = {
			.u8QueueLength  	 = VOLTMETRE_QUEUE_LENGTH,
			.u8ItemSize		 	 = VOLTMETRE_ITEM_SIZE,
			.pu8QueueStorageArea = &g_VoltmetreQueueStorageArea[ 0 ],
	},
	[ OS_MSG_QUEUE_MEASURE 		  ] = {
			.u8QueueLength  	 = MEASURE_QUEUE_LENGTH,
			.u8ItemSize		 	 = MEASURE_ITEM_SIZE,
			.pu8QueueStorageArea = &g_MeasureQueueStorageArea[ 0 ],
	},
};

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
//static t_MsgQueue g_sMsgQueue;

/******************************************************************************/
//  Function definitions
/******************************************************************************/

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2018-10-11
///                 
/*----------------------------------------------------------------------------*/
void voOsMsgQueue_Init(void)
{
	uint8_t u8Idx;

	for(u8Idx = 0; u8Idx < OS_MSG_QUEUE_COUNT; u8Idx++)
	{
		// Create a queue capable of containing 10 uint64_t values.
		g_sListMsgQueue[ u8Idx ].xHandle = xQueueCreateStatic( g_sListMsgQueue[ u8Idx ].u8QueueLength,
															   g_sListMsgQueue[ u8Idx ].u8ItemSize,
															   g_sListMsgQueue[ u8Idx ].pu8QueueStorageArea,
															   &g_sListMsgQueue[ u8Idx ].xStaticQueue );
	}
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2018-10-11
///
/*----------------------------------------------------------------------------*/
int32_t OsMsgQueue_SendToBack(uint8_t u8Idx, void *pvoArg)
{
	int32_t ret = -1;

	if( xQueueSendToBack( g_sListMsgQueue[ u8Idx ].xHandle,
					   	  pvoArg,
						  ( TickType_t ) 10 ) == pdPASS  )
	{
		ret = 0;
	}

	return( ret );
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-19
///
/*----------------------------------------------------------------------------*/
void voOsMsgQueue_SendToFrontFromISR(uint8_t u8Idx, void *pvoArg)
{
	// We have not woken a task at the start of the ISR.
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xQueueSendToFrontFromISR( g_sListMsgQueue[ u8Idx ].xHandle, pvoArg, &xHigherPriorityTaskWoken );

	// Did sending to the queue unblock a higher priority task?
	if( xHigherPriorityTaskWoken )
	{
		// Actual macro used here is port specific.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		//taskYIELD_FROM_ISR ();
	}
}

void voOsMsgQueue_SendFromISR(uint8_t u8Idx, void *pvoArg)
{
	// We have not woken a task at the start of the ISR.
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	xQueueSendFromISR( g_sListMsgQueue[ u8Idx ].xHandle, pvoArg, &xHigherPriorityTaskWoken );

	// Did sending to the queue unblock a higher priority task?
	if( xHigherPriorityTaskWoken )
	{
		// Actual macro used here is port specific.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		//taskYIELD_FROM_ISR ();
	}
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]      None
/// \param[out]     None
/// \return         None
///
/// \version        1.0
/// \date           2022-01-19
///
/*----------------------------------------------------------------------------*/
int32_t OsMsgQueue_Receive(uint8_t u8Idx, void *pvoArg, uint32_t TimeOut, uint8_t u8WaitMessageType)
{
	t_list_msg_queue *psList = &g_sListMsgQueue[ u8Idx ];
	int32_t ret = -1;
	uint32_t u32Timeout;

	if( u8WaitMessageType == OS_MSG_NO_WAIT_MESSAGE )
		u32Timeout = 0;								// No wait if queue is empty
	else
		u32Timeout = TimeOut / portTICK_PERIOD_MS;	// Wait message

	if( xQueueReceive( g_sListMsgQueue[ u8Idx ].xHandle,
					   pvoArg,
					   (TickType_t)u32Timeout ) == pdPASS  )
	{
		ret = (int32_t)uxQueueMessagesWaiting( psList->xHandle );
	}

	return( ret );
}
