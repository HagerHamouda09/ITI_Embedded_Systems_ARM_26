/*
 * AFIO_int.h
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_AFIO_AFIO_INT_H_
#define MCAL_AFIO_AFIO_INT_H_

#define AFIO_PORTA	0b0000
#define AFIO_PORTB	0b0001
#define AFIO_PORTC	0b0010

void MAFIO_vSetEXTIPort(u8 A_u8LineNo, u8 A_u8PortNo);


#endif /* MCAL_AFIO_AFIO_INT_H_ */
