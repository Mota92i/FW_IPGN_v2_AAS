/******************************************************************************/
/******************************************************************************/
/// \file           EEPROM.c
/// \date           2012-30-07
/// \copyright      Enerdis - Realisations speciales
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
    #include "drv_display.h"
#undef public

#define public extern
	#include "peripherals.h"
	#include "fsl_dspi.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
static const uint8_t TabDigit[] = {
		0x40,0x60,0x70,0x78,0x7C,0x7E,0x7F,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   // $00
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   // $10
		0x00,0x38,0x22,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,   // $20
		0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B,0x00,0x00,0x00,0x09,0x00,0x65,   // $30
		0x00,0x77,0x7F,0x4E,0x3D,0x4F,0x47,0x5E,0x37,0x30,0x38,0x37,0x0E,0x76,0x15,0x1D,   // $40
		0x67,0x1D,0x05,0x5B,0x0F,0x1C,0x3E,0x3E,0x37,0x3B,0x6D,0x4E,0x13,0x78,0x62,0x04,   // $50
		0x00,0x77,0x1F,0x0D,0x3D,0x6F,0x47,0x5E,0x37,0x10,0x38,0x37,0x0E,0x76,0x15,0x1D,   // $60
		0x67,0x1D,0x05,0x5B,0x0F,0x1C,0x3E,0x3E,0x67,0x3B,0x38,0x00,0x30,0x00,0x00,0x1D    // $70
};

static const uint8_t g_u8TabLed[] = { 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x80 };

/******************************************************************************/
//
/******************************************************************************/
typedef enum{
	NOOP	        	 = 0x00,
	REG_DECODE      	 = 0x01,
	REG_INTENSITY   	 = 0x02,
	REG_SCANLIMIT   	 = 0x03,
	REG_CONFIGURATION    = 0x04,
	REG_DISPLAY_TEST     = 0x07,

	Digit0_P0       	 = 0x20,
	Digit1_P0,
	Digit2_P0,
	Digit3_P0,
	Digit4_P0,
	Digit5_P0,
	Digit6_P0,
	Digit7_P0,

	Digit0_P1       	 = 0x40,
	Digit1_P1,
	Digit2_P1,
	Digit3_P1,
	Digit4_P1,
	Digit5_P1,
	Digit6_P1,
	Digit7_P1,

	Digit0          	 = 0x60,
	Digit1,
	Digit2,
	Digit3,
	Digit4,
	Digit5,
	Digit6,
	Digit7,
}t_Max6951_reg;

/******************************************************************************/
//  function prototype declarations
/******************************************************************************/
static void voWriteMax6951(uint8_t u8Cmde, uint8_t u8Data);

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
static void voWriteMax6951(uint8_t u8Cmde, uint8_t u8Data)
{
    dspi_transfer_t masterXfer;
	uint8_t buf[2];

	//
	buf[0] = u8Cmde;	// Address
	buf[1] = u8Data;	// Data

    // Start master transfer
    masterXfer.txData 		= &buf[0];
    masterXfer.rxData 		= NULL;
    masterXfer.dataSize 	= sizeof(buf);
    masterXfer.configFlags 	= kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

	DSPI_MasterTransferBlocking(SPI0, &masterXfer);
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
void voDrvDisplay_Init(uint8_t u8Lumin)
{
	voWriteMax6951(REG_SCANLIMIT, 0x07);	// Multiplexage sur 8 digits
	voWriteMax6951(REG_DECODE,    0x00);	// Pas décodage hexa sur ttes les digits
	voDrvDisplay_SetLum(u8Lumin);
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
void voDrvDisplay_SetLum(uint8_t u8Lumin)
{
	if( u8Lumin )
	{
		voWriteMax6951(REG_CONFIGURATION, 0x01);   	// Shutdown=0,Vit clignot=2Hz,clignot invalide,Synchro=0
		voWriteMax6951(REG_INTENSITY, u8Lumin - 1);
	}
	else
	{
		voWriteMax6951(REG_CONFIGURATION, 0x00);   	// Shutdown=1,Vit clignot=2Hz,clignot invalide,Synchro=0
	}
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
/// Affichage du buffer ASCII  "chaine" dans l'afficheur (afficher 7 digits)
/// Les caractéres sont codés sur 7 bit, le bit d7 correspond à
/// l'affichage de la virgule sur le digit
//----------------------------------------------------------------------------*/
void voDrvDisplay_Digit(char *pString)
{
	uint8_t u8Val;
	uint8_t i;

	for(i = Digit0_P0; i <= Digit6_P0; i++)
	{
		u8Val = TabDigit[(*pString) & 0x7F] | (uint8_t)(*pString & 0x80);
		voWriteMax6951(i, u8Val);

		pString++;
	}
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
/// Affichage des 7 leds
/// A l'appel : led contient les 7 bits d'�tat des leds
//----------------------------------------------------------------------------*/
void voDrvDisplay_Led(uint8_t u8Led)
{
	uint8_t u8Val = 0;
	uint8_t i;

	for(i = 0; i < (sizeof(g_u8TabLed) / sizeof(g_u8TabLed[0])); i++)
	{
		if( u8Led & 0x01 )
			u8Val |= g_u8TabLed[i];

		u8Led = u8Led >> 1;
	}

	voWriteMax6951(Digit7_P0, u8Val);
}

void voDrvDisplay_LedNum(uint8_t u8NumLed)
{
	if( u8NumLed < (sizeof(g_u8TabLed) / sizeof(g_u8TabLed[0])) ) // 7 led dispo uniquement
	{
		voWriteMax6951(Digit7_P0, g_u8TabLed[ u8NumLed ]);
	}
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
void voDrvDisplay_IpAdr(uint32_t u32IpAdr)
{
	const uint8_t shiftBit[] = {0,    12, 8,    20, 16,   28, 24};
	const uint8_t AddBit[]   = {0x80, 0,  0x80, 0,  0x80, 0,  0};
	uint8_t ip_str[(sizeof(shiftBit) / sizeof(shiftBit[0]))];
	uint8_t l;
	uint8_t i;

	for(i = 0; i < (sizeof(ip_str) / sizeof(ip_str[0])); i++)
	{
		l = (uint8_t)( (u32IpAdr >> shiftBit[i]) & 0x0F );
		if( l > 9 )
			l += '7';
		else
			l += '0';
		ip_str[i] = l | AddBit[i];
	}

	voDrvDisplay_Digit((char *)&ip_str[0]);
}
