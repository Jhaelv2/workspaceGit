/*
 * InterfazAT.c
 *
 *  Created on: 22 dic. 2020
 *      Author: Hugo
 */
#include "InterfazAT.h"
#include "string.h"
/*
 * Nombre:      AT_Gestion
 * Descripcion: Examina el bufferRx para buscar un comndo AT
 * */
void AT_Gestion(void)
{
	uint8_t letrasAT[2], indiceAyT = 0;
	static uint8_t flujoAT = eATDesicion;

	switch(flujoAT)
	{
	case eATDesicion:
		while(indiceAyT >= 2)
		{
			if(UART_Pull_Buffer_Rx(&letrasAT[indiceAyT])==Completo);
			indiceAyT++;
		}
		if((letrasAT[0] == 'A') && (letrasAT[1] == 'T'))
			flujoAT = eOkoComando;
	break;

	case eOkoComando:
		if(UART_Pull_Buffer_Rx(&letrasAT[0]) == Fallo)
			break;
		switch(letrasAT[0])
		{
		case 0x0D:

		break;
		}

	break;
	}
}
