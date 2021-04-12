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

#define Microsegundos 1000000
#define Milisegundos  1000

typedef enum{
	eTPM0  = 0,
	eTPM1,
	eTPM2,
}eTpmModulos;

typedef enum{
	eCH0 = 0,
	eCH1,
	eCH2,
	eCH3,
	eCH4,
	eCH5,
}eTpmChanels;


#define gpiA				0u
#define gpiB				1u
#define	gpiC				2u
#define	gpiD				3u
#define gpiE				4u


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

void     PwmTpmConfig       (uint32_t PeriodoLPTMW,uint16_t TiempoExtendido, uint8_t ModuloTPM, uint8_t canalTPM, uint16_t DutyCicle);
void     PwmChannelInit     (uint8_t TpmModuloN, uint16_t CuentasPorTiempo, uint8_t Channel,uint16_t DutyCicleTiempopin);
void     PWM_Dutty          (uint8_t tpm, uint8_t Channel, uint16_t Dutty);
void     maquina_colores    (void);
void     delay              (uint16_t time);
uint16_t AjusteTiempoCuenta (uint32_t DivisorDeTiempo, uint8_t *Prescaler);

#endif /* PWM_H_ */
