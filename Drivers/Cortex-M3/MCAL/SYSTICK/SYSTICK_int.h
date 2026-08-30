/*
 * SYSTICK_int.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_SYSTICK_SYSTICK_INT_H_
#define MCAL_SYSTICK_SYSTICK_INT_H_


typedef struct{
	u8 InterruptEnable;			//INT_ENABLE or INT_DISABLE
	u8 CLK_SRC;					// CLK_SRC_AHB or CLK_SRC_AHB_8
} MSYSTICK_Config_t;

void MSYSTICK_vInit(MSYSTICK_Config_t *A_xCfg);

void MSYSTICK_vStartTimer(u32 A_u32LoadValue);
void MSYSTICK_vStopTimer(void);

void MSYSTICK_vSetDelay_ms(f64 A_u32Delay_ms);
void MSYSTICK_vSetDelay_us(u32 A_u32Delay_us);

u32 MSYSTICK_u32GetElapsedTime_SingleShot(void);
u32 MSYSTICK_u32GetRemainingTime_SingleShot(void);

void MSYSTICK_vSetInterval_Single(u32 A_u32Delay_ms, void(*A_xFptr)(void));
void MSYSTICK_vSetInterval_Multi(u32 A_u32Delay_ms, void(*A_xFptr)(void));
void MSYSTICK_vSetIntervalMulti_us(u32 A_u32Delay_us, void (*A_xFptr)(void));

#define INT_ENABLE	1
#define INT_DISABLE	0

#define CLK_SRC_AHB	   1
#define CLK_SRC_AHB_8  2


#endif /* MCAL_SYSTICK_SYSTICK_INT_H_ */
