/*
 * SYSTICK_prg.c
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#include  "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "SYSTICK_int.h"
#include "SYSTICK_prv.h"
#include "SYSTICK_cfg.h"


static void(*G_xFptr)(void) = NULL;
static u8 G_u8SingleFlag =0;
void MSYSTICK_vInit(MSYSTIC_Config_t *A_xCfg)
{
	//STOP
	CLR_BIT(SYSTICK->CTRL, ENABLE);

	if(A_xCfg->InterruptEnable == INT_ENABLE){
		SET_BIT(SYSTICK->CTRL, TICKINT);
	}
	else if(A_xCfg->InterruptEnable == INT_DISABLE){
		CLR_BIT(SYSTICK->CTRL, TICKINT);
	}

	if(A_xCfg->CLK_SRC == CLK_SRC_AHB){
		SET_BIT(SYSTICK->CTRL, CLKSOURCE);
	}
	else if(A_xCfg->CLK_SRC == CLK_SRC_AHB_8){
		CLR_BIT(SYSTICK->CTRL, CLKSOURCE);
	}
}

void MSYSTICK_vStartTimer(u32 A_u32LoadValue)
{
SYSTICK->LOAD = A_u32LoadValue;
SYSTICK->VAL = 0;

SET_BIT(SYSTICK->CTRL, ENABLE);

}
void MSYSTICK_vStopTimer(void)
{
	CLR_BIT(SYSTICK->CTRL, ENABLE);
	SYSTICK->VAL = 0;
}

u32 MSYSTICK_u32GetElapsedTime_SingleShot(void)
{
	return (SYSTICK->LOAD) - (SYSTICK->VAL);
}
u32 MSYSTICK_u32GetRemainingTime_SingleShot(void)
{
	return (SYSTICK->VAL);

}

void MSYSTICK_vSetDelay_ms(f64 A_f64Delay_ms)
{
// 25M AHB/8
	//25M/8 3125000
	//3125

	u32 L_u32Ticks =(u32)( A_f64Delay_ms * 3125.0);

	SYSTICK->VAL=0;

	if((L_u32Ticks>= 0x00000001) && (L_u32Ticks< 0x00FFFFFF) )
	{
		MSYSTICK_vStartTimer(L_u32Ticks);
		while(!GET_BIT(SYSTICK->CTRL, COUNTFLAG));
		MSYSTICK_vStopTimer();
	}

}
void MSYSTICK_vSetDelay_us(f64 A_f64Delay_us)
{
// 25M AHB/8
	//25M/8 3125000
	//3125

	u32 L_u32Ticks =(u32)( A_f64Delay_us * 3.125);

	SYSTICK->VAL=0;

	if((L_u32Ticks>= 0x00000001) && (L_u32Ticks< 0x00FFFFFF) )
	{
		MSYSTICK_vStartTimer(L_u32Ticks);
		while(!GET_BIT(SYSTICK->CTRL, COUNTFLAG));
		MSYSTICK_vStopTimer();
	}

}

void MSYSTICK_vSetIntervalSingle(u32 A_u32Delay_ms, void (*Fptr)(void))
{
	G_u8SingleFlag =1;

	u32 L_u32Ticks =(u32)( A_u32Delay_ms * 3125.0);

	G_xFptr = Fptr;
	SYSTICK->VAL=0;

	if((L_u32Ticks>= 0x00000001) && (L_u32Ticks< 0x00FFFFFF) )
	{
		MSYSTICK_vStartTimer(L_u32Ticks);
	}

}

void MSYSTICK_vSetIntervalMulti(u32 A_u32Delay_ms, void (*Fptr)(void))
{
	G_u8SingleFlag =0;

	u32 L_u32Ticks =(u32)( A_u32Delay_ms * 3125.0);

	G_xFptr = Fptr;
	SYSTICK->VAL=0;

	if((L_u32Ticks>= 0x00000001) && (L_u32Ticks< 0x00FFFFFF) )
	{
		MSYSTICK_vStartTimer(L_u32Ticks);
	}


}

void MSYSTICK_vSetIntervalMulti_us(u32 A_u32Delay_us, void (*Fptr)(void))
{
    G_u8SingleFlag = 0;

    u32 L_u32Ticks = (u32)(A_u32Delay_us * 3.125);

    G_xFptr = Fptr;
    SYSTICK->VAL = 0;

    if((L_u32Ticks >= 1) && (L_u32Ticks < 0x00FFFFFF))
    {
        MSYSTICK_vStartTimer(L_u32Ticks);
    }
}


void SysTick_Handler (void)
{
if(G_xFptr !=NULL)
{
	G_xFptr();
}
if(G_u8SingleFlag == 1)
{
MSYSTICK_vStopTimer();
}
}

