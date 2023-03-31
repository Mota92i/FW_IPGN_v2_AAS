/******************************************************************************/
/// \file           drv_serial_break_detection.h
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
#ifndef _DRV_SERIAL_BREAK_DETECTION_PRIVATE_H_
#define _DRV_SERIAL_BREAK_DETECTION_PRIVATE_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "drv_serial_uart.h"

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voDrvSerialDetection_Init(void);
extern void voDrvSerialDetectionBreak_Open(uint8_t  u8Channel,
								    	   void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer));
extern void voDrvSerialDetectionIdle_Open(uint8_t  u8Channel,
								   	   	  uint32_t u32Value_ms,
										  void (* pIrq_App_cb)(t_uart_msg_queue *p_msgBuffer));
extern void voDrvSerialDetectionBreak_Close(uint8_t u8Channel);
extern void voDrvSerialDetectionIdle_Close(uint8_t u8Channel);
extern void voDrvSerialDetectionBreak_Refresh(uint8_t u8Channel);
extern void voDrvSerialDetectionIdle_Refresh(uint8_t u8Channel);

#endif /* _DRV_SERIAL_BREAK_DETECTION_PRIVATE_H_ */
