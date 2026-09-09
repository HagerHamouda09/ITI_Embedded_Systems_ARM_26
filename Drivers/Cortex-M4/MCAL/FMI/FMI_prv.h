/*
 * FMI_prv.h
 *
 *  Created on: Sep 9, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_FMI_FMI_PRV_H_
#define MCAL_FMI_FMI_PRV_H_

//M4
#define FMI_BASE_ADDR	0x40023C00U

typedef struct{
	u32 ACR;
	u32 KEYR;
	u32 OPTKEYR;
	u32 SR;
	u32 CR;
	u32 OPTCR;
}FMI_MemMap_t;

#define FMI ((volatile FMI_MemMap_t*)(FMI_BASE_ADDR))

#define KEY1 	0x45670123U
#define KEY2	0xCDEF89ABU

#define BSY 	16
#define STRT 	16
#define PG		0
#define SER		1
#define MER		2
#define PSIZE	8
#define LOCK	31
#define SNB		3


#endif /* MCAL_FMI_FMI_PRV_H_ */
