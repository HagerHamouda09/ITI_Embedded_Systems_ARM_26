/*
 * EXTI_int.h
 *
 *  Created on: Aug 21, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_EXTI_EXTI_INT_H_
#define MCAL_EXTI_EXTI_INT_H_

//typedef struct
//{
//	u8 LineNo;
//	u8 Trigger_t;
//}EXTI_Cng_t;

void MEXTI_vInit(void);
void MEXTI_vEnableINT(u8 A_u8LineNo);
void MEXTI_vDisableINT(u8 A_u8LineNo);
void MEXTI_vSetTrigger(u8 A_u8LineNo, u8 A_u8Trigger_t);
void MEXTI_vSoftwareINT (u8 A_u8LineNo);
void MEXTI_vSetCallBack(void(*A_xFptr)(void), u8 A_u8LineNo);

/* Line Number == Pin number in any port */
#define EXTI_LINE0		0
#define EXTI_LINE1		1
#define EXTI_LINE2		2
#define EXTI_LINE3		3
#define EXTI_LINE4		4
#define EXTI_LINE5		5
#define EXTI_LINE6		6
#define EXTI_LINE7		7
#define EXTI_LINE8		8
#define EXTI_LINE9		9
#define EXTI_LINE10		10
#define EXTI_LINE11		11
#define EXTI_LINE12		12
#define EXTI_LINE13		13
#define EXTI_LINE14		14
#define EXTI_LINE15		15

/* Trigger Type */
#define	EXTI_RISING_EDGE	0
#define EXTI_FALLING_EDGE	1
#define EXTI_ONCHANGE_EDGE 	2



#endif /* MCAL_EXTI_EXTI_INT_H_ */
