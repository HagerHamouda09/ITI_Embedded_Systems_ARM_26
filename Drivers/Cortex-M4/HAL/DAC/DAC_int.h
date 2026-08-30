/*
 * DAC_int.h
 *
 *  Created on: Aug 29, 2026
 *      Author: Hager Adel
 */

#ifndef HAL_DAC_DAC_INT_H_
#define HAL_DAC_DAC_INT_H_

void HDAC_vInit(GPIOx_PinConfig_t *A_xPins, u8 A_u8PinsNo);

void HDAC_vSendSample(const u8* A_u8Ptr, u32 A_u32Index);


#endif /* HAL_DAC_DAC_INT_H_ */
