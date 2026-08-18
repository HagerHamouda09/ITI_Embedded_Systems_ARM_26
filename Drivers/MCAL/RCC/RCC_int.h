/*
 * RCC_int.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Hager Adel
 */

#ifndef RCC_INT_H_
#define RCC_INT_H_



void MRCC_vInit(void);

void MRCC_vEnableCLK(u8 A_u8BusID, u8 A_u8PeripheralID);

void MRCC_vDisableCLK(u8 A_u8BusID, u8 A_u8PeripheralID);


#endif /* RCC_INT_H_ */
