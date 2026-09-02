/*
 * SPI_prg.c
 *
 *  Created on: Sep 1, 2026
 *      Author: Hager Adel
 */



#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "SPI_int.h"
#include "SPI_prv.h"


void MSPI_vInit(void)
{
	//SSM and SSI
	SET_BIT(SPI1->CR1, SSM);
	SET_BIT(SPI1->CR1, SSI);

	//data frame format
	CLR_BIT(SPI1->CR1, DFF);

	// select MSB
	CLR_BIT(SPI1->CR1, LSBFIRST);

	//  Select Master
	SET_BIT(SPI1->CR1, MSTR);

	//  Select Mode
	CLR_BIT(SPI1->CR1, CPOL);
	CLR_BIT(SPI1->CR1, CPHA);

	// Enable SPI
	SET_BIT(SPI1->CR1, SPE);
}
u8 MSPI_u8Transceive(u8 A_u8Data)
{
	// wait till transmit buffer is empty
	while(GET_BIT(SPI1->SR, TXE) == 0)
		;
	SPI1->DR = A_u8Data;

	// wait till receiving data
	while(GET_BIT(SPI1->SR, RXNE) == 0)
		;
	return SPI1->DR;
}
