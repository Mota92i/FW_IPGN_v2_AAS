/******************************************************************************/
/// \file           drv_display.h
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
#ifndef __DRV_DISPLAY_H__
#define __DRV_DISPLAY_H__

/******************************************************************************/
//  Included header file list
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
typedef enum {
	DISPLAY_BRIGHTNESS_DEFAULT	 	= 10,
	DISPLAY_BRIGHTNESS_MIN	 		= 2,
	DISPLAY_BRIGHTNESS_MAX	 		= 30
}t_display_brightness;

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
#define	DISPLAY_ALL_LED				(0x7F)
#define	DISPLAY_ALL_DIGIT			((char *)"8888888")

#define	DISPLAY_ALL_LED_OFF			(0x00)
#define	DISPLAY_ALL_DIGIT_OFF		((char *)"       ")

/******************************************************************************/
//  Multi-file-scope function prototype declarations
/******************************************************************************/
extern void voDrvDisplay_Init(uint8_t u8Lumin);
extern void voDrvDisplay_SetLum(uint8_t u8Lumin);
extern void voDrvDisplay_Digit(char *pString);
extern void voDrvDisplay_Led(uint8_t u8Led);
extern void voDrvDisplay_LedNum(uint8_t u8NumLed);
extern void voDrvDisplay_IpAdr(uint32_t u32IpAdr);

#endif /* __DRV_DISPLAY_H__ */
