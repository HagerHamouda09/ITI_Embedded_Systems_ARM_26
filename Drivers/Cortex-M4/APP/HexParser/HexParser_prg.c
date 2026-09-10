/*
 * HexParser_prg.c
 *
 *  Created on: Sep 10, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/FMI/FMI_int.h"

#include "HexParser_int.h"

#define G_HIGH_ADDRESS 0x08000000
static u8 AHexParser_ASCII_to_Hex(u8 A_u8ASCII)
{
	u8 L_u8Value =0;

	if(A_u8ASCII >= '0' &&  A_u8ASCII <= '9')
	{
		L_u8Value = A_u8ASCII -'0';
	}
	else
	{
		L_u8Value = A_u8ASCII - 55;
	}

	return L_u8Value;

}


void AHexParser_vParseRecord(const s8* A_s8Record)
{
	u8 L_u8Var = AHexParser_ASCII_to_Hex(A_s8Record[8]);
	switch(L_u8Var)
	{
	case 0:
		AHexParser_vParseData(A_s8Record);
		break;
	case 1:
		// EOF
		break;

	case 4:
		//SET HIGH ADDRESS
		break;

	default:
		break;

	}

}

void AHexParser_vParseData(const s8* A_s8Record)
{

	// 	CHAR COUNT
	//10
	u8 CC_high = AHexParser_ASCII_to_Hex(A_s8Record[1]); //1
	u8 CC_low  = AHexParser_ASCII_to_Hex(A_s8Record[2]); //0

	//16
	//00010000
	u8 CC = (CC_high<<4) | CC_low;

	// LOW ADDRESS
	// Read from 3 to 6
	//          3210
	// LOW ADDR 3456
	// 0x0     1       0        0
	// 0b0000  0b0001  0b0000   0b0000
u8 Address0 = AHexParser_ASCII_to_Hex(A_s8Record[3]); //0x0 -> 0b0000
u8 Address1	= AHexParser_ASCII_to_Hex(A_s8Record[4]); //0x1 -> 0b0001
u8 Address2	= AHexParser_ASCII_to_Hex(A_s8Record[5]);
u8 Address3	= AHexParser_ASCII_to_Hex(A_s8Record[6]);

u16 LowAddress = Address0<<12 | Address1<<8 | Address2<<4 | Address3;

u32 Address = G_HIGH_ADDRESS | LowAddress;

u16 Data[10]={0};

// byte = 8 bit

// 0123
// 0D90
// little endian ->90 ->LOWER MEMLOCATION
//				-> 0D -> HIGHER MEM LOCATION

for(u8 i=0; i< (CC/2); i++)
{
	u8 digit0 = AHexParser_ASCII_to_Hex(A_s8Record[9  + (4*i)]); //9
	u8 digit1 = AHexParser_ASCII_to_Hex(A_s8Record[10 + (4*i)]); //0
	u8 digit2 = AHexParser_ASCII_to_Hex(A_s8Record[11 + (4*i)]); //0
	u8 digit3 = AHexParser_ASCII_to_Hex(A_s8Record[12 + (4*i)]); //D
               //9           //0        //0          //D
	Data[i] = (digit0<<4) | (digit1<<0) | (digit2<<12) | (digit3<<8);

}
MFMI_vProgramFlash(Address, Data,(CC/2));
}
