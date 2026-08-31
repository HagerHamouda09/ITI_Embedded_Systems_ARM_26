/*
 * USART_prv.h
 *
 *  Created on: Aug 30, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_USART_USART_PRV_H_
#define MCAL_USART_USART_PRV_H_


#define USART1_BASE_ADDR	0x40013800U
#define USART2_BASE_ADDR	0x40004400U
#define USART3_BASE_ADDR	0x40004800U


typedef struct
{
	u32 SR;
	u32 DR;
	u32 BRR;
	u32 CR1;
	u32 CR2;
	u32 CR3;
	u32 GTPR;

}USARTx_MemMap_t;

#define USART1		((volatile USARTx_MemMap_t*)(USART1_BASE_ADDR))
#define USART2		((volatile USARTx_MemMap_t*)(USART2_BASE_ADDR))
#define USART3		((volatile USARTx_MemMap_t*)(USART5_BASE_ADDR))




#endif /* MCAL_USART_USART_PRV_H_ */
