/*
 * EXTI_prg.c
 *
 *  Created on: Aug 21, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "EXTI_int.h"
#include "EXTI_prv.h"

static void (*G_Fptr[16])(void) = {NULL} ;

void MEXTI_vInit(void)
{
	// CLEAR FLAG
	EXTI->PR = 0xFFFFFFFF;
}

void MEXTI_vEnableINT(u8 A_u8LineNo)
{
	if(A_u8LineNo <16)
	{
		SET_BIT(EXTI->IMR, A_u8LineNo);
	}
}

void MEXTI_vDisableINT(u8 A_u8LineNo)
{
	if(A_u8LineNo <16)
	{
		CLR_BIT(EXTI->IMR, A_u8LineNo);
	}
}

void MEXTI_vSetTrigger(u8 A_u8LineNo, u8 A_u8Trigger_t)
{
	if(A_u8LineNo <16)
		{
			switch(A_u8Trigger_t)
			{
			case EXTI_RISING_EDGE:
				SET_BIT(EXTI->RTSR, A_u8LineNo );
				CLR_BIT(EXTI->FTSR, A_u8LineNo );
				break;

			case EXTI_FALLING_EDGE:
				CLR_BIT(EXTI->RTSR, A_u8LineNo );
				SET_BIT(EXTI->FTSR, A_u8LineNo );
				break;

			case EXTI_ONCHANGE_EDGE:
				SET_BIT(EXTI->RTSR, A_u8LineNo );
				SET_BIT(EXTI->FTSR, A_u8LineNo );
				break;
			}
		}
}

void MEXTI_vSoftwareINT (u8 A_u8LineNo)
{
	if(A_u8LineNo <16)
		{
			SET_BIT(EXTI->SWIER ,A_u8LineNo);
		}
}


void MEXTI_vSetCallBack(void(*A_xFptr)(void), u8 A_u8LineNo)
{
	if(A_u8LineNo <16)
			{
	// POINTER -> FUNCTION I WANT TO EXECUTE IN EXTIx 0.1.2.3
	G_Fptr[A_u8LineNo] = A_xFptr;
			}
}

//ISR
void EXTI0_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE0] != NULL)
	{
	G_Fptr[EXTI_LINE0]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE0);
}

void EXTI1_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE1] != NULL)
	{
	G_Fptr[EXTI_LINE1]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE1);
}

void EXTI2_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE2] != NULL)
	{
	G_Fptr[EXTI_LINE2]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE2);
}

void EXTI3_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE3] != NULL)
	{
	G_Fptr[EXTI_LINE3]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE3);
}

void EXTI4_IRQHandler(void)
{
	if(G_Fptr[EXTI_LINE4] != NULL)
	{
	G_Fptr[EXTI_LINE4]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE4);
}

void EXTI9_5_IRQHandler(void)
{
	if(GET_BIT(EXTI->PR, EXTI_LINE5))
	{
	if(G_Fptr[EXTI_LINE5] != NULL)
	{
	G_Fptr[EXTI_LINE5]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE5);
	}

	if(GET_BIT(EXTI->PR, EXTI_LINE6))
	{
	if(G_Fptr[EXTI_LINE6] != NULL)
	{
	G_Fptr[EXTI_LINE6]();
	}
	SET_BIT(EXTI->PR , EXTI_LINE6);
	}
}
