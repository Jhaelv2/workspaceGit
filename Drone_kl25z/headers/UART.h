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
#include <stdio.h>

#define RxBufferSize 16
#define TxBufferSize 100

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
	uint8_t Byte;
	struct
	{
		uint8_t Configurando   :1,//B0
		        Ocupado        :1,//B1
			    DatoDisponible :1,//B2
		        BitsRestantes  :5;//B3,B4,B5,B6,B7

	}Bits;
}uEstadoDeTransmicion;

typedef struct
{
	uint8_t  u8BufferRx[RxBufferSize];
	uint16_t u16IndiceUltimoDatoEntrante;
	uint16_t u16IndiceUltimoDatoUSaliente;
	uEstadoDeTransmicion Status;
}sRecibe;

typedef struct
{
	uint8_t  * pu8BufferTx;
	uint8_t  u8BufferTx[TxBufferSize];
	uint16_t u16PosicionDatoPortransmitir;
	uint16_t u16PosicionDatoTransmitido;
	uint8_t  BufferTxLleno :1 ;
	uint8_t  DatoCargado   :1 ;
	uint8_t  u8TamanoDeBuffer;

}sTransmite;

typedef struct
{
	sRecibe     Rx;
	sTransmite  Tx;
}sUart;



typedef enum
{
  SMTX_Init = 0,
  SMTX_ConsultaDatoDisponible,
  SMTX_TtansmiteDatoActual,
  SMTX_ConsultaTransmicionCompleta,
}eBufferTx;

extern sUart gsUart;
//----------------------------------------------------------------------------
//
// Variables Externas
//
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
// Prototipos de funcionesd
//
//----------------------------------------------------------------------------
void    UARTx_Configuracion      (void);
void    UART_Push_Buffer_Rx      (uint8_t  u8DatoCapturado);
void    UART_Buffer_Tx           (int8_t * pu8Buffer);
proceso UART_Pull_Buffer_Rx      (int8_t * pu8Dato);
void    UART_Buffer_Tx_Servicio  (int8_t  * pu8DatoParaBuffer);
void    UART_CharTo_Buffer_Tx    (int8_t  u8Char);
void    UART_Float_To_Buffer_Tx  (float  u8DatoFloat);
void    UART_Short_To_Buffer_Tx  (int16_t u8DatoShort);
void    UART_Int_To_Buffer_Tx    (int32_t u32DatoInt);
void    UART_Gestion_Tx          (void);
void    UART0_BaudRate_Consulta  (void);
void    UART0_Nuevo_BaudRate     (uint32_t u8BaudRate);

#endif /* UART_H_ */
