/******************************************************************************/
/// \file           os_msg_queue.h
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
//  Inclusion checking
/******************************************************************************/
#ifndef __OS_MSG_QUEUE_H__
#define __OS_MSG_QUEUE_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
//  constant declaration
/******************************************************************************/
typedef enum{
	OS_MSG_NO_WAIT_MESSAGE = 0,
	OS_MSG_WAIT_MESSAGE
}t_os_msg_queue_wait_message;

/******************************************************************************/
//  constant declaration
/******************************************************************************/
typedef enum{
	OS_MSG_QUEUE_SERIAL = 0,
	OS_MSG_QUEUE_VOLTMETRE,
	OS_MSG_QUEUE_MEASURE,
	
	OS_MSG_QUEUE_COUNT
}t_os_msg_queue;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void    voOsMsgQueue_Init(void);

extern void	   voOsMsgQueue_SendToFrontFromISR(uint8_t u8Idx, void *pvoArg);
extern void    voOsMsgQueue_SendFromISR		  (uint8_t u8Idx, void *pvoArg);
extern int32_t OsMsgQueue_SendToBack		  (uint8_t u8Idx, void *pvoArg);
extern int32_t OsMsgQueue_Receive(uint8_t u8Idx, void *pvoArg, uint32_t TimeOut, uint8_t u8WaitMessageType);

#endif /* __OS_MSG_QUEUE_H__ */

