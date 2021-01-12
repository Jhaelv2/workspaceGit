#include <stdio.h>
#include <stdbool.h>
#include "MKL25Z4.h"
#include "I2C.h"
#include "MMA8451.h"
#include "PWM.h"
#include "InterfazAT.h"

/******************************************************************************
* Functions
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

float Xout_g, Yout_g, Zout_g;
char DataReady;
int a,b,c = 0;
_Bool increaseDuty = true;

/******************************************************************************
* Main
******************************************************************************/

int main (void)
{
	DataReady = 0;
	I2C_Init();
  	Accelerometer_Init();
  	Calibrate();
  	Init_pins();
	PWM_Clocks();
	UARTx_Configuracion();
	TPM0_CH1_Init();
	TPM2_CH0_Int();
	TPM2_CH1_Int();
	AT_Size_of_command();

	UART_Push_Buffer_Rx('A');
	UART_Push_Buffer_Rx('T');
	UART_Push_Buffer_Rx('+');
	UART_Push_Buffer_Rx('P');
	UART_Push_Buffer_Rx('W');
	UART_Push_Buffer_Rx('M');
//	UART_Push_Buffer_Rx('B');
//	UART_Push_Buffer_Rx('A');
//	UART_Push_Buffer_Rx('U');
//	UART_Push_Buffer_Rx('D');
//	UART_Push_Buffer_Rx('R');
	UART_Push_Buffer_Rx(0x0D);

  	while(1)
    {
  		AT_Gestion();

  		if(increaseDuty)
  				{
  					a += 1237;
  					b += 2500;
  					c += 4000;
  					if(a > 43702)
  						increaseDuty = false;

  					TPM0->CONTROLS[1].CnV = a;
  					TPM2->CONTROLS[0].CnV = b;
  					TPM2->CONTROLS[1].CnV = c;
  				}

  				else
  				{
  					a -= 1237;
  					b -= 2500;
  					c -= 4000;
  					if(a < 1)
  					   increaseDuty = true;

  					TPM0->CONTROLS[1].CnV = a;
  					TPM2->CONTROLS[0].CnV = b;
  					TPM2->CONTROLS[1].CnV = c;
  				}
		if (DataReady)		// Is a new set of data ready?
		{
			DataReady = 0;

			I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);		// Read data output registers 0x01-0x06

			Xout_14_bit = ((short) (AccData[0]<<8 | AccData[1])) >> 2;		// Compute 14-bit X-axis output value
			Yout_14_bit = ((short) (AccData[2]<<8 | AccData[3])) >> 2;		// Compute 14-bit Y-axis output value
			Zout_14_bit = ((short) (AccData[4]<<8 | AccData[5])) >> 2;		// Compute 14-bit Z-axis output value

			Xout_g = (( float) Xout_14_bit) / SENSITIVITY_2G;		// Compute X-axis output value in g's
			Yout_g = (( float) Yout_14_bit) / SENSITIVITY_2G;		// Compute Y-axis output value in g's
			Zout_g = (( float) Zout_14_bit) / SENSITIVITY_2G;		// Compute Z-axis output value in g's

			if(Xout_g>1 || Xout_g<-1)
			{
				BLUE_ON;
				RED_OFF;
				GREEN_OFF;
			}

			if(Xout_g>2 || Xout_g<-2)
			{
				BLUE_ON;
				RED_OFF;
				GREEN_ON;
			}

			if(Yout_g>1 || Yout_g<-1)
			{
				BLUE_OFF;
				RED_ON;
				GREEN_OFF;
			}

			if(Yout_g>2 || Yout_g<-2)
			{
				BLUE_ON;
				RED_ON;
				GREEN_OFF;
			}

			if(Zout_g>1 || Zout_g<-1)
			{
				BLUE_OFF;
				RED_OFF;
				GREEN_ON;
			}

			if(Zout_g>2 || Zout_g<-2)
			{
				BLUE_OFF;
				RED_ON;
				GREEN_ON;
			}
		}
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

