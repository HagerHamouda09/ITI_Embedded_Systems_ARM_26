/*
 * RCC_prg.c
 *
 *  Created on: Aug 16, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "RCC_int.h"
#include "RCC_prv.h"
#include "RCC_cfg.h"


void MRCC_vInit(void)
{
#if RCC_SYS_CLK == HSE_CLK

#if	HSE_BYPASS	== RC_CLK

	SET_BIT(RCC->CR,HSEBYP);

#elif HSE_BYPASS == MECHANICAL_CLK

	CLR_BIT(RCC->CR,HSEBYP);

#endif

	//HSE on
	SET_BIT(RCC->CR,16);

	while(!GET_BIT(RCC->CR, 17));

	CLR_BIT(RCC->CFGR, 1);
	SET_BIT(RCC->CFGR, 0);

#elif	RCC_SYS_CLK == HSI_CLK

	SET_BIT(RCC->CR, 0);

	while(!GET_BIT(RCC->CR, 1));

	CLR_BIT(RCC->CFGR, 1);
	CLR_BIT(RCC->CFGR, 0);


#elif	RCC_SYS_CLK == PLL_CLK

#else
#error "invalid option"
#endif

}



void MRCC_vEnableCLK(u8 A_u8BusID, u8 A_u8PeripheralID)
{
	switch(A_u8BusID)
	{
//		switch(A_u8PeripheralID)
//		{
//		case RCC_GPIOA:
//			SET_BIT(RCC->AHB1ENR, 0);
//		case RCC_GPIOB:
//				SET_BIT(RCC->AHB1ENR, 1);
//		}
	case RCC_AHB1:
		SET_BIT(RCC->AHB1ENR, A_u8PeripheralID);
		break;

	case RCC_AHB2:
		SET_BIT(RCC->AHB2ENR, A_u8PeripheralID);

		break;

	case RCC_APB1:
		SET_BIT(RCC->APB1ENR, A_u8PeripheralID);

			break;

	case RCC_APB2:
		SET_BIT(RCC->APB2ENR, A_u8PeripheralID);

				break;
	}
}

void MRCC_vDisableCLK(u8 A_u8BusID, u8 A_u8PeripheralID)
{
	switch(A_u8BusID)
	{

	case RCC_AHB1:
		CLR_BIT(RCC->AHB1ENR, A_u8PeripheralID);
		break;

	case RCC_AHB2:
		CLR_BIT(RCC->AHB2ENR, A_u8PeripheralID);

		break;

	case RCC_APB1:
		CLR_BIT(RCC->APB1ENR, A_u8PeripheralID);

			break;

	case RCC_APB2:
		CLR_BIT(RCC->APB2ENR, A_u8PeripheralID);

				break;
	}
}
