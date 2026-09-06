/*
 * DMA_prv.h
 *
 *  Created on: Sep 5, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_DMA_DMA_PRV_H_
#define MCAL_DMA_DMA_PRV_H_


#define DMA1_BASE_ADDR	0x40026000U
#define DMA2_BASE_ADDR	0x40026400U

typedef struct{
	u32 CR;
	u32 NDTR;
	u32 PAR;
	u32 M0AR;
	u32 M1AR;
	u32 FCR;
}DMA_Stream_t;

typedef struct{
	u32 LISR;
	u32 HISR;
	u32 LIFCR;
	u32 HIFCR;
	DMA_Stream_t Str[8];

}DMA_MemMap_t;

// CR 0
//DMA2 ->Str[0].CR



#define DMA1	((volatile DMA_MemMap_t* )(DMA1_BASE_ADDR))
#define DMA2	((volatile DMA_MemMap_t* )(DMA2_BASE_ADDR))

#endif /* MCAL_DMA_DMA_PRV_H_ */
