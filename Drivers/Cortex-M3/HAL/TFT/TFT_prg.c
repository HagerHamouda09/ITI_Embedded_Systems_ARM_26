/*
 * TFT_prg.c
 *
 *  Created on: Sep 1, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "../../MCAL/GPIO/GPIO_int.h"
#include "../../MCAL/SPI/SPI_int.h"
#include "../../MCAL/SYSTICK/SYSTICK_int.h"

#include "TFT_int.h"

static u16 CurrentXStart;
static u16 CurrentXEnd;

static u16 CurrentYStart;
static u16 CurrentYEnd;

GPIOx_PinConfig_t TFT_RST_PIN={
		.Port = GPIO_PORTA,
		.Pin = GPIO_PIN0,
		.Mode = GPIO_MODE_OUTPUT,
		.Speed = GPIO_SPEED_HIGH,
		.OutputType = GPIO_OT_PUSHPULL
};

// SEND DATA 1-> HIGH
// SEND CMD 0 -> LOW
GPIOx_PinConfig_t TFT_A0_PIN={
		.Port = GPIO_PORTA,
		.Pin = GPIO_PIN1,
		.Mode = GPIO_MODE_OUTPUT,
		.Speed = GPIO_SPEED_HIGH,
		.OutputType = GPIO_OT_PUSHPULL
};

static void Reset_Seq(void)
{
	// RST PIN = 1
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	//DELAY 100usec
	MSYSTICK_vSetDelay_us(100);
	//RST PIN 0
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_LOW);
	// delay 1usec
	MSYSTICK_vSetDelay_us(1);
	// Pin high
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	// delay 100usec
	MSYSTICK_vSetDelay_us(100);
	// pin low
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_LOW);
	// delay 100usec
	MSYSTICK_vSetDelay_us(100);
	// pin high
	MGPIO_vSetPinValue(TFT_RST_PIN.Port, TFT_RST_PIN.Pin, GPIO_HIGH);
	// delay 120 msec
	MSYSTICK_vSetDelay_ms(120);

}

static void Write_cmd(u8 A_u8cmd)
{
	MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_LOW);
	MSPI_u8Transceive(A_u8cmd);
}

static void Write_data(u8 A_u8data)
{
	MGPIO_vSetPinValue(TFT_A0_PIN.Port, TFT_A0_PIN.Pin, GPIO_HIGH);
	MSPI_u8Transceive(A_u8data);
}

void HTFT_vInit(void)
{
	//PIN CONFIG
	MGPIO_vPinInit(&TFT_RST_PIN);
	MGPIO_vPinInit(&TFT_A0_PIN);

	//SPI
	MSPI_vInit();

	//SYSTICK
	MSYSTIC_Config_t STK_CFG = {
			.InterruptEnable = INT_DISABLE,
			.CLK_SRC = CLK_SRC_AHB_8
	};

	MSYSTICK_vInit(&STK_CFG);
	//RST
	Reset_Seq();

	//SLEEP OUT (CMD)
	Write_cmd(0x11);

	//WAIT 15 msec
	MSYSTICK_vSetDelay_ms(15);

	//Select mode 3A
	Write_cmd(0x3A);
	// mode is 565->0x05
	Write_data(0x05);

	//DISPLAY ON
	Write_cmd(0x29);

}

void HTFT_vShowImage(const u16 A_u16ImgArr[], u16 A_u16ImgSize)
{
	u8 MSB=0;
	u8 LSB=0;
	//set x position
	Write_cmd(0x2A);

	// 0x0000

	//1 send xstart
	//msb
	Write_data(0);
	//least
	Write_data(0);

	// send xEnd
	// 127 = 0x00 7F
	Write_data(0);
	Write_data(127);

	//set y position
	Write_cmd(0x2B);

	// 0x0000

	//1 send ystart
	//msb
	Write_data(0);
	//least
	Write_data(0);

	// send xEnd
	// 159
	Write_data(0);
	Write_data(159);

	//send image
	Write_cmd(0x2C);


	for(u16 i=0; i<A_u16ImgSize ;i++)
	{
		//      0x 1234
		MSB = (A_u16ImgArr[i] & 0xFF00) >>8;
		LSB = A_u16ImgArr[i] & 0x00FF;

		Write_data(MSB);
		Write_data(LSB);

	}


}

void HTFT_vSetXPos(u16 A_u16xStart,u16 A_u16xEnd)
{
	CurrentXStart = A_u16xStart;
	CurrentXEnd = A_u16xEnd;
	//set x position
	Write_cmd(0x2A);

	// 0x0000

	//1 send xstart
	//msb
	Write_data(0);
	//least
	Write_data(A_u16xStart);

	// send xEnd
	// 127 = 0x00 7F
	Write_data(0);
	Write_data(A_u16xEnd);

}
void HTFT_vSetYPos(u16 A_u16YStart,u16 A_u16YEnd)
{

	CurrentYStart = A_u16YStart;
	CurrentYEnd = A_u16YEnd;

	//set y position
	Write_cmd(0x2B);

	// 0x0000

	//1 send ystart
	//msb
	Write_data(0);
	//least
	Write_data(A_u16YStart);

	// send xEnd
	// 159
	Write_data(0);
	Write_data(A_u16YEnd);
}

void HTFT_vFillBackgroundColor(u16 A_u16Color)
{
	HTFT_vSetXPos(0,127);
	HTFT_vSetYPos(0,159);

	HTFT_vFillRectangle(A_u16Color);
}

//0X1234
void HTFT_vFillRectangle(u16 A_u16Color)
{
	u8 MSB =0;
	u8 LSB = 0;
u16 Pixels = (CurrentXEnd-CurrentXStart+1) * (CurrentYEnd - CurrentYStart+1);

Write_cmd(0x2C);

MSB = (A_u16Color & 0xFF00) >>8;
		LSB = A_u16Color & 0x00FF;

for(u16 i=0; i<Pixels ;i++)
{
		Write_data(MSB);
		Write_data(LSB);
}

}
