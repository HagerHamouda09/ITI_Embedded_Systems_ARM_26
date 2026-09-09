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
	//1- wait on busy flag =1
	while(GET_BIT(FMI->SR, BSY));

	//2- if locked CR register -> unlock
	if(GET_BIT(FMI->CR,LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	//3- Set the MER bit in the FLASH_CR
	SET_BIT(FMI->CR, MER);

	//4- Set the STRT bit in the FLASH_CR
	SET_BIT(FMI->CR,STRT);
	
	//5- Wait for the BSY bit to be cleared
	while(GET_BIT(FMI->SR, BSY));

	//6- Turn off mass erase
	CLR_BIT(FMI->CR, MER);
}

void MFMI_vPageErase(u32 A_u32PageAddress)
{
	//1- wait on busy flag =1
	while(GET_BIT(FMI->SR, BSY));

	//2- if locked CR register -> unlock
	if(GET_BIT(FMI->CR,LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}


	/* 3- Enable Page Erase */
	SET_BIT(FMI->CR, PER);

	/* 4- Write the address of the page to FLASH_AR */
	FMI->AR = A_u32PageAddress;

	/* 5- Start erase operation */
	SET_BIT(FMI->CR,STRT);

	/* 6- Wait until erase is finished */
	while(GET_BIT(FMI->SR, BSY));

	/* 7- Disable Page Erase */
	CLR_BIT(FMI->CR, PER);

}

void MFMI_vProgramFlash(u32 A_u32Address, u16* A_PtrData, u16 DataLength)
{
	/* 1- Wait until Flash is not busy */
	while(GET_BIT(FMI->SR, BSY));
	
	/* 2- Unlock Flash if it is locked */
	if(GET_BIT(FMI->CR,LOCK))
	{
		FMI->KEYR = KEY1;
		FMI->KEYR = KEY2;
	}

	/* 3- Enable Flash programming */
	SET_BIT(FMI->CR, PG);

	// arr[2]={1,2}
	// add =10
	 
	/* 4- Program data half-word by half-word */
	for(u16 i=0; i<DataLength; i++)
	{
		// 16 bit = 2byte
		// 0x8008000
		// 0x8008002
		*(volatile u16*)A_u32Address = A_PtrData[i];
		A_u32Address+=2;

		while(GET_BIT(FMI->SR, BSY));

	}
		/* 5- Disable Flash programming */
	CLR_BIT(FMI->CR, PG);

}

