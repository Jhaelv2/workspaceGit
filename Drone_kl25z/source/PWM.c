/*
 * PWM.c
 *
 *  Created on: 13 dic. 2020
 *      Author: Madera
 */

#include <PWM.h>

PORT_Type port;
int a,b,c,d = 0;
/********************************************************************************************************************************
*Nombre:        PWM_TPM_CONFIG
*
*Definition:    Configura Registros SIM (SCGC6, SOPT2) y TPM_SC para habilitar el TPM del canal requerido
*				Configura el MOD en 520 para tener aprox 40KHZ sin prescaler
*				se configura el preescaler en 4 para tener 10Khz en frecuencia
*
*Entradas:      PeriodoLPTMW se refiere a se se espera que el periodo del lptmw se divida en mili o micro segundos,
*               align admite las definiciones "Milisegundos" y "Microsegundos".
*               TiempoExtendido: Se refiere a la cantidad de unidades de tiempo que pasaran antes de que se reinicie
*               el contador.
*               Align: selecciona el tipo de alineacion pwm
*
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PwmTpmConfig (uint32_t PeriodoLPTMW,uint16_t TiempoExtendido, uint8_t ModuloTPM, uint8_t canalTPM, uint16_t DutyCicle)
{

	uint8_t Prescaler;
	uint16_t CuentasPorTiempo;
	uint16_t Tiempociclo;
	//NVIC_EnableIRQ(TPM0_IRQn );

	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	PORTD->PCR[0] |= PORT_PCR_MUX(4);

	TPM0->SC = 0;
	TPM0->CONF |= TPM_CONF_DBGMODE(1);
	CuentasPorTiempo = AjusteTiempoCuenta(PeriodoLPTMW, &Prescaler);

	TPM0->SC |=  TPM_SC_CPWMS(0x00) | TPM_SC_CMOD(0x01)  |  TPM_SC_PS(Prescaler);

	Tiempociclo = CuentasPorTiempo * TiempoExtendido;
	TPM0->MOD = Tiempociclo;
	PwmChannelInit(ModuloTPM, CuentasPorTiempo, canalTPM, DutyCicle);
}
/********************************************************************************************************************************
 * Nombre:      AjusteTiempoCuenta
 *
 * Descripcion: Ajusta la frecuencia del LPTPM para generar un periodo de tiempo por
 *              cantidad de cuentas deseado.
 *
 *
 * Parametros:  DivisorDeTiemp; se refiere a si se quiere que el tiempo de cada periodo
 *              sea contado en micro o mili segundos.
 *              Prescaler; Es un puntero que apunta a la variable que contiene el valor
 *              del prescaler, entra esta direccion para modificar el prescaler en funcion
 *              del divisor de tiempo seleccionado.
 *
 * Return:      ModVal: esta variable contiene la cantidad de pulsos de reloj necesarios para
 *              generar un mili o microsegundo (DivisorDeTiempo)
 *
 *******************************************************************************************************************************/
uint16_t AjusteTiempoCuenta(uint32_t DivisorDeTiempo, uint8_t *Prescaler)
{

	uint16_t ModVal;
	uint8_t PrescalerVal;

	switch (DivisorDeTiempo)
	{
	case Milisegundos:
		*Prescaler = 0x07;
		PrescalerVal = 128;
		break;
	case Microsegundos:
		*Prescaler = 0;
		PrescalerVal = 1;
		break;
	}

	ModVal = (((DEFAULT_SYSTEM_CLOCK)/PrescalerVal)/DivisorDeTiempo)+1;
	return ModVal;

}
/********************************************************************************************************************************
*Nombre:        PwmChannelInit
*
*Definition:    Configura el canal de PWM alineado al centro.
*
*Entradas:      TpmModuloN: Selecciona uno de los  6 modulos TPM al cual pertenece el canal a configurar
*               Channel:    Selecciona el canal a configurar.
*               DutyCicleTiempo: Establece en segundos la duracion del duty cicle (tiempo inicial en alto)
*
*Salidas:       Ninguna.
*********************************************************************************************************************************/
void PwmChannelInit (uint8_t TpmModuloN, uint16_t CuentasPorTiempo, uint8_t Channel,uint16_t DutyCicleTiempo){

	switch (TpmModuloN)
	{
	case eTPM0:
		TPM0->CONTROLS[Channel].CnSC |= TPM_CnSC_MSB(1) | TPM_CnSC_MSA(0) | TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0);
		TPM0->CONTROLS[Channel].CnV  |= 164;
		break;
	case eTPM1:
		TPM1->CONTROLS[Channel].CnSC |= TPM_CnSC_MSB(1) | TPM_CnSC_MSB(0) | TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0);
		TPM1->CONTROLS[Channel].CnV  |= DutyCicleTiempo * CuentasPorTiempo;
		break;
	case eTPM2:
		TPM2->CONTROLS[Channel].CnSC |= TPM_CnSC_MSB(1) | TPM_CnSC_MSB(0) | TPM_CnSC_ELSB(1) | TPM_CnSC_ELSA(0);
		TPM2->CONTROLS[Channel].CnV  |= DutyCicleTiempo * CuentasPorTiempo;
		break;
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
