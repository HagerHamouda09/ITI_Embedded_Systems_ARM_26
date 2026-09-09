/*
 * FMI_prg.c
 *
 *  Created on: Sep 9, 2026
 *      Author: Hager Adel
 */


#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "FMI_int.h"
#include "FMI_prv.h"

void MFMI_vMassErase(void)
{
	while(GET_BIT(FMI->SR, BSY));

	if(GET_BIT(FMI->CR,LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	SET_BIT(FMI->CR, MER);

	SET_BIT(FMI->CR,STRT);

	while(GET_BIT(FMI->SR, BSY));

	CLR_BIT(FMI->CR, MER);
}

void MFMI_vSectorErase(Sector_t A_xSectorNo)
{
	while(GET_BIT(FMI->SR, BSY));

	if(GET_BIT(FMI->CR,LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	FMI->CR &=~(0b1111<<SNB);
	FMI->CR |= (A_xSectorNo<<SNB);

	SET_BIT(FMI->CR, SER);

	SET_BIT(FMI->CR,STRT);

	while(GET_BIT(FMI->SR, BSY));

	CLR_BIT(FMI->CR, SER);
}

void MFMI_vProgramFlash(u32 A_u32Address, u16* A_PtrData, u16 DataLength)
{

	while(GET_BIT(FMI->SR, BSY));

	if(GET_BIT(FMI->CR,LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	FMI->CR &=~(0b11<<PSIZE);
	FMI->CR |= (1<<PSIZE);


	SET_BIT(FMI->CR, PG);

	// arr[2]={1,2}
	// add =10
	for(u16 i=0; i<DataLength; i++)
	{
		// 16 bit = 2byte
		// 0x8008000
		// 0x8008002
		*(volatile u16*)A_u32Address = A_PtrData[i];
		A_u32Address+=2;

		while(GET_BIT(FMI->SR, BSY));

	}
	CLR_BIT(FMI->CR, PG);

}
