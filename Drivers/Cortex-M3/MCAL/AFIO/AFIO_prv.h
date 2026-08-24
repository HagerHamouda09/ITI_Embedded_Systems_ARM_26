/*
 * AFIO_prv.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_AFIO_AFIO_PRV_H_
#define MCAL_AFIO_AFIO_PRV_H_

#define AFIO_BASE_ADDR    0x40010000U


typedef struct
{
    u32 EVCR;              // 0x00
    u32 MAPR;              // 0x04
    u32 EXTICR[4];         // 0x08 : 0x14
    u32 MAPR2;             // 0x18
} AFIO_MemMap_t;

#define AFIO ((volatile AFIO_MemMap_t*)AFIO_BASE_ADDR)

#endif /* MCAL_AFIO_AFIO_PRV_H_ */
