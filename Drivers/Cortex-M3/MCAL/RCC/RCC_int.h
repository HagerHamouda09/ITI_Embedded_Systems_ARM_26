/*
 * RCC_int.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.1
 */

#ifndef MCAL_RCC_RCC_INT_H_
#define MCAL_RCC_RCC_INT_H_


/* Buses */
#define RCC_AHB     0
#define RCC_APB1    1
#define RCC_APB2    2

/* ------- Peripherals --------*/
// Bus : RCC_APB2 (Note: GPIOs on STM32F1 are on APB2)

#define RCC_GPIOA   2
#define RCC_GPIOB   3
#define RCC_GPIOC   4


void MRCC_vInit(void);
void MRCC_vEnableClk(u8 A_u8BusID, u8 A_u8PeripheralID);
void MRCC_vDisableClk(u8 A_u8BusID, u8 A_u8PeripheralID);

#endif /* MCAL_RCC_RCC_INT_H_ */
