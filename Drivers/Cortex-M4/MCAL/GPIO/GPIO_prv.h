/*
 * GPIO_prv.h
 *
 *  Created on: Aug 18, 2026
 *      Author: Hager Adel
 */

#ifndef MCAL_GPIO_GPIO_PRV_H_
#define MCAL_GPIO_GPIO_PRV_H_

#define GPIOA_BASE_ADDRESS	0x40020000U
#define GPIOB_BASE_ADDRESS	0x40020400U
#define GPIOC_BASE_ADDRESS	0x40020800U

typedef struct{
u32 MODER;
u32 OTYPER;
u32 OSPEEDR;
u32 PUPDR;
u32 IDR;
u32 ODR;
u32 BSRR;
u32 LCKR ;
u32 AFRL;
u32 AFRH;
}GPIOx_MemMap_t;

#define GPIOA	((volatile GPIOx_MemMap_t*)(GPIOA_BASE_ADDRESS))
#define GPIOB	((volatile GPIOx_MemMap_t*)(GPIOB_BASE_ADDRESS))
#define GPIOC	((volatile GPIOx_MemMap_t*)(GPIOC_BASE_ADDRESS))

#endif /* MCAL_GPIO_GPIO_PRV_H_ */
