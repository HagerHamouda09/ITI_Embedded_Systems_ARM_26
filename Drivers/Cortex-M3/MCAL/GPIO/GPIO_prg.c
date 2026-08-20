/*
 * GPIO_prg.c
 *
 *  Created on: Aug 17, 2025
 *      Author: Nada Mamdouh
 *      Version: 0.2
 */
#include "../../LIB/STD_TYPES.h"
#include "../../LIB/BIT_MATH.h"

#include "GPIO_int.h"
#include "GPIO_prv.h"
#include "GPIO_cfg.h"

/* Forbidden JTAG/SWD Pins
 * PortA = A13, A14, A15
 * PortB = B4, B3
 * */

/* Helper macros for STM32F1 CRL/CRH indexing */
#define GET_CR_SHIFT(PIN) ((PIN % 8) * 4)

void MGPIO_vSetMode(u8 A_u8PortID, u8 A_u8PinNum, u8 A_u8Mode)
{
    if( (A_u8PortID == GPIO_PORTA)&& (A_u8PinNum == 13|| A_u8PinNum == 14|| A_u8PinNum == 15)) { return; }
    else if ((A_u8PortID == GPIO_PORTB)&& (A_u8PinNum == 3|| A_u8PinNum == 4)){ return; }

    u8 L_u8Shift = GET_CR_SHIFT(A_u8PinNum);
    volatile u32* CR_Reg = NULL;

    switch(A_u8PortID) {
        case GPIO_PORTA: CR_Reg = (A_u8PinNum < 8) ? &(GPIOA->CRL) : &(GPIOA->CRH); break;
        case GPIO_PORTB: CR_Reg = (A_u8PinNum < 8) ? &(GPIOB->CRL) : &(GPIOB->CRH); break;
        case GPIO_PORTC: CR_Reg = (A_u8PinNum < 8) ? &(GPIOC->CRL) : &(GPIOC->CRH); break;
    }

    if(CR_Reg) {
        if(A_u8Mode == GPIO_MODE_INPUT) {
            *CR_Reg &= ~(0b11 << L_u8Shift); // Clear lowest 2 bits of 4-bit block
        } else if (A_u8Mode == GPIO_MODE_OUTPUT) {
            *CR_Reg &= ~(0b11 << L_u8Shift);
            *CR_Reg |= (GPIO_SPEED_2MHZ << L_u8Shift); // Default to 2MHz if not set
        }
    }
}

void MGPIO_vSetOutputType(u8 A_u8PortID, u8 A_u8PinNum, u8 A_u8OutputType)
{
    if( (A_u8PortID == GPIO_PORTA)&& (A_u8PinNum == 13|| A_u8PinNum == 14|| A_u8PinNum == 15)) { return; }
    else if ((A_u8PortID == GPIO_PORTB)&& (A_u8PinNum == 3|| A_u8PinNum == 4)){ return; }

    u8 L_u8Shift = GET_CR_SHIFT(A_u8PinNum);
    volatile u32* CR_Reg = NULL;

    switch(A_u8PortID) {
        case GPIO_PORTA: CR_Reg = (A_u8PinNum < 8) ? &(GPIOA->CRL) : &(GPIOA->CRH); break;
        case GPIO_PORTB: CR_Reg = (A_u8PinNum < 8) ? &(GPIOB->CRL) : &(GPIOB->CRH); break;
        case GPIO_PORTC: CR_Reg = (A_u8PinNum < 8) ? &(GPIOC->CRL) : &(GPIOC->CRH); break;
    }

    if(CR_Reg) {
        if(A_u8OutputType == GPIO_OT_PUSHPULL ) {
            *CR_Reg &= ~(0b0100 << L_u8Shift); // Clear Bit 2 (CNF0)
        } else if(A_u8OutputType == GPIO_OT_OPENDREAIN ) {
            *CR_Reg |= (0b0100 << L_u8Shift);  // Set Bit 2 (CNF0)
        }
    }
}

void MGPIO_vSetOutputSpeed(u8 A_u8PortID, u8 A_u8PinNum, u8 A_u8OutputSpeed)
{
    if( (A_u8PortID == GPIO_PORTA)&& (A_u8PinNum == 13|| A_u8PinNum == 14|| A_u8PinNum == 15)) { return; }
    else if ((A_u8PortID == GPIO_PORTB)&& (A_u8PinNum == 3|| A_u8PinNum == 4)){ return; }

    u8 L_u8Shift = GET_CR_SHIFT(A_u8PinNum);
    volatile u32* CR_Reg = NULL;

    switch(A_u8PortID) {
        case GPIO_PORTA: CR_Reg = (A_u8PinNum < 8) ? &(GPIOA->CRL) : &(GPIOA->CRH); break;
        case GPIO_PORTB: CR_Reg = (A_u8PinNum < 8) ? &(GPIOB->CRL) : &(GPIOB->CRH); break;
        case GPIO_PORTC: CR_Reg = (A_u8PinNum < 8) ? &(GPIOC->CRL) : &(GPIOC->CRH); break;
    }

    if(CR_Reg) {
        *CR_Reg &= ~(0b11 << L_u8Shift);
        *CR_Reg |= (A_u8OutputSpeed << L_u8Shift);
    }
}

void MGPIO_vSetPULLType(u8 A_u8PortID, u8 A_u8PinNum, u8 A_u8PushPull)
{
    if( (A_u8PortID == GPIO_PORTA)&& (A_u8PinNum == 13|| A_u8PinNum == 14|| A_u8PinNum == 15)) { return; }
    else if ((A_u8PortID == GPIO_PORTB)&& (A_u8PinNum == 3|| A_u8PinNum == 4)){ return; }

    u8 L_u8Shift = GET_CR_SHIFT(A_u8PinNum);
    volatile u32* CR_Reg = NULL;

    switch(A_u8PortID) {
        case GPIO_PORTA: CR_Reg = (A_u8PinNum < 8) ? &(GPIOA->CRL) : &(GPIOA->CRH); break;
        case GPIO_PORTB: CR_Reg = (A_u8PinNum < 8) ? &(GPIOB->CRL) : &(GPIOB->CRH); break;
        case GPIO_PORTC: CR_Reg = (A_u8PinNum < 8) ? &(GPIOC->CRL) : &(GPIOC->CRH); break;
    }

    if(CR_Reg) {
        if(A_u8PushPull == GPIO_NO_PULL) {
            *CR_Reg &= ~(0b1100 << L_u8Shift);
            *CR_Reg |=  (0b0100 << L_u8Shift); // Floating
        } else {
            *CR_Reg &= ~(0b1100 << L_u8Shift);
            *CR_Reg |=  (0b1000 << L_u8Shift); // Pull-up/Pull-down

            // F1 sets Pull Up/Down via the ODR register
            if(A_u8PushPull == GPIO_PULL_UP) {
                MGPIO_vSetPinValue(A_u8PortID, A_u8PinNum, GPIO_HIGH);
            } else {
                MGPIO_vSetPinValue(A_u8PortID, A_u8PinNum, GPIO_LOW);
            }
        }
    }
}

void MGPIO_vSetPinValue(u8 A_u8PortID, u8 A_u8PinNum, u8 A_u8Value)
{
    if( (A_u8PortID == GPIO_PORTA)&& (A_u8PinNum == 13|| A_u8PinNum == 14|| A_u8PinNum == 15)) { return; }
    else if ((A_u8PortID == GPIO_PORTB)&& (A_u8PinNum == 3|| A_u8PinNum == 4)){ return; }

    if (A_u8Value == GPIO_HIGH) {
        switch(A_u8PortID) {
            case GPIO_PORTA: SET_BIT(GPIOA -> ODR, A_u8PinNum); break;
            case GPIO_PORTB: SET_BIT(GPIOB -> ODR, A_u8PinNum); break;
            case GPIO_PORTC: SET_BIT(GPIOC -> ODR, A_u8PinNum); break;
        }
    } else if(A_u8Value == GPIO_LOW) {
        switch(A_u8PortID) {
            case GPIO_PORTA: CLR_BIT(GPIOA -> ODR, A_u8PinNum); break;
            case GPIO_PORTB: CLR_BIT(GPIOB -> ODR, A_u8PinNum); break;
            case GPIO_PORTC: CLR_BIT(GPIOC -> ODR, A_u8PinNum); break;
        }
    }
}

u8 MGPIO_u8GetPinValue(u8 A_u8PortID, u8 A_u8PinNum)
{
    u8 L_u8Value = 0;
    if( (A_u8PortID == GPIO_PORTA)&& (A_u8PinNum == 13|| A_u8PinNum == 14|| A_u8PinNum == 15)) { return 0; }
    else if ((A_u8PortID == GPIO_PORTB)&& (A_u8PinNum == 3|| A_u8PinNum == 4)){ return 0; }

    switch(A_u8PortID) {
        case GPIO_PORTA: L_u8Value = GET_BIT(GPIOA -> IDR, A_u8PinNum); break;
        case GPIO_PORTB: L_u8Value = GET_BIT(GPIOB -> IDR, A_u8PinNum); break;
        case GPIO_PORTC: L_u8Value = GET_BIT(GPIOC -> IDR, A_u8PinNum); break;
    }
    return L_u8Value;
}

void MGPIO_vPinInit(GPIOx_PinConfig_t* A_xPinCfg)
{
    if( (A_xPinCfg->Port == GPIO_PORTA)&& (A_xPinCfg->Pin == 13|| A_xPinCfg->Pin == 14|| A_xPinCfg->Pin == 15)) return;
    if( (A_xPinCfg->Port == GPIO_PORTB)&& (A_xPinCfg->Pin == 3|| A_xPinCfg->Pin == 4)) return;

    u8 L_u8Shift = GET_CR_SHIFT(A_xPinCfg->Pin);
    u8 L_u8Config = 0; // The 4-bit value to load

    // 1. Construct the 4-bit configuration
    if(A_xPinCfg->Mode == GPIO_MODE_INPUT || A_xPinCfg->Mode == GPIO_MODE_ANALOG)
    {
        if(A_xPinCfg->Mode == GPIO_MODE_ANALOG){
            L_u8Config = 0b0000;
        } else if(A_xPinCfg->PullType == GPIO_NO_PULL) {
            L_u8Config = 0b0100; // Floating
        } else {
            L_u8Config = 0b1000; // Pull-up/down
            // ODR must be set to determine Up vs Down in F1
            if(A_xPinCfg->PullType == GPIO_PULL_UP)
                MGPIO_vSetPinValue(A_xPinCfg->Port, A_xPinCfg->Pin, GPIO_HIGH);
            else
                MGPIO_vSetPinValue(A_xPinCfg->Port, A_xPinCfg->Pin, GPIO_LOW);
        }
    }
    else // Output or Alternate Function
    {
        u8 L_u8Speed = A_xPinCfg->Speed;
        if(L_u8Speed == 0) L_u8Speed = GPIO_SPEED_2MHZ; // Safe default

        if(A_xPinCfg->Mode == GPIO_MODE_OUTPUT) {
            L_u8Config = (A_xPinCfg->OutputType == GPIO_OT_OPENDREAIN) ? (0b0100 | L_u8Speed) : (0b0000 | L_u8Speed);
        } else if(A_xPinCfg->Mode == GPIO_MODE_ALF) {
            L_u8Config = (A_xPinCfg->OutputType == GPIO_OT_OPENDREAIN) ? (0b1100 | L_u8Speed) : (0b1000 | L_u8Speed);
        }
    }

    // 2. Apply to Register
    volatile u32* CR_Reg = NULL;
    switch(A_xPinCfg->Port)
    {
        case GPIO_PORTA: CR_Reg = (A_xPinCfg->Pin < 8) ? &(GPIOA->CRL) : &(GPIOA->CRH); break;
        case GPIO_PORTB: CR_Reg = (A_xPinCfg->Pin < 8) ? &(GPIOB->CRL) : &(GPIOB->CRH); break;
        case GPIO_PORTC: CR_Reg = (A_xPinCfg->Pin < 8) ? &(GPIOC->CRL) : &(GPIOC->CRH); break;
    }

    if(CR_Reg) {
        *CR_Reg &= ~(0b1111 << L_u8Shift);      // Clear the 4 bits
        *CR_Reg |=  (L_u8Config << L_u8Shift);  // Set the newly constructed 4 bits
    }
}

void MGPIO_TogPinValue(u8 A_u8PortID, u8 A_u8PinNum)
{
    switch(A_u8PortID) {
        case GPIO_PORTA: TOG_BIT(GPIOA -> ODR, A_u8PinNum); break;
        case GPIO_PORTB: TOG_BIT(GPIOB -> ODR, A_u8PinNum); break;
        case GPIO_PORTC: TOG_BIT(GPIOC -> ODR, A_u8PinNum); break;
    }
}
