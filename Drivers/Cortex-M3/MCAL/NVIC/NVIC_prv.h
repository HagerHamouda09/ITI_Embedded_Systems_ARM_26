/*
 * NVIC_prv.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_NVIC_NVIC_PRV_H_
#define MCAL_NVIC_NVIC_PRV_H_


#define NVIC_BASE_ADDR		0xE000E100U

typedef struct{
	u32 ISERx[3];
	u32 Reserved0[29];
	u32 ICERx[3];
	u32 Reserved1[29];
	u32 ISPRx[3];
	u32 Reserved2[29];
	u32 ICPRx[3];
	u32 Reserved3[29];
	u32 IAPRx[3];
	u32 Reserved4[61];
	//u32 IPRx[60];
	u8 IPRx[240];
	u32 Reserved5[683];
	u32 STIR;

}NVIC_MemMap_t;

#define NVIC ((volatile NVIC_MemMap_t*)(NVIC_BASE_ADDR))

#define SCB_AIRCR *((volatile u32*)(0xE000ED0C))

#define VECTKEY	0x05FA0000

#endif /* MCAL_NVIC_NVIC_PRV_H_ */
