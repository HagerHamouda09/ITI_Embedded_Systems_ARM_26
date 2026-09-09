/*
 * FMI_prv.h
 *
 *  Created on: Sep 9, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_FMI_FMI_PRV_H_
#define MCAL_FMI_FMI_PRV_H_

//M3
#define FMI_BASE_ADDR	0x40022000U

typedef struct{
	u32 ACR;
	u32 KEYR;
	u32 OPTKEYR;
	u32 SR;
	u32 CR;
	u32 AR;
	u32 Reserved1;
	u32 OBR;
	u32 WRPR;
}FMI_MemMap_t;

#define FMI ((volatile FMI_MemMap_t*)(FMI_BASE_ADDR))

#define KEY1 	0x45670123U
#define KEY2	0xCDEF89ABU

#define BSY 	0

#define STRT 	6
#define PG		0
#define PER		1
#define MER		2
#define LOCK	7



#endif /* MCAL_FMI_FMI_PRV_H_ */
