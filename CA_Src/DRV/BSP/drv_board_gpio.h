/******************************************************************************/
/// \file           drv_board_gpio.c
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
#ifndef __DRV_BOARD_GPIO_H__
#define __DRV_BOARD_GPIO_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
typedef enum {
	DISPLAY_LUM_GENE	 	= 0,
	DISPLAY_LUM_ZONE_1,
	DISPLAY_LUM_ZONE_2,
	DISPLAY_LUM_ZONE_3,

	DISPLAY_LUM_COUNT
}t_display_lum;

typedef enum {
	VOLTMETRE_VOIE_AUTOZERO	= 0,
	VOLTMETRE_VOIE_COURANT,

	VOLTMETRE_VOIE_COUNT
}t_voltmetre_voie;

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voDrvGpio_OffLedLum(uint8_t u8Num);
extern void voDrvGpio_OnLedLum(uint8_t u8Num);
extern uint8_t u8DrvGpio_ReadLedCmd(uint8_t u8Num);

extern void voDrvGpio_VoltmetreSelectVoie(uint8_t u8Num);
extern uint32_t u32DrvGpio_VoltmetreReadBascule(void);

extern void DrvGpio_DebugToggle(void);

extern void DrvGpio_ResetPhy(void);
extern void DrvGpio_NoResetPhy(void);

extern uint8_t DrvGpio_GetSwitchState(void);

extern uint8_t DrvGpio_GetUart0RxLevel(void);
#endif /* __DRV_BOARD_GPIO_H__ */

