/*
 * I2C.h
 *
 *  Created on: 6 dic. 2020
 *      Author: Hugo
 */

#ifndef I2C_H_
#define I2C_H_

#include "MKL25Z4.h"

#define RED_ON                 GPIOB->PDOR &= ~(1<<18)
#define RED_OFF                GPIOB->PDOR |= (1<<18)
#define BLUE_ON                GPIOD->PDOR &= ~(1<<1)
#define BLUE_OFF               GPIOD->PDOR |= (1<<1)
#define GREEN_ON               GPIOB->PDOR &= ~(1<<19)
#define GREEN_OFF              GPIOB->PDOR |= (1<<19)

#define I2C_DisableAck()       I2C0->C1 |= I2C_C1_TXAK_MASK
#define I2C_EnableAck()        I2C0->C1 &= ~I2C_C1_TXAK_MASK
#define I2C_RepeatedStart()    I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_EnterRxMode()      I2C0->C1 &= ~I2C_C1_TX_MASK
#define I2C_write_byte(data)   I2C0_D = data

#define I2C_Start()            I2C0->C1 |= I2C_C1_TX_MASK;\
                               I2C0->C1 |= I2C_C1_MST_MASK

#define I2C_Stop()             I2C0->C1 &= ~I2C_C1_MST_MASK;\
                               I2C0->C1 &= ~I2C_C1_TX_MASK

#define I2C_Wait()             while((I2C0->S & I2C_S_IICIF_MASK)==0) {} \
                               I2C0->S |= I2C_S_IICIF_MASK;

void           I2C_WriteRegister      (unsigned char u8SlaveAddress, unsigned char u8RegisterAddress, /*unsigned*/ char u8Data);
unsigned char  I2C_ReadRegister       (unsigned char u8SlaveAddress, unsigned char u8RegisterAddress);
void           I2C_ReadMultiRegisters (unsigned char u8SlaveAddress, unsigned char u8RegisterAddress, unsigned char n, unsigned char *r);
void           Pause                  (int number);
void           Init_pins              (void);


#endif /* I2C_H_ */
