/*
 * SYSCFG.prv.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_SYSCFG_SYSCFG_PRV_H_
#define MCAL_SYSCFG_SYSCFG_PRV_H_

#define SYSCFG_BASE_ADDR		0x40013800U

typedef struct{
	u32 MEMRMP;
	u32 PMC;
	u32 EXTICRx[4];
	u32 CMPCR;
}SYSCFG_MemMap_t;

#define SYSCFG		((volatile SYSCFG_MemMap_t*)(SYSCFG_BASE_ADDR))




#endif /* MCAL_SYSCFG_SYSCFG_PRV_H_ */
