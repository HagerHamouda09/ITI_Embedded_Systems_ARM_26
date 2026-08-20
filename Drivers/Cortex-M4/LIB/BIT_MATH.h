/*
 * BIT_MATH.h
 *
 *  Created on: Aug 16, 2026
 *      Author: Hager Adel
 */

#ifndef BIT_MATH_H_
#define BIT_MATH_H_



// #define No_of_bits 8


#define SET_BIT(reg,bitN)       	((reg) |=  (1U << (bitN) ))

#define CLR_BIT(reg,bitN)       	((reg) &= (~(1U << (bitN) )))

#define TOG_BIT(reg,bitN)           ((reg) ^=  (1U << (bitN) ))

#define GET_BIT(reg,bitN)           (((reg) >> (bitN) ) & (1U))

#define SET_BYTE(reg,value)         ( (reg) = (value) )

// #define ROT_LEFT(reg,bitN)          ((unsigned char)((reg<<bitN)|(reg>>(No_of_bits - bitN))))

// #define ROT_RIGHT(reg,bitN)         ((unsigned char)((reg>>bitN)|(reg<<(No_of_bits - bitN))))




#endif /* BIT_MATH_H_ */
