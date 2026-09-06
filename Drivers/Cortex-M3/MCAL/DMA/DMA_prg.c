/*
 * DMA_prg.c
 *
 *  Created on: Sep 5, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "DMA_int.h"
#include "DMA_prv.h"

static void (*G_Fptr[8])(void)={NULL};

void MDMA_vInit(u8 A_u8ChannelID)
{
	// Cheam disable
	CLR_BIT(DMA1->Ch[A_u8ChannelID].CCR,0);

	// MEMORY TO MEMORY
	SET_BIT(DMA1->Ch[A_u8ChannelID].CCR, 14);

	// Source / des increment
	SET_BIT(DMA1->Ch[A_u8ChannelID].CCR, 7);
	SET_BIT(DMA1->Ch[A_u8ChannelID].CCR, 6);

	// Enable Interrupt
	SET_BIT(DMA1->Ch[A_u8ChannelID].CCR, 1);

}

void MDMA_vSetChannelCfg(
u8 A_u8ChannelID,
u32 *A_u32SourceAddr,
u32 *A_u32DestAddr,
Size_t A_xPSize,
Size_t A_xMSize,
u16 A_u16BlockSize
)
{
	// set source and dest add
	DMA1->Ch[A_u8ChannelID].CPAR =(u32)A_u32SourceAddr;
	DMA1->Ch[A_u8ChannelID].CMAR =(u32)A_u32DestAddr;

	// select size
	DMA1->Ch[A_u8ChannelID].CCR &= ~(0b1111 << 8);
	DMA1->Ch[A_u8ChannelID].CCR |= (A_xPSize << 8);
	DMA1->Ch[A_u8ChannelID].CCR |= (A_xMSize << 10);

	// select block size
	DMA1->Ch[A_u8ChannelID].CNDTR = A_u16BlockSize;

}

void MDMA1_vEnableChannel(u8 A_u8ChannelID)
{
	SET_BIT(DMA1->Ch[A_u8ChannelID].CCR,0);
}

void MDMA1_vCallBack(u8 A_u8ChannelID, void(*A_xFptr)(void))
{
	G_Fptr[A_u8ChannelID] = A_xFptr;
}

void DMA1_Channel1_IRQHandler(void)
{
	if(G_Fptr[0] !=NULL)
	{
		// 0 tog led function
		G_Fptr[0]();
	}
DMA1->IFCR |= (1U<<0) | (1U<<1) | (1U<<2) | (1U<<3);
}

