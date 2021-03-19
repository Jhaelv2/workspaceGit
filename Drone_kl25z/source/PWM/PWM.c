/*
 * PWM.c
 *
 *  Created on: 13 dic. 2020
 *      Author: Madera
 */

#include <PWM/PWM.h>

PORT_Type port;
int a,b,c,d = 0;
/********************************************************************************************************************************
*Nombre:        PWM_TPM_CONFIG
*
*Definition:    Configura Registros SIM (SCGC6, SOPT2) y TPM_SC para habilitar el TPM del canal requerido
*				Configura el MOD en 520 para tener aprox 40KHZ sin prescaler
*				se configura el preescaler en 4 para tener 10Khz en frecuencia
*
*Entradas:      tpm, align, prescaler
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PWM_TPM_Config(uint8_t tpm, _Bool align, uint8_t prescaler){

	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(TPM_MCGFLLCLK);

	if(tpm==PWM0){
		SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
		TPM0->SC |= TPM_SC_CPWMS(align)
				 |  TPM_SC_PS(prescaler)
				 |  TPM_SC_CMOD(LPTPM_CLK);
		//TPM0->MOD = 520;
	}
	else if (tpm==PWM1){
		SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;
		TPM1->SC |= TPM_SC_CPWMS(align)
				 |  TPM_SC_PS(prescaler)
				 |  TPM_SC_CMOD(LPTPM_CLK);
		//TPM1->MOD = 520;
	}
	else if (tpm==PWM2){
		SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
		TPM2->SC |= TPM_SC_CPWMS(align)
				 |  TPM_SC_PS(prescaler)
				 |  TPM_SC_CMOD(LPTPM_CLK);
		//TPM2->MOD = 520;
	}
	else return;

	PWM_Freq(tpm,10000);

}
/********************************************************************************************************************************
*Nombre:        PWM_CHANEL_INIT
*
*Definition:    Configura el canal de PWM
*
*Entradas:      TPM, Freq
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PWM_CHANNEL_Init(uint8_t tpm, uint8_t channel, uint8_t Mode, uint8_t  gpio, int pin){

	if(tpm==PWM0){
		/*	CH0 PTA3 ALT3
		 * 	CH1 PTA4 ALT3
		 * 	CH2 PTA5 ALT3
		 * 	CH5 PTA0 ALT3		 */
		if(gpio==gpiA){
			SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
			if((channel<=2)||(channel==5)){
				if((pin>=3)||(pin<=5))
					PORTA->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		/*	CH0 PTC1 ALT4
		 * 	CH1 PTC2 ALT4
		 * 	CH2 PTC3 ALT4
		 * 	CH3 PTC4 ALT4
		 * 	CH4 PTC8 ALT3
		 * 	CH5 PTC9 ALT3		 */
		else if(gpio==gpiC){
			SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
			if(channel<=3 && channel>=0){
				if(pin<=4 || pin>=1)
					PORTC->PCR[pin] = PORT_PCR_MUX(4);
				else return;
			}
			else if(channel<=5 && channel>=4){
				if(pin==8 || pin==9)
					PORTC->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		/*	CH0 PTD0 ALT4
		 * 	CH1 PTD1 ALT4
		 * 	CH2 PTD2 ALT4
		 * 	CH3 PTD3 ALT4
		 * 	CH4 PTD4 ALT4
		 * 	CH5 PTD5 ALT4		 */
		else if(gpio==gpiD){
			SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
			if(channel<=5 && channel>=0){
				if(pin<=5 && pin>=0)
					PORTD->PCR[pin] = PORT_PCR_MUX(4);
				else return;
			}
			else return;
		}
		/*	CH0 PTE24 ALT3
		 * 	CH1 PTE25 ALT3
		 * 	CH2 PTE29 ALT3
		 * 	CH3 PTE30 ALT3
		 * 	CH4 PTE31 ALT3		 */
		else if(gpio==gpiE){
			SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
			if(channel<=4 && channel>=0){
				if(pin==31||pin==29||pin==30||pin==25||pin==24)
					PORTE->PCR[pin] = PORT_PCR_MUX(4);
				else return;
			}
			else return;

		}
		TPM0->CONTROLS[channel].CnSC |= Mode ;
	}
	else if(tpm==PWM1){
		/*	CH0	PTA12 ALT3
		 * 	CH1 PTA13 ALT3		 */
		if(gpio==gpiA){
			SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
			if(channel==CH0 || channel==CH1){
				if(pin==12 || pin==13)
					PORTA->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		/*	CH0	PTB0 ALT3
		 *	CH1	PTB1 ALT3		 */
		else if(gpio==gpiB){
			SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
			if(channel==CH0 || channel==CH1){
				if(pin==0 || pin==1)
					PORTB->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		/*	CH0	PTE20 ALT3
		 *	CH1	PTE21 ALT3		 */
		else if(gpio==gpiE){
			SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
			if(channel==CH0 || channel==CH1){
				if(pin==20 || pin==21)
					PORTE->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		else
			return;

		TPM1->CONTROLS[channel].CnSC |= Mode ;
	}
	else if(tpm==PWM2){
		/*	CH0	PTA1 ALT3
		 *	CH1	PTA2 ALT3		 */
		if(gpio==gpiA){
			SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
			if(channel==CH0 || channel==CH1){
				if(pin==1 || pin==2)
					PORTA->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		/*	CH0	PTB2 ALT3
		 *	CH1	PTB3 ALT3
		 *	CH0 PTB18 ALT3
		 *	CH1 PTB19 ALT3		 */
		else if(gpio==gpiB){
			SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
			if(channel==CH0 || channel==CH1){
				if(pin==2 || pin==3)
					PORTB->PCR[pin] = PORT_PCR_MUX(3);
				else if(pin==18 || pin==19)
					PORTB->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;
		}
		/*	CH0	PTE22 ALT3
		 *	CH1	PTE23 ALT3		 */
		else if(gpio==gpiE){
			SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
			if(channel==CH0 || channel==CH1){
				if(pin==22 || pin==23)
					PORTE->PCR[pin] = PORT_PCR_MUX(3);
				else return;
			}
			else return;

		}
		else
			return;

		TPM2->CONTROLS[channel].CnSC |= Mode ;

	}

	else
		return;

	PWM_Dutty(tpm,channel,25);

}
/********************************************************************************************************************************
*Nombre:        PWM_Freq
*
*Definition:    Determina la Frecuencia del canal de PWM
*
*Entradas:      TPM, Freq
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/

void PWM_Freq(uint8_t tpm,uint16_t Freq){

	uint16_t modulo;
	modulo = MCGFLLCLK_TIME/(Freq)*2*PS_4 -1;

	if(tpm==PWM0){
		TPM0->MOD = modulo;
	}
	else if (tpm==PWM1){
		TPM1->MOD = modulo;
	}
	else if (tpm==PWM2){
		TPM2->MOD = modulo;
	}
	else{
		return;
	}

}
/********************************************************************************************************************************
*Nombre:        PWM_Dutty
*
*Definition:    Determina el ancho de pulso del canal PWM
*
*Entradas:      tpm, Channel, Dutty
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PWM_Dutty(uint8_t tpm, uint8_t channel, uint16_t Dutty ){

	uint16_t chV;

	if(tpm==PWM0){
		chV= (Dutty*TPM0->MOD)/100;
		TPM0->CONTROLS[channel].CnV = chV;
	}
	else if(tpm==PWM1){
		chV= (Dutty*TPM1->MOD)/100;
		TPM1->CONTROLS[channel].CnV = chV;
	}
	else if(tpm==PWM2){
		chV= (Dutty*TPM2->MOD)/100;
		TPM2->CONTROLS[channel].CnV = chV;
	}
	else{
		return;
	}

}
/********************************************************************************************************************************
*Nombre:        PWM_Enable
*
*Definition:    Rutina para desabilitar un canal de PWM
*
*Entradas:      Ninguna.
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PWM_Enable(uint8_t tpm, uint8_t channel, uint8_t Mode){

	if(tpm==PWM0){
		TPM0->CONTROLS[channel].CnSC |= Mode ;
	}
	else if(tpm==PWM1){
		TPM1->CONTROLS[channel].CnSC |= Mode ;
	}
	else if(tpm==PWM2){
		TPM2->CONTROLS[channel].CnSC |= Mode ;
	}
	else{
		return;
	}
}
/********************************************************************************************************************************
*Nombre:        PWM_DISABLE
*
*Definition:    Rutina para desabilitar un canal de PWM
*
*Entradas:      Ninguna.
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PWM_Disable(uint8_t tpm, uint8_t channel){

	if(tpm==PWM0){
		TPM0->CONTROLS[channel].CnSC = PWM_DISABLE;
	}
	else if(tpm==PWM1){
		TPM1->CONTROLS[channel].CnSC = PWM_DISABLE;
	}
	else if(tpm==PWM2){
		TPM2->CONTROLS[channel].CnSC = PWM_DISABLE;
	}
	else{
		return;
	}
}
/********************************************************************************************************************************
*Nombre:        maquina_colores
*
*Definition:    Maquina de Estados para cambiar de PWM
*
*Entradas:      Ninguna.
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/

void maquina_colores(void){

	static uint8_t eColors = e_rojo;

	switch(eColors){

	case e_rojo:
  		//RED LED
		PWM_Enable(PWM2, CH0, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM2, CH0, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM2, CH0, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM2, CH0);
  		eColors = e_azul;
  		break;

	case e_azul:
  		//BLUE LED
		PWM_Enable(PWM0, CH1, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM0, CH1, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM0, CH1, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM0, CH1);
  		eColors = e_verde;
  		break;

	case e_verde:
  		//GREEN LED
		PWM_Enable(PWM2, CH1, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM2, CH1, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM2, CH1, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM2, CH1);
  		eColors = e_amarillo;
  		break;

	case e_amarillo:
  		//YELLOW LED
		PWM_Enable(PWM2, CH0, PWM_L);
		PWM_Enable(PWM2, CH1, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM2, CH1, a);
  			PWM_Dutty(PWM2, CH0, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM2, CH1, a);
  		  			PWM_Dutty(PWM2, CH0, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM2, CH1);
  		PWM_Disable(PWM2, CH0);
  		eColors = e_cyan;
  		break;

	case e_cyan:
  		//CYAN LED
		PWM_Enable(PWM0, CH1, PWM_L);
		PWM_Enable(PWM2, CH1, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM0, CH1, a);
  			PWM_Dutty(PWM2, CH1, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM0, CH1, a);
  		  			PWM_Dutty(PWM2, CH1, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM0, CH1);
  		PWM_Disable(PWM2, CH1);
  		eColors = e_magenta;
  		break;

	case e_magenta:
  		//MAGENTA LED
		PWM_Enable(PWM0, CH1, PWM_L);
		PWM_Enable(PWM2, CH0, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM0, CH1, a);
  			PWM_Dutty(PWM2, CH0, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM0, CH1, a);
  		  			PWM_Dutty(PWM2, CH0, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM0, CH1);
  		PWM_Disable(PWM2, CH0);
  		eColors = e_blanco;
  		break;

	case e_blanco:
  		//white LED
		PWM_Enable(PWM0, CH1, PWM_L);
		PWM_Enable(PWM2, CH0, PWM_L);
		PWM_Enable(PWM2, CH1, PWM_L);
  		for(a=0; a<=100; a++){
  			PWM_Dutty(PWM0, CH1, a);
  			PWM_Dutty(PWM2, CH0, a);
  			PWM_Dutty(PWM2, CH1, a);
  			delay(5);
  			if(a==100){
  				for(a=100; a>=0; a--){
  		  			PWM_Dutty(PWM0, CH1, a);
  		  			PWM_Dutty(PWM2, CH0, a);
  		  			PWM_Dutty(PWM2, CH1, a);
  		  			delay(5);
  				}
  				a=101;
  			}
  		}
  		PWM_Disable(PWM0, CH1);
  		PWM_Disable(PWM2, CH0);
  		PWM_Disable(PWM2, CH1);
  		eColors = e_rojo;
  		break;

	}

}

void delay(uint16_t time){
	for(b=0; b<=700 ; b++){
		for(c=0; c<=time; c++){
		}
	}
}

/********************************************************************************************************************************
*Nombre:        PWM_IRQHandler
*
*Definition:    Rutina de interrupcion para el PWM
*
*Entradas:      Ninguna.
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void TPM0_IRQHandler(void){
	if(TPM0->STATUS & TPM_STATUS_TOF_MASK){
		TPM0->STATUS |= TPM_STATUS_TOF_MASK;
	}
}
