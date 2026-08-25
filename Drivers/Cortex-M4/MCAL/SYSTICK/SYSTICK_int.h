/*
 * SYSTICK_int.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_SYSTICK_SYSTICK_INT_H_
#define MCAL_SYSTICK_SYSTICK_INT_H_
typedef struct{
	u8 InterruptEnable;
	u8 CLK_SRC;
}MSYSTIC_Config_t;

void MSYSTICK_vInit(MSYSTIC_Config_t *A_xCfg);

void MSYSTICK_vStartTimer(u32 A_u32LoadValue);
void MSYSTICK_vStopTimer(void);

u32 MSYSTICK_u32GetElapsedTime_SingleShot(void);
u32 MSYSTICK_u32GetRemainingTime_SingleShot(void);

void MSYSTICK_vSetDelay_ms(f64 A_f64Delay_ms);

void MSYSTICK_vSetDelay_us(f64 A_f64Delay_us);

void MSYSTICK_vSetIntervalSingle(u32 A_u32Delay_ms, void (*Fptr)(void));

void MSYSTICK_vSetIntervalMulti(u32 A_u32Delay_ms, void (*Fptr)(void));


#define INT_ENABLE	1
#define INT_DISABLE	0

#define CLK_SRC_AHB		1
#define CLK_SRC_AHB_8	2

#endif /* MCAL_SYSTICK_SYSTICK_INT_H_ */
