/*
Este es un Codigo fuente para la biblioteca de Drivers
del Dron-Carga beta 0
 

@file System.c
@date 18 mar. 2021
@author Madera
@brief System

@version 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include "System.h"
#include "fsl_debug_console.h"


void System_exit(int val) {
	for(;;) {
		/* we don't do a shutdown with exit(), as this is will affect debugging.
		 * Instead, we stay here in an endless loop.
		 */
		__asm("nop"); /* burning some CPU cycles here */
	}
	// exit(val);
}

/* Stub: Initialize your hardware here */
void System_Init(void)
{
	PRINTF("Init of hardware finished.\n");
}

/* Stub: Shutdown your hardware here */
void System_Shutdown(void)
{

	/* asm("\tSTOP"); */
	PRINTF("System shutdown.\n");
	System_exit(0);
}

/* Stub: Recover the system */
void System_Recover(void)
{
	/* Stub: Recover the hardware */
	/* asm("\tRESET"); */
	PRINTF("System reset.\n");
	System_exit(0);
}

/* Stub: Put system in a safe state */
void System_Safestate(void)
{
	/* Disable all port pins */
	/* PORTA = 0x0000; */
	/* PORTB = 0x0000; */
	/* PORTC = 0x0000; */

	/* Disable interrupts */
	/* DIE(); */

	/* Put processor into idle state */
	/* asm("\tIDLE"); */
	PRINTF("System safe state.\n");
	System_exit(0);
}

/* Stub: Transmit a string to the host/debugger/simulator */
void System_WriteString(char * msg)
{
	PRINTF(msg);
}

void System_WriteInt(int n)
{
	PRINTF("%d", n);
}
