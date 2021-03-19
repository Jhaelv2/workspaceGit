/*
Este es un Codigo fuente para la biblioteca de Drivers
del Dron-Carga beta 0
 

@file DMA_.c
@date 11 mar. 2021
@author Hugo
@brief DMA_

@version 2021
*/
/*   System Headers   	*/
/*   Used Headers   	*/
/*   Own Headers   		*/

#include "DMA__cfg.h"
#include "DMA__def.h"
#include "DMA_.h"

/*     [EXPORTED]		*/

/*public variables*/

/*     [INTERNAL]		*/

/*internal__pre-processor macros*/

/*internal__types  */

/*private__variables */

/*private__functions */
void DmaConfiguracion (void)
{
	SIM->SCGC7 = SIM_SCGC7_DMA(1);
	SIM->SCGC6 = SIM_SCGC6_DMAMUX(1);

	//pREPARAR  canal 0 para llenar el buffer de rc.
	DMA0->DMA[0].DCR |= DMA_DCR_LINKCC(0)
			         | DMA_DCR_DMOD(0X1)
					 | DMA_DCR_SMOD(0)
			         | DMA_DCR_DSIZE(1)
					 | DMA_DCR_DINC(1)
					 | DMA_DCR_SSIZE(1)
					 | DMA_DCR_SINC(0)
					 | DMA_DCR_AA(0)
					 | DMA_DCR_CS(1)
					 | DMA_DCR_ERQ(1)
					 | DMA_DCR_CS(1);//NO se si nececesito esta funcionalidad pero creo que si va a ser necesario

	DMA0->DMA[0].SAR = (uint32_t)&UART0->D;                 //La direccion de fuente es la del Registro "D" de uart0
	DMA0->DMA[0].DAR = (uint32_t)&gsUart.Rx.u8BufferRx[0];  // la direccion de destrino es la del elemento 0 del buffer Rx

	DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_BCR(RxBufferSize);

}

/*public_functions */
