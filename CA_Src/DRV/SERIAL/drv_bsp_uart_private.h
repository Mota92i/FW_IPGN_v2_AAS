/******************************************************************************/
/// \file           synergy_uart.h
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
#ifndef _DRV_BSP_UART_PRIVATE_H_
#define _DRV_BSP_UART_PRIVATE_H_

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>
#include "fsl_uart.h"
#include "fsl_ftm.h"
#include "peripherals.h"
#include "drv_serial.h"
#include "drv_serial_def.h"
#include "drv_bsp_uart_private_def.h"

/******************************************************************************/
// VARIABLES DECLARATIONS
/******************************************************************************/
public t_uart_custom uart_synergy_st[ SERIAL_CHANNEL_COUNT ];

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
int32_t DrvBspUart_Open(t_uart_custom *pDef);
int32_t DrvBspUart_Close(t_uart_custom *pDef);
int32_t DrvBspUart_Write(t_uart_custom *pDef, uint8_t *pDataTx, uint16_t u16Size);
int32_t DrvBspUart_GetRxLevel(uint8_t u8Channel);

void DrvBspUart_SetTimeAndStartTimer(t_uart_custom *pDef, uint32_t u32Period);
void DrvBspUart_StopTimer(t_uart_custom *pDef);
int32_t DrvBspUart_GetNbOfDataInFifo(t_uart_custom *pDef);
uint32_t DrvBspUart_GetModBusTimeout(t_uart_custom *pDef);

#endif /* _DRV_BSP_UART_PRIVATE_H_ */
