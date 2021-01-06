/*
 * UART.c
 *
 *  Created on: 12 dic. 2020
 *      Author: Hugo
 */


#include "UART.h"
#include <stdio.h>
sUart gsUart;
uint8_t TAMANOX;

/*-----------------------------------------------------------------------------+
|    Configuracion del modulo UART0                                            |
+------------------------------------------------------------------------------+
|  ToDo: Añadir maquina de estados para transmitir un tipo de dato (no string) |
|        al buffer de Tx, o en su defecto solucionar el warning de sprintf     |
+-----------------------------------------------------------------------------*/

/********************************************************************************************************************************
*Nombre:       UARTx_Configuracion
*
*Descripcion:  Configura los regisgtros de modulo UART0 a 9600 bps
*
*Entradas:     Parametros:     * puTxBuffer, u8TxBufferSize
*
*Salidas:      Ninguna.
********************************************************************************************************************************/
void UARTx_Configuracion(uint8_t * puTxBuffer, uint8_t u8TxBufferSize)
{
	NVIC_ClearPendingIRQ(UART0_IRQn );        //Deshabilita interrupciones UART

	SIM->SCGC4 |= SIM_SCGC4_UART0(1);               //Habilita Clock a UART2
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

	PORTA->PCR[1] |= PORT_PCR_MUX(3);                 //Puerto D pin 2 como alternativa 3 UART_2 Rx
	PORTA->PCR[2] |= PORT_PCR_MUX(3);                 //Puerto D pin 3 como alternativa 3 UART_2 Tx

	UART0->C2  |= UART_C2_RIE(0) | UART_C2_TE(0)    //Deshabilita las funciones Rx y Tx
			   |  UART_C2_RE(0);                    //Deshabilita sus correspondientes interrupciones


	UART0->C3      = 0;
    UART0->C1      = 0;

	//                    UarttModuleClock   UarttModuleClock = Core Clock =  20.97152MHz.
	//UART baud rate = ----------------------
	//                   ((OSR+1)xBR).

	UART0->C4  = UART0_C4_OSR (7);
	UART0->BDH = UART0_BDH_SBR(0x01);
	UART0->BDL = UART0_BDL_SBR(0x11);

	UART0->C2 |= UART_C2_RIE(1)
			  |  UART_C2_TE(1)
			  |  UART_C2_RE(1);

	gsUart.Rx.u16IndiceUltimoDatoUSaliente = 0;
	gsUart.Rx.u16IndiceUltimoDatoEntrante = 0;
	gsUart.Tx.u16PosicionDatoTransmitido = 0;
	gsUart.Tx.u8TamanoDeBuffer = u8TxBufferSize;
    gsUart.Tx.pu8BufferTx = puTxBuffer;
    gsUart.Rx.Status.Byte = 0;
	NVIC_EnableIRQ(UART0_IRQn);
	TAMANOX = sizeof(gsUart);
}

/********************************************************************************************************************************
*Nombre:       UART_Push_Buffer_Rx
*
*Descripcion:  Llena el buffer "DatosRecibidos" con los datos obtenidos en
*              rutina de interrupcion de Rx.
*
*Entradas:     u8DatoCapturado: Es el dato de 8 Bitss que entro mediante Rx
*
*Salidas:      Ninguna.
********************************************************************************************************************************/
void UART_Push_Buffer_Rx(uint8_t u8ByteRxD)
{
	gsUart.Rx.u8Buffer[gsUart.Rx.u16IndiceUltimoDatoEntrante] = u8ByteRxD;
	gsUart.Rx.u16IndiceUltimoDatoEntrante ++;

	if(gsUart.Rx.u16IndiceUltimoDatoEntrante == RxBufferSize)
	gsUart.Rx.u16IndiceUltimoDatoEntrante = 0;
	gsUart.Rx.Status.Bits.DatoDisponible = 1;
	return;
}
/********************************************************************************************************************************
*Nombre:       UART_ExtraerDatosDeBufferRx
*
*Descripcion:  Extrae los Bytes almacenados en el buffer de RX
*
*Entradas:     direccion de dato donde se almacenara el byte
*
*Salidas:     uint8_t SUCCES FAIL.
********************************************************************************************************************************/
proceso UART_Pull_Buffer_Rx (uint8_t * pu8Dato)
{
	//uint8_t u8Dato = 0;
	if(gsUart.Rx.Status.Bits.DatoDisponible)
	{
		 *pu8Dato  = gsUart.Rx.u8Buffer[gsUart.Rx.u16IndiceUltimoDatoUSaliente];
		 gsUart.Rx.u16IndiceUltimoDatoUSaliente ++;
	     if(gsUart.Rx.u16IndiceUltimoDatoUSaliente == RxBufferSize)
			 gsUart.Rx.u16IndiceUltimoDatoUSaliente = 0;
		 if(gsUart.Rx.u16IndiceUltimoDatoUSaliente == gsUart.Rx.u16IndiceUltimoDatoEntrante)
			 gsUart.Rx.Status.Bits.DatoDisponible = 0;
		 return Completo;
	}
	return Fallo;
}
/***********************************************************************************************************************************
*Nombre:       UART_Buffer_TxServicio
*
*Descripcion:  Se encarga de que el siguiente dato introducido sea posicionado en la siguiente posicion
*              disponible del BufferTx, si el buuffer esta lleno reinica las posicionne (siguiente = 0)
*
*Parametros:   DatoParaBuffer; Bitse de entrada a ser almacenado en el buffer de transmicion.
*
*Return:       Ninguno.
***********************************************************************************************************************************/
void UART_Buffer_TxServicio(uint8_t * pu8DatoParaBuffer)
{
   *(gsUart.Tx.pu8BufferTx + gsUart.Tx.u16PosicionDatoPortransmitir) = * pu8DatoParaBuffer;
	gsUart.Tx.u16PosicionDatoPortransmitir++;

	if(gsUart.Tx.u16PosicionDatoPortransmitir == gsUart.Tx.u8TamanoDeBuffer)
	gsUart.Tx.u16PosicionDatoPortransmitir = 0;
	gsUart.Rx.Status.Bits.DatoDisponible = 1;
	return;
}

/********************************************************************************************************************************
*Nombre:        UART_Buffer_Tx
*
*Definition:    Recibe una cadena de caracteres de 8 Bitss y la usa para llenar (Byte a Byte) el
*               el buffer de transmicion de datos.
*
*Entradas:      pu8Buffer; Parametro puntero con la direccion del buffer a transmitir
*
*Salidas:       Ninguna.
********************************************************************************************************************************/
void UART_Buffer_Tx (uint8_t * pu8Buffer )
{
	uint8_t ValorDeByte = *pu8Buffer;
	while(ValorDeByte != '\0')
	{
		UART_Buffer_TxServicio(&ValorDeByte);
		ValorDeByte = *(++pu8Buffer);
	}
	return;
}

/********************************************************************************************************************************
*Nombre:        UART_CharTo_BufferTx
*
*Definition:    Recibe un dato numerico de tamaño char (8Bits), el cual es procesado para ser
*               para guardar sus unidades, decenas y centenas en un buffer, dicho bugffer es
*               introducido como parametro en la funcion "TansmiteBuffer" la cual tambien
*               recibe como parametro el tamaño de dicho buffer.
*
*Entradas:      u8Char; es la variable que contiene al numero de 8 bists a transmitir
*
*Salidas:       Ninguna.
********************************************************************************************************************************/
void UART_Char_To_BufferTx(uint8_t u8Char)
{
	uint8_t u8CharAString[3];
	sprintf(u8CharAString, "%d", u8Char);
	UART_Buffer_Tx(u8CharAString);
}

/********************************************************************************************************************************
*Nombre:        UART_Short_To_Buffer_Tx
*
*Definition:    Recibe un dato numerico de tipo Short, el cual es procesado mediante la funcion
*               "sprintf" la cual llena un arreglo con los digitos en ascii del numero.
*
*
*Entradas:      u16DatoShort; es la variable que contiene al numero de tipo short a transmitir
*
*Salidas:       Ninguna.
********************************************************************************************************************************/
void UART_Short_To_Buffer_Tx (uint16_t u16DatoShort)
{
	uint8_t u8ShortAString[5];
	sprintf(u8ShortAString,"%u", u16DatoShort);
	UART_Buffer_Tx(u8ShortAString);
}

/********************************************************************************************************************************
*Nombre:        UART_Int_To_Buffer_Tx
*
*Definition:    Recibe un dato numerico de tipo Int, el cual es procesado para ser
*               introducido en  buffer es
*               introducido como parametro en la funcion "TansmiteBuffer" la cual tambien
*               recibe como parametro el tamaño de dicho buffer.
*
*Entradas:      u32DatoInt; Es la variable que contiene al numero tipo Int a transmitir
*
*Salidas:       Ninguna.
********************************************************************************************************************************/
void  UART_Int_To_Buffer_Tx(uint32_t u32DatoInt)
{
	uint8_t u8IntAString[10];
	sprintf(u8IntAString,"%u", u32DatoInt);
	UART_Buffer_Tx(u8IntAString);
	return;
}

/********************************************************************************************************************************
*Nombre:        UART_FloatToBufferTx
*
*Definition:    Recibe un dato numerico de tipo float, el cual es procesado para ser
*               introducido en  un buffer el cual entra como parametro en la funcion
*               "TansmiteBuffer"
*
*Entradas:      u8DatoFloat; es la variable que contiene al numero de 8 bists a transmitir
*
*Salidas:       Ninguna.
********************************************************************************************************************************/
void  UART_Float_To_Buffer_Tx(float  u8DatoFloat)
{
	uint8_t u8FloatAString[10];
	sprintf(u8FloatAString,"%f", u8DatoFloat);
	UART_Buffer_Tx(u8FloatAString);
	return;
}

/********************************************************************************************************************************
*Nombre:       UART_Gestion_Tx
*
*Descripcion:  Maquina de estados para la gestion de la da salida de datos del Buffer Tx
*
*Entradas:     *BufferATransmitir: Variable puntero que apunta a la direccion del buffer
*              a transmitir.
*
*Salidas:      Ninguna.
********************************************************************************************************************************/
void UART_Gestion_Tx(void)
{
	static uint8_t eBufferTx = SMTX_ConsultaDatoDisponible;

	switch(eBufferTx)
	{
    case SMTX_ConsultaDatoDisponible:
    	if(gsUart.Tx.u16PosicionDatoPortransmitir != gsUart.Tx.u16PosicionDatoTransmitido)
    	{
    		eBufferTx = SMTX_TtansmiteDatoActual;
    	}
    	break;

    case SMTX_TtansmiteDatoActual:
	    (UART0->S1);
	    (UART0->D);
    	UART0->D   = gsUart.Tx.pu8BufferTx[gsUart.Tx.u16PosicionDatoTransmitido];
    	eBufferTx = SMTX_ConsultaTransmicionCompleta;
    	break;

    case SMTX_ConsultaTransmicionCompleta:
    	if(UART0->S1 & UART_S1_TDRE_MASK)
    	{
    		gsUart.Tx.u16PosicionDatoTransmitido++;
    		if(gsUart.Tx.u16PosicionDatoTransmitido == gsUart.Tx.u8TamanoDeBuffer)
    			gsUart.Tx.u16PosicionDatoTransmitido = 0;
    		eBufferTx = SMTX_ConsultaDatoDisponible;
    		UART0->C2 |=  UART_C2_TE(0);
    		UART0->C2 |=  UART_C2_TE(1);
    	}
    	break;
	}
	return;
}

/********************************************************************************************************************************
*Nombre:        UART0_BaudRate_Consulta
*
*Definicion:    Consulta los valores de los registros que configuran el baud rate del UART0
*               y los procesa para calcular el baud rate actual del modulo.
*
*Parametros:      Ninguno
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void UART0_BaudRate_Consulta(void)
{
	uint8_t  u8SBRHVal;
	uint16_t u8SBRTotalVal;
	uint8_t  u8OSR;
	float    u32BaudRateActual;

	u8SBRHVal     = UART0->BDL;
	u8SBRTotalVal = (UART0->BDH & 0b00011111);
	u8SBRTotalVal = (u8SBRTotalVal << 8) | u8SBRHVal;
	u8OSR         = (UART0->C4 & 0b00011111);

	u32BaudRateActual = ((DEFAULT_SYSTEM_CLOCK)/((u8OSR+1)*u8SBRTotalVal));
	UART_Short_To_Buffer_Tx(u32BaudRateActual);
	return;
}
/********************************************************************************************************************************
*Nombre:        UART0_Nuevo_BaudRate
*
*Definition:    Permite que el usuario especifique un nevo valor de baudrate para el cual seran
*               calculados los valores de OSR, BRH Y BRL necesarios.
*
*Entradas:      u8BaudRate:
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void UART0_Nuevo_BaudRate(uint32_t u8BaudRate)
{
	float    u32NuevoSBR;
	uint32_t u8NuevoSBRH;


	u32NuevoSBR = (DEFAULT_SYSTEM_CLOCK/((4) * ((float)u8BaudRate)));
    if(u32NuevoSBR > 255)
    {
    	u8NuevoSBRH  = ((uint32_t)u32NuevoSBR);
    	u8NuevoSBRH &= 0x00000F00;
    	u8NuevoSBRH  = u8NuevoSBRH >> 8;
    }
    else
    	u8NuevoSBRH = 0;

	UART0->C2  |= UART_C2_RIE(0) | UART_C2_TE(0)    //Deshabilita las funciones Rx y Tx
			   |  UART_C2_RE(0);                    //Deshabilita sus correspondientes interrupciones

	UART0->C4  = UART0_C4_OSR((uint16_t)4);
	UART0->BDH = UART_BDH_SBR((uint16_t)u8NuevoSBRH);
	UART0->BDL = UART_BDL_SBR((uint16_t)u32NuevoSBR);

	UART0->C2 |= UART_C2_RIE(1)
			  |  UART_C2_TE(1)
			  |  UART_C2_RE(1);
}

/********************************************************************************************************************************
*Nombre:        UART2_IRQHandler
*
*Definition:    Rutina de interrupcion para la recepcion de datos por medio de RX.
*
*Entradas:      Ninguna.
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void UART0_RX_TX_IRQHandler(void)
{
	if(UART0->S1 & UART_S1_RDRF_MASK);             //Consulta la badera RDRF(Paso 1 pra borrar bandera RDRF)
	UART_Push_Buffer_Rx(UART0->D);
}



