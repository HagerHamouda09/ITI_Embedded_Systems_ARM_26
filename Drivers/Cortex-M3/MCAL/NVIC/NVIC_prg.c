/*
 * NVIC_prg.c
 *
 *  Created on: Aug 19, 2026
 *      Author: Hager Adel
 */


#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "NVIC_int.h"
#include "NVIC_prv.h"

u8 G_u8GroupPriority = 0;

void MNVIC_vEnable_Peripheral_INT(u8 A_u8Position)
{
	SET_BIT(NVIC->ISERx[A_u8Position/32], A_u8Position % 32);
}
void MNVIC_vDisable_Peripheral_INT(u8 A_u8Position)
{
	SET_BIT(NVIC->ICERx[A_u8Position/32], A_u8Position % 32);
}
void MNVIC_vSetPendingFlag(u8 A_u8Position)
{
	SET_BIT(NVIC->ISPRx[A_u8Position/32], A_u8Position % 32);
}
void MNVIC_vCLRPendingFlag(u8 A_u8Position)
{
	SET_BIT(NVIC->ICPRx[A_u8Position/32], A_u8Position % 32);
}
u8 MNVIC_u8GETFlagStatus(u8 A_u8Position)
{
	return 	GET_BIT(NVIC->IABRx[A_u8Position/32], A_u8Position % 32);
}
void MNVIC_vSetGroupPriority(NVIC_Group_t A_xGroupPriority)
{
	G_u8GroupPriority = A_xGroupPriority;
	SCB_AIRCR = VECTKEY | (A_xGroupPriority<<8);
}
void MNVIC_vSetPeripheralPeriority(u8 A_u8Position, u8 A_u8Group, u8 A_u8SubGroup)
{

	switch(G_u8GroupPriority)
	{
	case Group16Sub0:
		NVIC->IPRx[A_u8Position] = A_u8Group << 4;
		break;
	case Group8Sub2:
		NVIC->IPRx[A_u8Position] = A_u8Group << 5 | A_u8SubGroup <<4;
		break;
	case Group4Sub4:
		NVIC->IPRx[A_u8Position] = A_u8Group << 6 | A_u8SubGroup <<4;
		break;
	case Group2Sub8:
		NVIC->IPRx[A_u8Position] = A_u8Group << 7 | A_u8SubGroup <<4;
		break;
	case Group0Sub16:
		NVIC->IPRx[A_u8Position] = A_u8SubGroup << 4;
		break;
	}
}

