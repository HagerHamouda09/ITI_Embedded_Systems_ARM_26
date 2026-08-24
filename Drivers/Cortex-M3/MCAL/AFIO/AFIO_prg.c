/*
 * AFIO_prg.c
 *
 *  Created on: Aug 22, 2026
 *      Author: Hager Adel
 */

#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "AFIO_int.h"
#include "AFIO_prv.h"

void MAFIO_vSetEXTIPort(u8 A_u8LineNo, u8 A_u8PortNo)
{
    u8 A_u8RegisterNo = A_u8LineNo / 4;
    u8 A_u8ShiftAmount = (A_u8LineNo % 4) * 4;

    AFIO->EXTICR[A_u8RegisterNo] &=
        ~((0b1111) << A_u8ShiftAmount);

    AFIO->EXTICR[A_u8RegisterNo] |=
        (A_u8PortNo) << A_u8ShiftAmount;
}
