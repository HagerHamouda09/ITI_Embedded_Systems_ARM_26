/*
 * OS_prv.h
 *
 *  Created on: Aug 24, 2026
 *      Author: Hager Adel
 */

#ifndef OS_OS_PRV_H_
#define OS_OS_PRV_H_


typedef enum
{
	Running = 1,
	Suspended,
}TaskState;

typedef struct
{
	void (*Fptr)(void);
	u32 Periodicity;
//	u8 Priority;						//
	u32 FirstDelay;
	TaskState State;
}Task;

#define OUT_OF_RANGE_ERROR		1
#define RESERVED_PRIORITY_ERROR	2
#define NO_TASK_ERROR			3




#endif /* OS_OS_PRV_H_ */
