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

#include "LEDMATRIX_int.h"
#include "LEDMATRIX_prv.h"
#include "LEDMATRIX_cfg.h"

static void HLEDMATRIX_vEnableCurrentCol(u8 A_u8ColNo);
static void HLEDMATRIX_DisableAllCol();
static void HLEDMATRIX_vSetRowValue(u8 A_u8rowValue);

GPIOx_PinConfig_t *Rows;
GPIOx_PinConfig_t *Cols;

u8 NO_ROWS;
u8 NO_COLS;


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
	// set row value
	HLEDMATRIX_vSetRowValue(A_u8Frame[i]);
	// col enable
	HLEDMATRIX_vEnableCurrentCol(i);
	// calc delay
	MSYSTICK_vSetDelay_ms(SCAN_TIME);
	//disable cols
	HLEDMATRIX_DisableAllCol();
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
