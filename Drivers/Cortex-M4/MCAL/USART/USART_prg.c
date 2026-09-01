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


u8* G_pu8TxBuffer = NULL;
u8  G_u8TxLength  = 0;
u8  G_u8TxIndex   = 0;

u8 G_Buffer[50];
void (*G_Fpt[3])(void)={NULL};
// G_Fpt[0] -> USART1
// G_Fpt[1] -> USART2
// G_Fpt[2] -> USART6
void MUSART_vInit(void)
{
	// oversample by 16
	CLR_BIT(USART1->CR1, 15);

	// word length 8 data
	CLR_BIT(USART1->CR1, 12);

	// no parity
	CLR_BIT(USART1->CR1, 10);

	// baudrate 9600
	USART1->BRR = 162<<4 | 13;

	// 1 stop bit
	CLR_BIT(USART1->CR2, 12);
	CLR_BIT(USART1->CR2, 13);

	// Enable transmit
	SET_BIT(USART1->CR1, 3);

	// Enable Receive
	SET_BIT(USART1->CR1, 2);

	// enable usart
	SET_BIT(USART1->CR1, 13);

}

u8  MUSART_vReceive_synch(u8* A_pu8Byte)
{
	u8 L_u8status = 1 ;
	if( GET_BIT(USART1->SR, 5) == 1 )
	{
		*A_pu8Byte = USART1->DR ;
	}
	else
	{
		L_u8status = 0;
	}

	return L_u8status ;
}
void MUSART_vEnable_TX_Interrupt(void)
{
	SET_BIT(USART1->CR1,7);
}
void MUSART_vDisable_TX_Interrupt(void)
{
	CLR_BIT(USART1->CR1,7);
}
void MUSART_vEnable_TC_Interrupt(void)
{
	SET_BIT(USART1->CR1,6);
}
void MUSART_vDisable_TC_Interrupt(void)
{
	CLR_BIT(USART1->CR1,6);
}
void MUSART_vEnable_RX_Interrupt(void)
{
	SET_BIT(USART1->CR1,5);
}
void MUSART_vDisable_RX_Interrupt(void)
{
	CLR_BIT(USART1->CR1,5);
}
void MUSART_vSendData(u8 A_u8Data)
{
	/* Check whether register empty or not */
	while(!GET_BIT(USART1->SR,7))
		;
	USART1->DR = A_u8Data;

	/* Wait until data is transmitted */
	while(!GET_BIT(USART1->SR,6))
		;
	/* clear TC bit */
	CLR_BIT(USART1->SR, 6);
}
u8   MUSART_u8ReceiveData()
{
	while(!GET_BIT(USART1->SR,5))
		;

	return USART1->DR;
}
void MUSART_vSendString(char* A_u8ptrStr)
{
	u8 L_u8Index = 0;
	while(A_u8ptrStr[L_u8Index]!='\0')
		MUSART_vSendData(A_u8ptrStr[L_u8Index++]);

}
u8*  MUSART_u8ptrReceiveString(void) // check '\r' | '\n'
{
	u8 L_u8index = 0;
	// carrige return
	u8 ch ;

	while((ch = MUSART_u8ReceiveData()) != '\r' && ch != '\n')
	{
		G_Buffer[L_u8index] = ch;
		L_u8index++;
	}
	G_Buffer[L_u8index] = '\0';
	return G_Buffer;
}

void MUSART_vUSARTCallBack(u8 A_u8USARTNo, void(*Fptr)(void))
{
	// G_Fpt[0] -> USART1
	// G_Fpt[1] -> USART2
	// G_Fpt[2] -> USART6
	u8 L_u8index = (A_u8USARTNo==1)?0:(A_u8USARTNo==2)?1:2;
	G_Fpt[L_u8index] = Fptr;
}
void MUSART_vWrite_USART1_DataRegister(u8 A_u8Data)
{
	USART1->DR = A_u8Data;
}
u8   MUSART_u8Retreive_USART1_DataRegister(void)
{
	return USART1->DR;
}

void MUSART_vSendStringAsynch(u8* A_pu8String, u8 A_u8Length)
{
	/* Pass the string data to the global variables */
	G_pu8TxBuffer = A_pu8String;
	G_u8TxLength  = A_u8Length;
	G_u8TxIndex   = 0; // Start at the first character

	/* Enable the interrupt to begin transmission */
	MUSART_vEnable_TX_Interrupt();
}

void USART1_IRQHandler(void)
{
	/* Check whether register empty or not */
	if((GET_BIT(USART1->SR,7) == 1) && (GET_BIT(USART1->CR1, 7) == 1))
	{
		if(G_u8TxIndex < G_u8TxLength)
		{
			USART1->DR = G_pu8TxBuffer[G_u8TxIndex];
			G_u8TxIndex++;
		}else
		{
			MUSART_vDisable_TX_Interrupt();
			MUSART_vEnable_TC_Interrupt();
		}
	}

	/* Wait until data is transmitted */
	if((GET_BIT(USART1->SR,6) == 1) && (GET_BIT(USART1->CR1, 6) == 1))
	{
		/* Clear TC bit */
		CLR_BIT(USART1->SR, 6);
		MUSART_vDisable_TC_Interrupt();
	}

}

// void USART1_IRQHandler(void)
// {
// 	// TC interrupt
// 	if(GET_BIT(USART1->SR, 6))
// 	{
// 		// clear TC
// 		CLR_BIT(USART1->SR, 6);
// 		if(G_Fpt[0] != NULL) G_Fpt[0]();

// 	}else{
// 		if(G_Fpt[0] != NULL) G_Fpt[0]();
// 	}

// }
