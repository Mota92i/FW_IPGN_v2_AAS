/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/**
 * @file    MK64FX512xxx12_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"

#define public extern
    #include "task_main.h"
#undef public

/*******************************************************************************
 * Definitions
 ******************************************************************************/
	// Task priorities.
	#define PRIORITY_TASK_MAIN 	(configMAX_PRIORITIES - 2)

	// Dimensions the buffer that the task being created will use as its stack.
	// NOTE:  This is the number of words the stack will hold, not the number of
	// bytes.  For example, if each stack item is 32-bits, and this is set to 100,
	// then 400 bytes (100 * 32-bits) will be allocated.
	#define TASK_MAIN_STACK_SIZE	( 2000 )

    // Structure that will hold the TCB of the task being created.
    StaticTask_t g_xTaskMainBuffer;

    // Buffer that the task being created will use as its stack.  Note this is
    // an array of StackType_t variables.  The size of StackType_t is dependent on
    // the RTOS port.
    StackType_t g_xTaskMainStack[ TASK_MAIN_STACK_SIZE ];


/*
 * @brief   Application entry point.
 */
int main(void) {
    TaskHandle_t xHandle = NULL;

  	// Init board hardware.
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

	// Create the task without using any dynamic memory allocation.
	xHandle = xTaskCreateStatic(
					task_main,       		// Function that implements the task.
					"task_main",          	// Text name for the task.
					TASK_MAIN_STACK_SIZE,   // Number of indexes in the xStack array.
					( void * ) 1,    		// Parameter passed into the task.
					PRIORITY_TASK_MAIN,		// Priority at which the task is created.
					g_xTaskMainStack,       // Array to use as the task's stack.
					&g_xTaskMainBuffer );  	// Variable to hold the task's data structure.
	while( xHandle == NULL );

    vTaskStartScheduler();
    for (;;)
        ;

    return 0 ;
}

