/*
 * PWM.h
 *
 *  Created on: 13 dic. 2020
 *      Author: Madera
 */

#ifndef PWM_H_
#define PWM_H_

#include "MKL25Z4.h"

#define PWM_Clocks()		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTB_MASK;\
							SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK|SIM_SCGC6_TPM2_MASK;\
							SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);\
							PORTD->PCR[1] |= PORT_PCR_MUX(4);\
							PORTB->PCR[18] |= PORT_PCR_MUX(3);\
							PORTB->PCR[19] |= PORT_PCR_MUX(3);


#define TPM0_CH1_Init()		TPM0->SC = 0;\
							TPM0->CONTROLS[1].CnSC |= (TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);\
							TPM0->MOD = 43702;\
							TPM0->CONTROLS[1].CnV = 14568;\
							TPM0->SC |= TPM_SC_CMOD(1) |TPM_SC_PS(11);

#define	TPM2_CH0_Int()		TPM2->SC = 0;\
							TPM2->CONTROLS[0].CnSC |= (TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);\
							TPM2->MOD = 10;\
							TPM2->CONTROLS[0].CnV = 4568;\
							TPM2->SC |= TPM_SC_CMOD(1)|TPM_SC_PS(11);

#define	TPM2_CH1_Int()		TPM2->SC = 0;\
							TPM2->CONTROLS[1].CnSC |= (TPM_CnSC_ELSB_MASK|TPM_CnSC_MSB_MASK);\
							TPM2->MOD = 55555;\
							TPM2->CONTROLS[1].CnV = 24568;\
							TPM2->SC |= TPM_SC_CMOD(1)|TPM_SC_PS(11);


#endif /* PWM_H_ */
