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
	//SW slave
		SET_BIT(SPI1->CR1 , SSM);
	//Master
		SET_BIT(SPI1->CR1, SSI);
	// data
		CLR_BIT(SPI1->CR1, DFF);
	// MSB
		CLR_BIT(SPI1->CR1,LSBFIRST);

	// SELECT MASTER
		SET_BIT(SPI1->CR1, MSTR);

	//POLARITY
		CLR_BIT(SPI1->CR1,CPOL);
		CLR_BIT(SPI1->CR1,CPHA);

		//ENable spi
		SET_BIT(SPI1->CR1, SPE);
}



u8 MSPI_vTransceive(u8 A_u8Data)
{
	while(!GET_BIT(SPI1->SR, TXE));
	SPI1->DR =A_u8Data;

	while(!GET_BIT(SPI1->SR, RXNE));
	return SPI1->DR;

}
