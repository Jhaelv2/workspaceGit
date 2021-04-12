/*
 * InterfazAT.c
 *
 *  Created on: 22 dic. 2020
 *      Author: Hugo
 */
#include "InterfazAT.h"
#include  <string.h>
int8_t* Comandos[] =
{
		(int8_t*)"BAUDR",  //Baud rate request, pregunta el baud rate actual.
		(int8_t*)"PWM",
		(int8_t*)"PWMDUTTY",
		(int8_t*)"ElpePe",
		(int8_t*)"123456789"
};
sAT gsAT;

/**********************************************************************************************
 * Name:        AT_Size_of_command
 * Description: finds the size of the biggest strig and returns it
 * Parameters:  None.
 * Returns:     eAT.sizeOfBigestcommand: is the overall maximum size of the commands.
 *********************************************************************************************/
void AT_Size_of_command(void)
{
	int8_t  iMaxPosibleSize = 20;
	int8_t  i8CurrentCommand = 0, quantityOfChars = 0;
	int8_t* pCommandChar = Comandos[0];
	gsAT.sizeOfBigestcommand = 0;
	gsAT.i8QtyOfCommamds = (sizeof(Comandos)/4);

	while((i8CurrentCommand < gsAT.i8QtyOfCommamds))
	{
		while((*pCommandChar != '\0') && (quantityOfChars < iMaxPosibleSize))
		{
			pCommandChar++;
			quantityOfChars++;
		}
		if(quantityOfChars > gsAT.sizeOfBigestcommand)
				gsAT.sizeOfBigestcommand = quantityOfChars;
		quantityOfChars = 0;
		i8CurrentCommand ++;

		if((i8CurrentCommand < gsAT.i8QtyOfCommamds) && (*pCommandChar == '\0'))
			while(*pCommandChar == '\0')
				pCommandChar++;
	}

}

/*********************************************************************************************
 * Name:        AT_Inquire_The_Command
 * Description: First it stores
 ********************************************************************************************/
uint8_t AT_Inquire_The_Command(uint8_t *peCurrentCmdType)
{
	int8_t i8TempCommandRetainer[gsAT.sizeOfBigestcommand], i8CurrentCommandSize = 0;
	ecommandList eCurrCommandIndex = eBAUDR;
	int8_t tempHolder;
	*peCurrentCmdType = eUnknown;
	while((i8CurrentCommandSize < (gsAT.sizeOfBigestcommand +1)) && (*peCurrentCmdType == eUnknown))
	{
		if(UART_Pull_Buffer_Rx(&i8TempCommandRetainer[i8CurrentCommandSize]) == Completo)
		{
			switch((i8TempCommandRetainer[i8CurrentCommandSize]))
			{
			case 0x0D:
				*peCurrentCmdType = eExecute;
				break;
			case '=':
				*peCurrentCmdType = eWrite;
				break;
			case '?':
				*peCurrentCmdType = eRead;
				break;
			default:
				*peCurrentCmdType = eUnknown;
				break;
			}
		}
		i8CurrentCommandSize++;
		if((i8CurrentCommandSize >= (i8CurrentCommandSize +1)) && (*peCurrentCmdType == eUnknown))
			*peCurrentCmdType = eUnrrecognized;

	}
	i8TempCommandRetainer[(i8CurrentCommandSize - 1)] = '\0';

	if(*peCurrentCmdType == eRead)
	{
		if(UART_Pull_Buffer_Rx(&tempHolder) == Completo)
		{
			if(tempHolder != '\r')
				return eUnknownComand;
		}
	}

	if((*peCurrentCmdType != eUnknown) && (*peCurrentCmdType != eUnrrecognized))
	{

		while(eCurrCommandIndex < gsAT.i8QtyOfCommamds)
		{
			if(strcmp((char*)i8TempCommandRetainer,(char*)Comandos[eCurrCommandIndex]) == 0)
				return eCurrCommandIndex;
			eCurrCommandIndex++;
		}
	}
	return eUnknownComand;
}

/********************************************************************************************************
 * Name: AT_PwmNewDUTTY
 *******************************************************************************************************/
/********************************************************************************************************
 * Name: AT_BaudRateRead
 *******************************************************************************************************/
void AT_BaudRateRead(void)
{
	UART_Buffer_Tx((int8_t*)"\n\rEl baud rate actual es: \n\r");
	UART0_BaudRate_Consulta();
}

/********************************************************************************************************
 * Name: AT_BaudRateWrite
 *******************************************************************************************************/
void AT_BaudRateWrite(void)
{
	uint8_t  contador = 0;
	int8_t   numeroActual[10];
	uint32_t numeroResultante = 0;
	while ((numeroActual[contador] != '\r') && (contador < 10))
	{
		if(UART_Pull_Buffer_Rx(&numeroActual[contador]) == Completo);
		contador++;
	}
	numeroActual[contador] = '\0';
	numeroResultante = atoi((char*)numeroActual);
	UART_Buffer_Tx((int8_t*)"\n\rhello\n\r");
	UART0_Nuevo_BaudRate(numeroResultante);
}
/********************************************************************************************************
 * Nombre:      AT_Gestion
 * Descripcion: Examina el bufferRx para buscar un comndo AT
 * Parametros:  Ninguno-
 * Retornos:    NInguno.
 * ToDo:        Asegurate de que al terminar de procesar cualquier comando;
 *              1: No queden otros caracteres residuales antes del '\r' en caso de que si
 *              el comando es fallido
 *
 *
 *******************************************************************************************************/
void AT_Gestion(void)
{
	int8_t       letrasAT[2], indiceAyT = 0;
	eCommandType eDetectedCmdType = eUnknown;
	ecommandList eCommandDetected = eUnknownComand;

	static uint8_t flujoAT = eATDesicion;

	switch(flujoAT)
	{
	case eATDesicion:
		if(gsUart.Rx.Status.Bits.DatoAt == 1)
		{
			while(indiceAyT < 2)
			{
				if(UART_Pull_Buffer_Rx(&letrasAT[indiceAyT])==Completo);
				indiceAyT++;
			}
		}
		else
			break;

		if((letrasAT[0] == 'A') && (letrasAT[1] == 'T'))
		{
			if(UART_Pull_Buffer_Rx(&letrasAT[0]) == Completo)
				switch (letrasAT[0])
				{
				case 0x0D:
					UART_Buffer_Tx((int8_t*)"\n\rOk\n\r");
					flujoAT = eComandoProcesado;
					break;
				case '+':
					flujoAT = eContinuarProcesando;
					break;
				case '?':
				    flujoAT = eContinuarProcesando;
				    break;
			    default :
			    	UART_Buffer_Tx((int8_t*)"\n\rFallo en antes de procesar\n\r");
					flujoAT = ComandoFallido;
					break;
				}
		}
		else
		{
	    	UART_Buffer_Tx((int8_t*)"\n\rFAT incompleto\n\r");

			flujoAT = ComandoFallido;
			break;
		}

		if(flujoAT == eContinuarProcesando)
		{
			eCommandDetected = AT_Inquire_The_Command(&eDetectedCmdType);
			switch(eCommandDetected)
			{
			case eBAUDR:
				if(eDetectedCmdType == eWrite)
					AT_BaudRateWrite();
				else if(eDetectedCmdType == eRead)
					AT_BaudRateRead();
				flujoAT = eComandoProcesado;
				break;
			case ePWM:
				UART_Buffer_Tx((int8_t*)"\n\riii q puto\n\r");
				if(eDetectedCmdType == eWrite);
				else if(eDetectedCmdType == eRead);
				flujoAT = ComandoFallido;
				break;
		    case ePWMDUTTY:
		    	UART_Buffer_Tx((int8_t*)"\n\rsi lees esto eres gay\n\r");
			    if(eDetectedCmdType == eWrite);
			     //Aqui escribe el codigo.
			    else if(eDetectedCmdType == eRead);
			    flujoAT = ComandoFallido;
			    break;
		     case eElpePe:
		    	 UART_Buffer_Tx((int8_t*)"\n\rYOU are such A BIG FAGGOT\n\r");
			    if(eDetectedCmdType == eWrite);
			    else if(eDetectedCmdType == eRead);
			    flujoAT = ComandoFallido;
			    break;
//		    case e123456789:
//			    if(eDetectedCmdType == eWrite);
//			    else if(eDetectedCmdType == eRead);
//			    break;
//		    case eUnknownComand:
//			    if(eDetectedCmdType == eWrite);
//			    else if(eDetectedCmdType == eRead);
//			    break;
			default:
				UART_Buffer_Tx((int8_t*)"\n\rFallo en Procesando\n\r");
				flujoAT = ComandoFallido;
				break;
		    }
			if(UART_Pull_Buffer_Rx(&letrasAT[0])==Completo);
//			{
//				if(letrasAT[0] != '\r')
//					flujoAT = ComandoFallido;
//			}
		}
		//
		//flujoAT = eATDesicion;
		//gsUart.Rx.Status.Bits.DatoAt = 0;
		break;

	case ComandoFallido:
		while((letrasAT[0] != '\r') && (gsUart.Rx.Status.Bits.DatoDisponible != 0))
		{
			if(UART_Pull_Buffer_Rx(&letrasAT[0])==Completo);
		}
		flujoAT = eATDesicion;
		gsUart.Rx.Status.Bits.DatoAt = 0;
		break;

    case eComandoProcesado:
		flujoAT = eATDesicion;
		gsUart.Rx.Status.Bits.DatoAt = 0;
		break;
	}

}

