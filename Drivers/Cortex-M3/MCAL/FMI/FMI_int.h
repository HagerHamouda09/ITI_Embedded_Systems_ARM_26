/*
 * FMI_int.h
 *
 *  Created on: Sep 9, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_FMI_FMI_INT_H_
#define MCAL_FMI_FMI_INT_H_


void MFMI_vMassErase(void);

void MFMI_vPageErase(u32 A_u32PageAddress);

void MFMI_vProgramFlash(u32 A_u32Address, u16* A_PtrData, u16 DataLength);


#endif /* MCAL_FMI_FMI_INT_H_ */
