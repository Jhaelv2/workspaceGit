/*
 * UART.h
 *
 *  Created on: 12 dic. 2020
 *      Author: Hugo
 */

#ifndef UART_H_
#define UART_H_

#include "MKL25Z4.h"
#include "SERVICIOS.h"

#define TAMANO_BUFFER_RX 16
#define TAMANO_BUFFER_TX 1000

//----------------------------------------------------------------------------
//
// DEclaracion de estructuras
//
//----------------------------------------------------------------------------


typedef enum
{
	BBT_Inroduce_dato=0,
	BBT_Limpiar_Bandera,
}BorrarBanderaTDRF;

typedef union
{
	uint8_t All;
	struct
	{							   //  XXXXXXXX Condiciones del Evento
		uint8_t Inicializando :1,//  ||||||||_ B0  =  [1:Si  0:No]
		      EnServicio      :1,//  |||||||__ B1  =  [1:Si  0:No]
			  DatoDisponible  :1,//  ||||||___ B2  =  [1:Si  0:No] Si la edicion fue manual o externa
		      Bit3			  :1,//  |||||____ B3  =  [1:Si  0:No]
		      Bit4            :1,//  ||||_____ B4  =  [1:Si  0:No]
		      Bit5            :1,//  |||______ B5  =  [1:Si  0:No]
		      Bit6            :1,//  ||_______ B6  =  [1:Si  0:No]
		      Bit7            :1;//  |________ B7  =  [1:Si  0:No]
	}Bit;
}uCondicionEvento;

typedef struct
{
	uint8_t  u8Buffer[TAMANO_BUFFER_RX];
	uint16_t u16PosicionDatoRecibido;
	uint16_t u16PosicionDatoExtraido;
	uCondicionEvento flag;
}sRx;

typedef struct
{
	uint8_t  * pu8BufferTx;
	uint16_t u16PosicionDatoPortransmitir;
	uint16_t u16PosicionDatoTransmitido;
	uint8_t  BufferTxLleno :1 ;
	uint8_t  DatoCargado   :1 ;
	uint8_t  u8TamanoDeBuffer;

}sTx;

typedef struct
{
	sRx  Rx;
	sTx  Tx;
}sUart;

typedef enum
{
  eBT_Init = 0,
  eBT_ConsultaDatoDisponible,
  eBT_TtansmiteDatoActual,
  eBT_ConsultaTransmicionCompleta,
}eBufferTx;



//----------------------------------------------------------------------------
//
// Variables Externas
//
//----------------------------------------------------------------------------
//extern sUart gsUart;
//----------------------------------------------------------------------------
//
// Prototipos de funcionesd
//
//----------------------------------------------------------------------------
void    UARTx_Configuracion     (uint8_t * puTxBuffer, uint8_t u8TxBufferSize);
void    UART_PushBufferRx       (uint8_t  u8DatoCapturado);
void    UART_BufferTx           (uint8_t  * pu8Buffer);
proceso PullBufferRx            (uint8_t  * pu8Dato);
void    UART_BufferTxServicio   (uint8_t  * pu8DatoParaBuffer);
void    UART_CharToBufferTx     (uint8_t  u8Char);
void    UART_FloatToBufferTx    (float  u8DatoFloat);
void    UART_ShortToBufferTx    (uint16_t u8DatoShort);
void    UART_IntToBufferTx      (uint32_t u32DatoInt);
void    UART_AtencionUartTx     (void);
void    UART0_BaudRateConsulta  (void);
void    UART0_NuevoBaudRate     (uint32_t u8BaudRate);

#endif /* UART_H_ */
