/*
 * RCC_cfg.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.1
 */

#ifndef MCAL_RCC_RCC_CFG_H_
#define MCAL_RCC_RCC_CFG_H_

/*RCC_SYS_CLK
 * options:
 * HSE_CLK
 * HSI_CLK
 * PLL_CLK
 *
 * */

#define RCC_SYS_CLK		HSI_CLK
/*HSE_BYPASS
 * options:
 * RC_CLK
 * MECHANICAL_CLK
 *
 * */
#define HSE_BYPASS		MECHANICAL_CLK

#endif /* MCAL_RCC_RCC_CFG_H_ */
