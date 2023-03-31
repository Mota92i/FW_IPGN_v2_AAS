/******************************************************************************/
/// \file           os_semaphore.c
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
    #include "os_semaphore.h"
#undef public

#define public extern
	#include "FreeRTOS.h"
	#include "semphr.h"
#undef public

/******************************************************************************/
// CONSTANT DECLARATIONS
/******************************************************************************/
enum{
	SEMAPHORE_TIMER = 0,
	SEMAPHORE_EEPROM,

	SEMAPHORE_COUNT,
};

/******************************************************************************/
//  types declaration
/******************************************************************************/
typedef struct{
	SemaphoreHandle_t xHandle;
	StaticSemaphore_t xBuffer;
}t_semaphore;

/******************************************************************************/
// VARIABLE DECLARATIONS
/******************************************************************************/
static t_semaphore g_sSemaphore[ SEMAPHORE_COUNT ];

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
void voOsSemaphore_Init(void)
{
	uint8_t i;

	for(i = 0; i < SEMAPHORE_COUNT; i++)
	{
		g_sSemaphore[ i ].xHandle = xSemaphoreCreateBinaryStatic(&g_sSemaphore[ i ].xBuffer);

		if( g_sSemaphore[ i ].xHandle == NULL )
			while(1);
		else
			xSemaphoreGive( g_sSemaphore[ i ].xHandle );
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
void voOsSemaphore_MsDlyTmrTake(void)
{
	while( xSemaphoreTake( g_sSemaphore[ SEMAPHORE_TIMER ].xHandle, portMAX_DELAY ) != pdPASS );
}

void voOsSemaphore_MsDlyTmrGive(void)
{
	xSemaphoreGive( g_sSemaphore[ SEMAPHORE_TIMER ].xHandle );
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
void voOsSemaphore_EepromTake(void)
{
	while( xSemaphoreTake( g_sSemaphore[ SEMAPHORE_EEPROM ].xHandle, portMAX_DELAY ) != pdPASS );
}

void voOsSemaphore_EepromGive(void)
{
	xSemaphoreGive( g_sSemaphore[ SEMAPHORE_EEPROM ].xHandle );
}
