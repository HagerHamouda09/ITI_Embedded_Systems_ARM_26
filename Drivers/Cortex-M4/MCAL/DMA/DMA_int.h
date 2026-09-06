/*
 * DMA_int.h
 *
 *  Created on: Sep 5, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_DMA_DMA_INT_H_
#define MCAL_DMA_DMA_INT_H_

typedef enum{
	Byte=0b00,
	HalfWord,
	Word
}Size_t;

typedef enum{
	FIFO_1_4=0,
	FIFO_1_2,
	FIFO_3_4,
	FIFO_FULL
}Threshold_t;

void MDMA2_vInit(u8 A_u8StreamID);

void MDMA2_vSetStreamCfg(
u8 A_u8StreamID,
u32 *A_u32SourceAddr,
u32 *A_u32DestAddr,
Size_t A_xPSize,
Size_t A_xMSize,
u16 A_u16BlockSize,
Threshold_t A_xThreshold
);

void MDMA2_vEnableStream(u8 A_u8StreamID);

void MDMA2_vCallBack(u8 A_u8StreamID, void(*A_xFptr)(void));



#endif /* MCAL_DMA_DMA_INT_H_ */
