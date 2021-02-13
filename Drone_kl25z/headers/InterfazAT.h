/*
 * InterfazAT.h
 *
 *  Created on: 22 dic. 2020
 *      Author: Hugo
 */

#ifndef HEADERS_INTERFAZAT_H_
#define HEADERS_INTERFAZAT_H_


#include "MKL25Z4.h"
#include "UART.h"


extern int8_t* Comandos[];

typedef enum
{
	eATDesicion = 0,
	eOkoComando,
	eIdentificaComando,
	eContinuarProcesando,
	eComandoProcesado,
}eControlAT;

typedef enum
{
	eBAUDR = 0,
	ePWM,
	ePWMDUTTY,
	eElpePe,
	e123456789,
	eUnknownComand
}ecommandList;


typedef enum
{
	eUnknown= 0,
	eRead,
	eTest,
	eWrite,
	eExecute,
	eUnrrecognized,
}eCommandType;

typedef struct
{
	int8_t sizeOfBigestcommand;
	int8_t i8QtyOfCommamds;
}sAT;

void         AT_Size_of_command     (void);
ecommandList AT_Inquire_The_Command (eCommandType* peCurrentCmdType);
void         AT_BaudRateRead        (void);
void         AT_Gestion             (void);


#endif /* HEADERS_INTERFAZAT_H_ */
