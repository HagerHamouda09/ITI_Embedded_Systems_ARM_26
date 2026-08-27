/*
 * OS_prg.c
 *
 *  Created on: Aug 24, 2026
 *      Author: Hager Adel
 */


#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/SYSTICK/SYSTICK_int.h"

#include "OS_int.h"
#include "OS_prv.h"
#include "OS_cfg.h"

Task System_Tasks[MAX_NO_TASKS] = {{0}} ;
u32 TimingTask[MAX_NO_TASKS];

static void OS_Scheduler(void);

void OS_vStart(void)
{
	MSYSTIC_Config_t STK_cfg={
			.InterruptEnable = INT_ENABLE,
			.CLK_SRC = CLK_SRC_AHB_8
	};
	MSYSTICK_vInit(&STK_cfg);

	MSYSTICK_vSetIntervalMulti(TICKTIME , OS_Scheduler);
}


u8 OS_u8CreateTask(void (*A_xFptr)(void),u32 A_u32Periodicity,u8 A_u8Priority, u32 A_u32FirstDelay)
{
	u8 L_u8ErrorState = 0;
	//Range
	if((A_u8Priority >=0) && (A_u8Priority < MAX_NO_TASKS))
	{
		// if there is no task in this index
		if(System_Tasks[A_u8Priority].Fptr == 0)
		{
			System_Tasks[A_u8Priority].Fptr = A_xFptr;
			System_Tasks[A_u8Priority].Periodicity = A_u32Periodicity;
			System_Tasks[A_u8Priority].State = Running;
			TimingTask[A_u8Priority] = A_u32FirstDelay;

		}else
		{
			L_u8ErrorState = RESERVED_PRIORITY_ERROR;
		}

	}else
	{
		L_u8ErrorState = OUT_OF_RANGE_ERROR;
	}

	return L_u8ErrorState;
}

static void OS_Scheduler(void)
{
	for(u8 counter=0; counter<MAX_NO_TASKS; counter++)
	{

		if(System_Tasks[counter].State == Running)
		{
			if((TimingTask[counter] == 0) || (TimingTask[counter] > System_Tasks[counter].Periodicity))
			{
				/* Run task*/
				System_Tasks[counter].Fptr();

				/*Reload */
				TimingTask[counter] = System_Tasks[counter].Periodicity;

			}else
			{

				TimingTask[counter]--;
			}

		}else
		{

		}
	}
}

u8 OS_u8SuspendTask(u8 A_u8Priority)
{
	u8 L_u8ErrorState = 0;
	if(System_Tasks[A_u8Priority].State == Running)
	{
		System_Tasks[A_u8Priority].State = Suspended;
	}else
	{
		L_u8ErrorState = NO_TASK_ERROR;
	}
	return L_u8ErrorState;
}
u8 OS_u8ResumeTask(u8 A_u8Priority)
{
	u8 L_u8ErrorState = 0;
	if(System_Tasks[A_u8Priority].State == Suspended)
	{
		System_Tasks[A_u8Priority].State = Running;
	}else
	{
		L_u8ErrorState = NO_TASK_ERROR;
	}
	return L_u8ErrorState;
}
u8 OS_u8DeleteTask(u8 A_u8Priority)
{
	u8 L_u8ErrorState = 0;
	if(System_Tasks[A_u8Priority].Fptr != 0 )
	{
		System_Tasks[A_u8Priority] = (Task){0};
	}else
	{
		L_u8ErrorState = NO_TASK_ERROR;
	}
	return L_u8ErrorState;
}
