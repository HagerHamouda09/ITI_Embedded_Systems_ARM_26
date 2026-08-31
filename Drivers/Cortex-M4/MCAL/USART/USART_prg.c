/*
 * USART_prg.c
 *
 *  Created on: Aug 30, 2026
 *      Author: Hager Adel
 */



#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "USART_int.h"
#include "USART_prv.h"


void MUSART_vInit(void)
{
	// OVERSAMPLE BY 16
CLR_BIT(USART1->CR1, 15);

	// DATA LENGTH 8
CLR_BIT(USART1->CR1, 12);

	// NO PARITY
CLR_BIT(USART1->CR1, 10);

	//BAUD RATE 9600
	USART1->BRR = (162<<4) | 13;

	//1 STOP BIT
CLR_BIT(USART1->CR2, 12);
CLR_BIT(USART1->CR2, 13);

// TRANSIMTER ENABLE
SET_BIT(USART1->CR1, 3);

// RECEIVER ENABLE
SET_BIT(USART1->CR1, 2);

// USART ENABLE
SET_BIT(USART1->CR1, 13);


}

void MUSART_vSendData(u8 A_u8Data)
{
	while(!GET_BIT(USART1->SR, 7));

	USART1->DR = A_u8Data;

	while(!GET_BIT(USART1->SR, 6));

	CLR_BIT(USART1->SR, 6);

}
u8 MUSART_u8ReceiveData(void)
{
while(!GET_BIT(USART1->SR, 5));

return USART1->DR;
}





