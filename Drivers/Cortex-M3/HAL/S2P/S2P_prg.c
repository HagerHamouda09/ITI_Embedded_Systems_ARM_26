/*
 * S2P_prg.c
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "S2P_int.h"
#include "S2P_cfg.h"

void HS2P_vInit(S2P_Init_t* A_xInit)
{
	GPIOx_PinConfig_t data={
			.Port = A_xInit->DataPort,
			.Pin = A_xInit-> DataPin,
			.Mode = GPIO_MODE_OUTPUT,
			.OutputType = GPIO_OT_PUSHPULL,
			.Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL
	};

	GPIOx_PinConfig_t latch={
			.Port = A_xInit->LatchCLKPort,
			.Pin = A_xInit-> LatchCLKPin,
			.Mode = GPIO_MODE_OUTPUT,
			.OutputType = GPIO_OT_PUSHPULL,
			.Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL
	};

	GPIOx_PinConfig_t shift={
			.Port = A_xInit->ShiftCLKPort,
			.Pin = A_xInit-> ShiftCLKPin,
			.Mode = GPIO_MODE_OUTPUT,
			.OutputType = GPIO_OT_PUSHPULL,
			.Speed = GPIO_SPEED_LOW,
			.PullType = GPIO_NO_PULL
	};

	MGPIO_vPinInit(&data);
	MGPIO_vPinInit(&latch);
	MGPIO_vPinInit(&shift);

}

static void HS2P_vShiftData(S2P_Init_t* A_xInit)
{
MGPIO_vSetPinValue(A_xInit->ShiftCLKPort, A_xInit->ShiftCLKPin, GPIO_HIGH);
MSYSTICK_vSetDelay_us(1);
MGPIO_vSetPinValue(A_xInit->ShiftCLKPort, A_xInit->ShiftCLKPin, GPIO_LOW);

}

static void HS2P_vLatchData(S2P_Init_t* A_xInit)
{
	MGPIO_vSetPinValue(A_xInit->LatchCLKPort, A_xInit->LatchCLKPin, GPIO_HIGH);
	MSYSTICK_vSetDelay_us(1);
	MGPIO_vSetPinValue(A_xInit->LatchCLKPort, A_xInit->LatchCLKPin, GPIO_LOW);
}

// LSB
void HS2P_vSendData(S2P_Init_t* A_xInit, u32 A_u32Byte)
{
// PUT DATA IN DATA PINS
// SHIFT
//LATCH DATA

//101

for (u8 i=0; i<8 * S2P_NO_OF_SHIFT_REG; i++)
{
	MGPIO_vSetPinValue(A_xInit->DataPort, A_xInit->DataPin, GET_BIT(A_u32Byte,i ));
	HS2P_vShiftData(A_xInit);
}
HS2P_vLatchData(A_xInit);
}
