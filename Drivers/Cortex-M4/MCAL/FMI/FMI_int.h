/*
 * FMI_int.h
 *
 *  Created on: Sep 9, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_FMI_FMI_INT_H_
#define MCAL_FMI_FMI_INT_H_

typedef enum{
	Sector0=0,
	Sector1=1,
	Sector2=2,
	Sector3=3,
	Sector4=4,
	Sector5=5
}Sector_t;


void MFMI_vMassErase(void);

void MFMI_vSectorErase(Sector_t A_xSectorNo);

void MFMI_vProgramFlash(u32 A_u32Address, u16* A_PtrData, u16 DataLength);


#endif /* MCAL_FMI_FMI_INT_H_ */
