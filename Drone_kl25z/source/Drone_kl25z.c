#include <stdio.h>
#include <stdbool.h>
#include <PIT.h>
#include <PWM.h>
#include "MKL25Z4.h"
#include "I2C.h"
#include "MMA8451.h"
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
	//PWM BLUE LED
	PWM_TPM_Config(PWM0, CENTER_PWM, PS_4);
	PWM_CHANNEL_Init(PWM0, CH1, PWM_L, gpiD, 1);
	//PWM GREEN LED
	PWM_TPM_Config(PWM2, EDGE_PWM, PS_4);
	PWM_CHANNEL_Init(PWM2, CH1, PWM_L, gpiB, 19);
	//PWM RED LED
	PWM_TPM_Config(PWM2, EDGE_PWM, PS_4);
	PWM_CHANNEL_Init(PWM2, CH0, PWM_L, gpiB, 18);
  	while(1)
    {

  		maquina_colores();

	}
}




/******************************************************************************
* PORT A Interrupt handler
******************************************************************************/

void PORTA_IRQHandler()
{
	PORTA->PCR[14] |= PORT_PCR_ISF_MASK;			// Clear the interrupt flag
	DataReady = 1;
}


