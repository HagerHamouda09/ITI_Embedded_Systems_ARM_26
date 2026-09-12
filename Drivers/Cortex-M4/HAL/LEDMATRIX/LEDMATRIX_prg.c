/*
 * LEDMATRIX_prg.c
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "../S2P/S2P_int.h"

#include "LEDMATRIX_int.h"
#include "LEDMATRIX_prv.h"
#include "LEDMATRIX_cfg.h"


static void HLEDMATRIX_vEnableCurrentCol(u8 A_u8ColNo);
static void HLEDMATRIX_DisableAllCol();
static void HLEDMATRIX_vSetRowValue(u8 A_u8rowValue);

GPIOx_PinConfig_t *Rows;
GPIOx_PinConfig_t *Cols;

S2P_Init_t *S2P;

u8 NO_ROWS;
u8 NO_COLS;

void HLEDMATRIX_vInit_S2P(S2P_Init_t* A_xInit)
{
	S2P = A_xInit;

	HS2P_vInit(A_xInit);

	/* Initialize Systick */
	MSYSTIC_Config_t STK_cfg = {.InterruptEnable= INT_DISABLE ,.CLK_SRC = CLK_SRC_AHB_8};
	MSYSTICK_vInit(& STK_cfg);

}

void HLEDMATRIX_vInit(GPIOx_PinConfig_t *A_xRows, u8 A_u8RowsNo ,GPIOx_PinConfig_t *A_xCols, u8 A_u8ColNo)
{
	Rows = A_xRows;
	Cols= A_xCols;
	NO_ROWS =A_u8RowsNo;
	NO_COLS = A_u8ColNo;

	for(u8 i=0; i<A_u8RowsNo; i++)
	{
		MGPIO_vPinInit(&A_xRows[i]);
	}


	for(u8 i=0; i<A_u8ColNo; i++)
	{
		MGPIO_vPinInit(&A_xCols[i]);
	}

	MSYSTIC_Config_t STK_cfg= {
			.InterruptEnable= INT_DISABLE,
			.CLK_SRC= CLK_SRC_AHB_8
	};

	MSYSTICK_vInit(&STK_cfg);
}

void HLEDMATRIX_vDisplayFrame(u8 A_u8Frame[], u32 A_u32FrameDelay)
{

	for(u8 j=0 ; j<A_u32FrameDelay; j++)
	{

	for(u8 i=0 ; i<NO_COLS; i++)
	{
#if METHOD == DIRECT_LEDMATRIX
			/* Set Row Value */
			HLEDMATRIX_SetRowValue(A_u8Frame[i]);

			/* Enable Current Col */
			HLEDMATRIX_vEnableCurrentCol(i);

			/* Calculated delay */
			MSYSTICK_vSetDelay_ms(SCAN_TIME);

			/* Disable All Cols */
			HLEDMATRIX_vDisableAllCol();
#endif

#if METHOD == S2P_CONNECTION
//	u8 arr[8] = {0x00, 0x02, 0x03, 0xB1, 0xB9, 0x0F, 0x06, 0x00};
// frame[0] = 0x00
			u8 RowData = A_u8Frame[i];

			//11111111
			        u8 ColData = 0xFF;
			// i = 0
			//11111110
			        CLR_BIT(ColData, i);

			        u32 S2P_Data = ((u32)RowData << 8) | ColData;

			        /* Disable outputs */
			        MGPIO_vSetPinValue(S2P->OEPort, S2P->OEPin, GPIO_HIGH);

			        /* Shift + latch */
			        HS2P_vSendData(S2P, S2P_Data);

			        /* Enable outputs */
			        MGPIO_vSetPinValue(S2P->OEPort, S2P->OEPin, GPIO_LOW);

			        /* Display current column */
			        MSYSTICK_vSetDelay_ms(SCAN_TIME);
#endif
	}
	}
}


static void HLEDMATRIX_vEnableCurrentCol(u8 A_u8ColNo)
{
	MGPIO_vSetPinValue( Cols[A_u8ColNo].Port,Cols[A_u8ColNo].Pin, GPIO_LOW);

}
static void HLEDMATRIX_DisableAllCol()
{
	for(u8 i=0; i<NO_COLS ; i++)
	{
		MGPIO_vSetPinValue( Cols[i].Port,Cols[i].Pin, GPIO_HIGH);

	}
}

static void HLEDMATRIX_vSetRowValue(u8 A_u8rowValue)
{
	for(u8 i=0; i<NO_ROWS; i++)
	{
		MGPIO_vSetPinValue( Rows[i].Port,Rows[i].Pin, GET_BIT(A_u8rowValue,i));
	}
}
