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

static void(*G_xFptr)(void) = NULL;
static u8 G_u8SingleFlag = 0;

void MSYSTICK_vInit(MSYSTICK_Config_t *A_xCfg)
{
    /* Stop SysTick */
    CLR_BIT(SYSTICK->CTRL, ENABLE);

    /* Interrupt */
    #if SYSTICK_INT_STATE == INT_ENABLE
        SET_BIT(SYSTICK->CTRL, TICKINT);
    #else
        CLR_BIT(SYSTICK->CTRL, TICKINT);
    #endif

    /* Clock source */
    #if SYSTICK_CLK_SRC == CLK_SRC_AHB
        SET_BIT(SYSTICK->CTRL, CLKSOURCE);
    #else
        CLR_BIT(SYSTICK->CTRL, CLKSOURCE);
    #endif
}

void MSYSTICK_vStartTimer(u32 A_u32LoadValue)
{
    SYSTICK->LOAD = A_u32LoadValue;
    SYSTICK->VAL = 0;
    SET_BIT(SYSTICK->CTRL, ENABLE);
}

void MSYSTICK_vStopTimer(void)
{
	/* Stop Systick Counter */
	CLR_BIT(SYSTICK->CTRL, ENABLE);

	/* Reset Timer */
		SYSTICK->VAL = 0;
}

void MSYSTICK_vSetDelay_ms(f64 A_u32Delay_ms)
{
    u32 L_u32Ticks = (u32)(A_u32Delay_ms * 1000.0);

    SYSTICK->VAL = 0;

    if((L_u32Ticks >= 1) && (L_u32Ticks < 0x00FFFFFF))
    {
        MSYSTICK_vStartTimer(L_u32Ticks);

        while(GET_BIT(SYSTICK->CTRL, COUNTFLAG) == 0)
            ;

        MSYSTICK_vStopTimer();
    }
}

void MSYSTICK_vSetDelay_us(u32 A_u32Delay_us)
{
    u32 L_u32Ticks = A_u32Delay_us;

    SYSTICK->VAL = 0;

    if((L_u32Ticks >= 1) && (L_u32Ticks < 0x00FFFFFF))
    {
        MSYSTICK_vStartTimer(L_u32Ticks);

        while(GET_BIT(SYSTICK->CTRL, COUNTFLAG) == 0)
            ;

        MSYSTICK_vStopTimer();
    }
}

u32 MSYSTICK_u32GetElapsedTime_SingleShot(void)
{
	return (SYSTICK->LOAD) - (SYSTICK->VAL);
}
u32 MSYSTICK_u32GetRemainingTime_SingleShot(void)
{
	return (SYSTICK->VAL);
}

void MSYSTICK_vSetInterval_Single(u32 A_u32Delay_ms, void(*A_xFptr)(void))
{
    G_u8SingleFlag = 1;

    u32 L_u32Ticks = A_u32Delay_ms * 1000;

    G_xFptr = A_xFptr;

    SYSTICK->VAL = 0;

    if((L_u32Ticks >= 1) && (L_u32Ticks < 0x00FFFFFF))
    {
        MSYSTICK_vStartTimer(L_u32Ticks);
    }
}

void MSYSTICK_vSetInterval_Multi(u32 A_u32Delay_ms, void(*A_xFptr)(void))
{
    G_u8SingleFlag = 0;

    u32 L_u32Ticks = A_u32Delay_ms * 1000;

    G_xFptr = A_xFptr;

    SYSTICK->VAL = 0;

    if((L_u32Ticks >= 1) && (L_u32Ticks < 0x00FFFFFF))
    {
        MSYSTICK_vStartTimer(L_u32Ticks);
    }
}

void MSYSTICK_vSetIntervalMulti_us(u32 A_u32Delay_us, void (*A_xFptr)(void))
{
    G_u8SingleFlag = 0;

    u32 L_u32Ticks = A_u32Delay_us;

    G_xFptr = A_xFptr;

    SYSTICK->VAL = 0;

    if((L_u32Ticks >= 1) && (L_u32Ticks < 0x00FFFFFF))
    {
        MSYSTICK_vStartTimer(L_u32Ticks);
    }
}

void SysTick_Handler(void)
{
	if(G_xFptr != NULL)
	{
		G_xFptr();

	}
	if(G_u8SingleFlag == 1)
	{
		MSYSTICK_vStopTimer();
	}

}

