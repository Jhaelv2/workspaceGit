#include <AT/InterfazAT.h>
#include <I2C/I2C.h>
#include <PWM/PWM.h>
#include <stdio.h>
#include <stdbool.h>
//#include <PIT.h>
//#include <PWM.h>
#include "MKL25Z4.h"



/******************************************************************************
* Functions
******************************************************************************/

void delay(uint16_t time);
/******************************************************************************
* Global variables
******************************************************************************/

float Xout_g, Yout_g, Zout_g;
char DataReady;
//int a,b,c,d = 0;
_Bool increaseDuty = true;



/******************************************************************************
* Main
******************************************************************************/

int main (void)
{


	UARTx_Configuracion();
	AT_Size_of_command();

	//PWM BLUE LED
//	   PWM_TPM_Config(PWM0, CENTER_PWM, PS_4);
//	   PWM_CHANNEL_Init(PWM0, CH1, PWM_L, gpiD, 1);
	//PWM GREEN LED
//	   PWM_TPM_Config(PWM2, EDGE_PWM, PS_4);
//	   PWM_CHANNEL_Init(PWM2, CH1, PWM_L, gpiB, 19);
	//PWM RED LED
//     	PWM_TPM_Config(PWM2, EDGE_PWM, PS_4);
//	    PWM_CHANNEL_Init(PWM2, CH0, PWM_L, gpiB, 18);
	//TPM0->SC |= TPM_SC_TOIE_MASK;
	//TPM0->SC |= TPM_SC_TOF_MASK;
	//TPM0->CONTROLS[CH1].CnSC |= TPM_CnSC_CHIE_MASK;
	//NVIC_EnableIRQ(TPM0_IRQn);
	//UART_Buffer_Tx((int8_t*)"125");

  	while(1)
    {

  		AT_Gestion();
  		UART_Gestion_Tx();
  		//maquina_colores();

    }
}

