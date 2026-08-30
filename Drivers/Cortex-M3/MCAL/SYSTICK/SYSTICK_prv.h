/*
 * SYSTICK_prv.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_SYSTICK_SYSTICK_PRV_H_
#define MCAL_SYSTICK_SYSTICK_PRV_H_


#define SYSTICK_BASE_ADDR		0xE000E010U

typedef struct{
	u32 CTRL;
	u32 LOAD;
	u32 VAL;
	u32 CALIB;

}SYSTICK_MemMap_t;

#define SYSTICK		((volatile SYSTICK_MemMap_t*)(SYSTICK_BASE_ADDR))

/* Register bits */
#define COUNTFLAG	16
#define CLKSOURCE	2
#define TICKINT		1
#define ENABLE		0



#endif /* MCAL_SYSTICK_SYSTICK_PRV_H_ */
