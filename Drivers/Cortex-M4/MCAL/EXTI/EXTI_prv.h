/*
 * EXTI_prv.h
 *
 *  Created on: Aug 21, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_EXTI_EXTI_PRV_H_
#define MCAL_EXTI_EXTI_PRV_H_

#define EXTI_BASE_ADDR	0x40013C00U

typedef struct{
	u32 IMR;
	u32 EMR;
	u32 RTSR;
	u32 FTSR;
	u32 SWIER;
	u32 PR;
}EXTI_MemMap_t;

#define EXTI	((EXTI_MemMap_t*)(EXTI_BASE_ADDR))

#endif /* MCAL_EXTI_EXTI_PRV_H_ */
