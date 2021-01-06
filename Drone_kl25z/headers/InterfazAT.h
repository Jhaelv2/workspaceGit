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

typedef enum
{
	eATDesicion = 0,
	eOkoComando,
	eIdentificaComando,
}eControlAT;

void AT_Gestion(void);

#endif /* HEADERS_INTERFAZAT_H_ */
