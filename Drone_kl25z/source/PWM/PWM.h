/*
 * PWM.h
 *
 *  Created on: 13 dic. 2020
 *      Author: Madera
 */

#ifndef PWM_H_
#define PWM_H_

#include "MKL25Z4.h"

typedef enum{
	e_rojo=0,
	e_azul,
	e_verde,
	e_amarillo,
	e_cyan,
	e_magenta,
	e_blanco
}eColors;

#define PWM0				0U
#define PWM1				1U
#define PWM2				2U

#define CH0					0U
#define CH1					1U
#define CH2					2U
#define CH3					3U
#define CH4					4U
#define CH5					5U

#define gpiA				0u
#define gpiB				1u
#define	gpiC				2u
#define	gpiD				3u
#define gpiE				4u

#define	PS_1				0U
#define	PS_2				1U
#define	PS_4				2U
#define	PS_8				3U
#define	PS_16				4U
#define	PS_32				5U
#define PS_64				6U
#define PS_128				7U

#define EDGE_PWM			0U
#define CENTER_PWM			1U

#define LPTPM_DISABLE		0U
#define LPTPM_CLK			1U		//LPTPM COUNTER INCREMENTS ON EVERY LPTPM COUNTER CLOCK
#define LPTPM_EXTCLK		2U		//LPTPM COUNTER INCREMENTS ON RISING EDGE OF LPTPM_EXTCLK SYNCHRONIZED TO THE LPTPM COUNTER CLOCK

#define TPM_CLK_DISABLED	0U		//DISABLES TPM CLK
#define TPM_MCGFLLCLK		1U		//ENABLE TPM CLK MCGFLLCLK OR MCGPLLCLK/2
#define TPM_OSERCLK			2U		//ENABLE TPM OSCERCLK
#define TPM_MCGIRCLK		3U		//ENABLE TPM MCGIRCLK
//ALIGNED PWM MODES
#define PWM_H				TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK
#define PWM_L				TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK
//OUTPUT COMPARE DEFINES
#define PWM_OC_TGG			TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK							//OUTPUT COMPARE TOGGLE OUTPUT ON MATCH
#define PWM_OC_CLS			TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK							//CLEAR OUTPUT ON MATCH
#define PWM_OC_STP			TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK 	// SET OUTPUT ON MATCH
#define PWM_OC_POL			TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK 		// PULSE OUTPUT LOW
#define PWM_OC_POH			TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK | TPM_CnSC_ELSA_MASK		// PULSE OUTPUT HIGH
//CHANNEL DISABLE
#define PWM_DISABLE			TPM_CnSC_MSB(0) | TPM_CnSC_MSA(0) | TPM_CnSC_ELSB(0) | TPM_CnSC_ELSA(0)

#define MCGFLLCLK_TIME		41900000u

#define PWM_Clocks()		SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;\
							SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;\
							SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);\
							PORTE->PCR[29] |= PORT_PCR_MUX(3);\
							PORTE->PCR[30] |= PORT_PCR_MUX(3);


#define TPM0_CH2_Init()		TPM0->SC = 0;\
							TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);\
							TPM0->MOD = 520;\
							TPM0->CONTROLS[2].CnV = 25;\
							TPM0->SC |= TPM_SC_CMOD(1) |TPM_SC_PS(2) | TPM_SC_TOIE_MASK;

#define	TPM0_CH3_Init()		TPM0->SC = 0;\
							TPM0->CONTROLS[3].CnSC |= (TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);\
							TPM0->MOD = 4194;\
							TPM0->CONTROLS[3].CnV = 4568;\
							TPM0->SC |= TPM_SC_CMOD(1)|TPM_SC_PS(0);

void PWM_TPM_Config(uint8_t tpm, _Bool align, uint8_t prescaler);
void PWM_CHANNEL_Init(uint8_t tpm, uint8_t channel, uint8_t Mode, uint8_t  gpio, int pin);
void PWM_Freq(uint8_t tpm,uint16_t Freq);
void PWM_Dutty(uint8_t tpm, uint8_t Channel, uint16_t Dutty);
void maquina_colores(void);
void delay(uint16_t time);
void PWM_Disable(uint8_t tpm, uint8_t channel);

#endif /* PWM_H_ */
