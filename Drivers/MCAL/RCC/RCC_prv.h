/*
 * RCC_prv.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Hager Adel
 */

#ifndef RCC_PRV_H_
#define RCC_PRV_H_

#define RCC_BASE_ADDR	0x40023800U

typedef struct{
	u32 CR;
	u32 PLLCFGR;
	u32 CFGR;
	u32 CIR;
	u32 AHB1RSTR;
	u32 AHB2RSTR;

	u32 Reserved1;
	u32 Reserved2;

	u32 APB1RSTR;
	u32 APB2RSTR;

	u32 Reserved3;
	u32 Reserved4;

	u32 AHB1ENR;
	u32 AHB2ENR;

	u32 Reserved5;
	u32 Reserved6;

	u32 APB1ENR;
	u32 APB2ENR;

	u32 Reserved7;
	u32 Reserved8;

	u32 AHB1LPENR;
	u32 AHB2LPENR;

	u32 Reserved9;
	u32 Reserved10;

	u32 APB1LPENR;
	u32 APB2LPENR;

	u32 Reserved11;
	u32 Reserved12;

	u32 BDCR;
	u32 CSR;

	u32 Reserved13;
	u32 Reserved14;

	u32 SSCGR;
	u32 PLLI2SCFGR;
	u32 DCKCFGR;


}RCC_MemMap_t;


#define RCC 				((volatile RCC_MemMap_t*)(RCC_BASE_ADDR))


// SYSCLK OPTIONS
#define HSE_CLK	1
#define HSI_CLK	2
#define PLL_CLK	3

//BYPASS
#define RC_CLK			0
#define MECHANICAL_CLK	1



#endif /* RCC_PRV_H_ */
