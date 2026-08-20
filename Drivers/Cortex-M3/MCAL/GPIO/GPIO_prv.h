/*
 * GPIO_prv.h
 *
 *  Created on: Aug 17, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.2
 */

#ifndef MCAL_GPIO_GPIO_PRV_H_
#define MCAL_GPIO_GPIO_PRV_H_

#define GPIOA_BASE_ADDR		0x40010800U
#define GPIOB_BASE_ADDR		0x40010C00U
#define GPIOC_BASE_ADDR		0x40011000U

typedef struct{
    u32 CRL;    // Port configuration register low (Pins 0-7)
    u32 CRH;    // Port configuration register high (Pins 8-15)
    u32 IDR;    // Port input data register
    u32 ODR;    // Port output data register
    u32 BSRR;   // Port bit set/reset register
    u32 BRR;    // Port bit reset register
    u32 LCKR;   // Port configuration lock register

}GPIOx_MemMap_t;

#define GPIOA		((volatile GPIOx_MemMap_t*)(GPIOA_BASE_ADDR))
#define GPIOB		((volatile GPIOx_MemMap_t*)(GPIOB_BASE_ADDR))
#define GPIOC		((volatile GPIOx_MemMap_t*)(GPIOC_BASE_ADDR))


#endif /* MCAL_GPIO_GPIO_PRV_H_ */
