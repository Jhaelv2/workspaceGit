#include <stdio.h>
#include <stdbool.h>
#include "MKL25Z4.h"
#include "I2C.h"
#include "MMA8451.h"

/******************************************************************************
* Functions
******************************************************************************/

void MCU_Init(void);
void PWM_clocks(void);
void PWM_init(void);
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
//	DataReady = 0;
//	MCU_Init();
//  	Accelerometer_Init();
//  	Calibrate();
//  	Init_pins();
	PWM_clocks();
	PWM_init();

  	while(1)
    {
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
//		if (DataReady)		// Is a new set of data ready?
//		{
//			DataReady = 0;
//
//			I2C_ReadMultiRegisters(MMA845x_I2C_ADDRESS, OUT_X_MSB_REG, 6, AccData);		// Read data output registers 0x01-0x06
//
//			Xout_14_bit = ((short) (AccData[0]<<8 | AccData[1])) >> 2;		// Compute 14-bit X-axis output value
//			Yout_14_bit = ((short) (AccData[2]<<8 | AccData[3])) >> 2;		// Compute 14-bit Y-axis output value
//			Zout_14_bit = ((short) (AccData[4]<<8 | AccData[5])) >> 2;		// Compute 14-bit Z-axis output value
//
//			Xout_g = (( float) Xout_14_bit) / SENSITIVITY_2G;		// Compute X-axis output value in g's
//			Yout_g = (( float) Yout_14_bit) / SENSITIVITY_2G;		// Compute Y-axis output value in g's
//			Zout_g = (( float) Zout_14_bit) / SENSITIVITY_2G;		// Compute Z-axis output value in g's
//
//			if(Xout_g>1 || Xout_g<-1)
//			{
//				BLUE_ON;
//				RED_OFF;
//				GREEN_OFF;
//			}
//
//			if(Xout_g>2 || Xout_g<-2)
//			{
//				BLUE_ON;
//				RED_OFF;
//				GREEN_ON;
//			}
//
//			if(Yout_g>1 || Yout_g<-1)
//			{
//				BLUE_OFF;
//				RED_ON;
//				GREEN_OFF;
//			}
//
//			if(Yout_g>2 || Yout_g<-2)
//			{
//				BLUE_ON;
//				RED_ON;
//				GREEN_OFF;
//			}
//
//			if(Zout_g>1 || Zout_g<-1)
//			{
//				BLUE_OFF;
//				RED_OFF;
//				GREEN_ON;
//			}
//
//			if(Zout_g>2 || Zout_g<-2)
//			{
//				BLUE_OFF;
//				RED_ON;
//				GREEN_ON;
//			}
//		}
	}
}

/******************************************************************************
* PWM initialization functions
******************************************************************************/
void PWM_clocks(void){
	SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK |	//Turn On Clock for Port D
			SIM_SCGC5_PORTB_MASK;			//Turn On Clock for Port B
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK |		//Turn On Clock for TPM0
			SIM_SCGC6_TPM2_MASK;			//Turn On Clock for TPM2
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);		//TPM Clock Source Select only MCGIRCLK
	PORTD->PCR[1] |= PORT_PCR_MUX(4);		//Set Pin Mux Alt 4 for LQFP chip TPM0_CH0
	PORTB->PCR[18] |= PORT_PCR_MUX(3);		//Set Pin Mux Alt 3 for LQFP chip TPM2_CH0
	PORTB->PCR[19] |= PORT_PCR_MUX(3);		//Set Pin Mux Alt 3 for LQFP chip TPM2_CH1
}
void PWM_init(void){
	//Active TPM0_CH1
	TPM0->SC = 0; 							//Reset all TPM0 SC Registers
	TPM0->CONTROLS[1].CnSC |= (				//Select Status and Control as EDGE-ALIGNED PWM
			TPM_CnSC_ELSB_MASK|				//ELSnB = 1 ELSnA = 0
			TPM_CnSC_MSB_MASK); 			//MSnB = 1 MSnA = 0 PWM
	TPM0->MOD = 43702;						//Set PWM counter
	TPM0->CONTROLS[1].CnV = 14568;			//i don't understand jiji
	TPM0->SC |= TPM_SC_CMOD(1) | 			//Set CMOD for Increment LPTPM every LPTPM counter clock
			TPM_SC_PS(11);					//Set Prescale as divided by 16
	//Active TPM2_CH0
	TPM2->SC = 0;
	TPM2->CONTROLS[0].CnSC |= (
			TPM_CnSC_ELSB_MASK|
			TPM_CnSC_MSB_MASK);
	TPM2->MOD = 10;
	TPM2->CONTROLS[0].CnV = 4568;
	//Active TPM2_CH1
	TPM2->CONTROLS[1].CnSC |= (
			TPM_CnSC_ELSB_MASK|
			TPM_CnSC_MSB_MASK);
	TPM2->MOD = 55555;
	TPM2->CONTROLS[1].CnV = 24568;
	TPM2->SC |= TPM_SC_CMOD(1) |
			TPM_SC_PS(11);


}

/******************************************************************************
* MCU initialization function
******************************************************************************/

void MCU_Init(void)
{
	//I2C0 module initialization
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;		// Turn on clock to I2C0 module
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;		// Turn on clock to Port E module
	PORTE->PCR[24] = PORT_PCR_MUX(5);			// PTE24 pin is I2C0 SCL line
	PORTE->PCR[25] = PORT_PCR_MUX(5);			// PTE25 pin is I2C0 SDA line
	I2C0->F  = 0x14; 						// SDA hold time = 2.125us, SCL start hold time = 4.25us, SCL stop hold time = 5.125us *
	I2C0->C1 = I2C_C1_IICEN_MASK;    		// Enable I2C0 module

	//Configure the PTA14 pin (connected to the INT1 of the MMA8451Q) for falling edge interrupts
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Turn on clock to Port A module
	PORTA->PCR[14] |= (0|PORT_PCR_ISF_MASK|	// Clear the interrupt flag
					  PORT_PCR_MUX(0x1)|	// PTA14 is configured as GPIO
					  PORT_PCR_IRQC(0xA));	// PTA14 is configured for falling edge interrupts

	//Enable PORTA interrupt on NVIC
	NVIC_EnableIRQ(PORTA_IRQn);
}


/******************************************************************************
* PORT A Interrupt handler
******************************************************************************/

void PORTA_IRQHandler()
{
	PORTA->PCR[14] |= PORT_PCR_ISF_MASK;			// Clear the interrupt flag
	DataReady = 1;
}

