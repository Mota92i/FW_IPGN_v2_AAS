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
//  Included header file list
/******************************************************************************/
#define public
    #include "drv_board_gpio.h"
#undef public

#define public extern
	#include "fsl_gpio.h"
	#include "pin_mux.h"
#undef public

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voDrvGpio_OnOffLedLum(uint8_t u8Num, uint8_t u8State);

/******************************************************************************/
//  Function definitions
/******************************************************************************/

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
static void voDrvGpio_OnOffLedLum(uint8_t u8Num, uint8_t u8State)
{
	const struct{
		GPIO_Type * const pPort;
		uint32_t  pin;
	} sgpio[] = {
		[ DISPLAY_LUM_GENE   ] = { BOARD_INITPINS_LUM_GEN_GPIO, 	BOARD_INITPINS_LUM_GEN_PIN    },
		[ DISPLAY_LUM_ZONE_1 ] = { BOARD_INITPINS_LUM_ZONE_1_GPIO,	BOARD_INITPINS_LUM_ZONE_1_PIN },
		[ DISPLAY_LUM_ZONE_2 ] = { BOARD_INITPINS_LUM_ZONE_2_GPIO,	BOARD_INITPINS_LUM_ZONE_2_PIN },
		[ DISPLAY_LUM_ZONE_3 ] = { BOARD_INITPINS_LUM_ZONE_3_GPIO,	BOARD_INITPINS_LUM_ZONE_3_PIN },
	};

	if( u8Num < DISPLAY_LUM_COUNT )
	{
		GPIO_PinWrite(sgpio[ u8Num ].pPort, sgpio[ u8Num ].pin, u8State);
	}
}

void voDrvGpio_OffLedLum(uint8_t u8Num)
{
	voDrvGpio_OnOffLedLum(u8Num, 1);
}

void voDrvGpio_OnLedLum(uint8_t u8Num)
{
//	GPIO_PinWrite(BOARD_INITPINS_LUM_ENABLE_GPIO, BOARD_INITPINS_LUM_ENABLE_PIN, 0);
	voDrvGpio_OnOffLedLum(u8Num, 0);
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2020-01-13
///
//----------------------------------------------------------------------------*/
void voDrvGpio_VoltmetreSelectVoie(uint8_t u8Num)
{
	if( u8Num >= VOLTMETRE_VOIE_COUNT )
		u8Num = VOLTMETRE_VOIE_AUTOZERO;

	GPIO_PinWrite(BOARD_INITPINS_VOLTMETRE_VOIE_GPIO, BOARD_INITPINS_VOLTMETRE_VOIE_PIN, u8Num);
}

uint32_t u32DrvGpio_VoltmetreReadBascule(void)
{
	return( GPIO_PinRead(BOARD_INITPINS_VOLTMETRE_READ_BASCULE_GPIO, BOARD_INITPINS_VOLTMETRE_READ_BASCULE_PIN) );
}

//----------------------------------------------------------------------------*/
//
//----------------------------------------------------------------------------*/
/// \param[in]      none
/// \param[out]     none
/// \return         none
///
/// \version        1.0
/// \date           2018-10-11
///
//----------------------------------------------------------------------------*/
uint8_t u8DrvGpio_ReadLedCmd(uint8_t u8Num)
{
	uint8_t u8Level;
	const struct{
		GPIO_Type * const pPort;
		uint32_t  pin;
	} sgpio[] = {
		[ DISPLAY_LUM_GENE   ] = { (GPIO_Type *)0, 						0    			  				  },
		[ DISPLAY_LUM_ZONE_1 ] = { BOARD_INITPINS_CDE_LUM_ZONE_1_GPIO,	BOARD_INITPINS_CDE_LUM_ZONE_1_PIN },
		[ DISPLAY_LUM_ZONE_2 ] = { BOARD_INITPINS_CDE_LUM_ZONE_2_GPIO,	BOARD_INITPINS_CDE_LUM_ZONE_2_PIN },
		[ DISPLAY_LUM_ZONE_3 ] = { BOARD_INITPINS_CDE_LUM_ZONE_3_GPIO,	BOARD_INITPINS_CDE_LUM_ZONE_3_PIN },
	};

	if( u8Num == DISPLAY_LUM_GENE )
		u8Level = 1;
	else
		u8Level = ~GPIO_PinRead(sgpio[ u8Num ].pPort, sgpio[ u8Num ].pin) & 0x01;

	return( u8Level );
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]
/// \param[out]
/// \return
///
/// \version        1.0
/// \date           2020-03-09
///                 
/*----------------------------------------------------------------------------*/
void DrvGpio_DebugToggle(void)
{
	static uint8_t u8Level = 0;

	u8Level = ~u8Level & 0x01;
	GPIO_PinWrite(BOARD_INITPINS_DEBUG_PORT_E0_GPIO, BOARD_INITPINS_DEBUG_PORT_E0_PIN, u8Level);
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]
/// \param[out]
/// \return
///
/// \version        1.0
/// \date           2020-03-09
///
/*----------------------------------------------------------------------------*/
void DrvGpio_ResetPhy(void)
{
	GPIO_PinWrite(BOARD_INITPINS_RST_ETH_GPIO, BOARD_INITPINS_RST_ETH_PIN, 0);
}

void DrvGpio_NoResetPhy(void)
{
	GPIO_PinWrite(BOARD_INITPINS_RST_ETH_GPIO, BOARD_INITPINS_RST_ETH_PIN, 1);
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]
/// \param[out]
/// \return
///
/// \version        1.0
/// \date           2020-03-09
///
/*----------------------------------------------------------------------------*/
uint8_t DrvGpio_GetSwitchState(void)
{
	uint8_t val;

	val = GPIO_PinRead(BOARD_INITPINS_REG_LUM_GEN_GPIO, BOARD_INITPINS_REG_LUM_GEN_PIN) ^ 0x01; //(inversion)

	return( val & 0x01 );
}

/*----------------------------------------------------------------------------*/
//
/*----------------------------------------------------------------------------*/
/// \param[in]
/// \param[out]
/// \return
///
/// \version        1.0
/// \date           2020-03-09
///
/*----------------------------------------------------------------------------*/
uint8_t DrvGpio_GetUart0RxLevel(void)
{
	uint8_t val = GPIO_PinRead(BOARD_INITPINS_UART0_BREAK_DETECT_IN_GPIO, BOARD_INITPINS_UART0_BREAK_DETECT_IN_PIN);

	return( val );
}
