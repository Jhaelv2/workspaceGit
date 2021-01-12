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
ecommandList AT_Inquire_The_Command(eCommandType* peCurrentCmdType)
{
	int8_t i8TempCommandRetainer[gsAT.sizeOfBigestcommand], i8CurrentCommandSize = 0;
	ecommandList eCurrCommandIndex = eBAUDR;
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
 * Nombre:      AT_Gestion
 * Descripcion: Examina el bufferRx para buscar un comndo AT
 * Parametros:  Ninguno-
 * Retornos:    NInguno.
 * ToDo:        Averiguar el tamano de el comando introducido.
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
		while(indiceAyT < 2)
		{
			if(UART_Pull_Buffer_Rx(&letrasAT[indiceAyT])==Completo);
			indiceAyT++;
		}
		if((letrasAT[0] == 'A') && (letrasAT[1] == 'T'))
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
				default :
					flujoAT = eComandoProcesado;
					//break;
				}
		if(flujoAT == eContinuarProcesando)
			eCommandDetected = AT_Inquire_The_Command(&eDetectedCmdType);
		switch(eCommandDetected)
		{
		case eBAUDR:
			if(eDetectedCmdType == eWrite);

			else if(eDetectedCmdType == eRead);
			break;
		case ePWM:
			if(eDetectedCmdType == eWrite);
			else if(eDetectedCmdType == eRead);
			break;
		case ePWMDUTTY:
			if(eDetectedCmdType == eWrite);
			//Aqui escribe el codigo.
			else if(eDetectedCmdType == eRead);
			break;
		case eElpePe:
			if(eDetectedCmdType == eWrite);
			else if(eDetectedCmdType == eRead);
			break;
		case e123456789:
			if(eDetectedCmdType == eWrite);
			else if(eDetectedCmdType == eRead);
			break;
		case eUnknownComand:
			if(eDetectedCmdType == eWrite);
			else if(eDetectedCmdType == eRead);
			break;
		}
	}
}
