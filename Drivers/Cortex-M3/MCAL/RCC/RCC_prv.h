/*
 * RCC_prv.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.1
 */

#ifndef MCAL_RCC_RCC_PRV_H_
#define MCAL_RCC_RCC_PRV_H_

#define RCC_BASE_ADDR 		0x40021000U

typedef struct{
    u32 CR;
    u32 CFGR;
    u32 CIR;
    u32 APB2RSTR;
    u32 APB1RSTR;
    u32 AHBENR;
    u32 APB2ENR;
    u32 APB1ENR;
    u32 BDCR;
    u32 CSR;
}RCC_MemMap_t;


#define RCC 				((volatile RCC_MemMap_t*)(RCC_BASE_ADDR))


/* RCC_SYS_CLK --- options --- */
#define HSE_CLK		1
#define HSI_CLK		2
#define PLL_CLK		3

/* HSE_BYPASS --- options --- */
#define RC_CLK			0
#define MECHANICAL_CLK  1




/* Resigters */

#define HSEBYP		18

#define GPIOAEN		2
#define GPIOBEN		3


#endif /* MCAL_RCC_RCC_PRV_H_ */
