# (External Interrupts - EXTI)

# ON Default PA0

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/EXTI/EXTI_int.h"

void TogLed(void);

int main(void)
{
    /* 1. Enable Peripheral Clocks */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure GPIO Pins */
    GPIOx_PinConfig_t led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t button =
    {
        .Port     = GPIO_PORTA,
        .Pin      = GPIO_PIN0,
        .Mode     = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };

    MGPIO_vPinInit(&led);
    MGPIO_vPinInit(&button);

    /*  Configure External Interrupt (EXTI Line 0) */
    MEXTI_vInit();
    MEXTI_vEnableINT(EXTI_LINE0);
    MEXTI_vSetTrigger(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(TogLed, EXTI_LINE0);

    /*  Enable EXTI0 Interrupt Channel in NVIC (Position 6) */
    MNVIC_vEnable_Peripheral_INT(6);

    while(1)
    {
        // Super Loop
    }

    return 0;
}

/* ================= EXTI Callback Function ================= */

void TogLed(void)
{
    /* Toggle LED on PA1 */
    MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN1);
}
```
---

# (EXTI : Cortex-M4 SYSCFG vs Cortex-M3 AFIO)

### **Lab 1:** External Interrupt on STM32F4 (Cortex-M4 via SYSCFG)
Write an ARM Cortex-M4 (STM32F4) C program to toggle an LED on `PA1` when an external interrupt is triggered by a push button on `PB0` (Active LOW with internal pull-up).

**Key Architectural Points:**
* **Bus Architecture:** GPIO peripherals reside on the **AHB1** bus; the system configuration controller (**SYSCFG**) resides on **APB2** (Bit 14).
* **Line Routing:** EXTI line multiplexing is handled through the **SYSCFG** peripheral (`MSYSCFG_vSetEXTIPort`).

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/SYSCFG/SYSCFG_int.h"

void TogLed(void);

int main(void)
{
    /* 1. Enable Clocks (AHB1 for GPIO, APB2 Bit 14 for SYSCFG) */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOB);
    MRCC_vEnableCLK(RCC_APB2, 14);

    /* 2. Configure Pins */
    GPIOx_PinConfig_t led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t button =
    {
        .Port     = GPIO_PORTB,
        .Pin      = GPIO_PIN0,
        .Mode     = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };

    MGPIO_vPinInit(&led);
    MGPIO_vPinInit(&button);

    /* 3. Route PB0 to EXTI Line 0 via SYSCFG */
    MSYSCFG_vSetEXTIPort(EXTI_LINE0, SYSCFG_PORTB);

    /* 4. Configure EXTI Line 0 */
    MEXTI_vInit();
    MEXTI_vEnableINT(EXTI_LINE0);
    MEXTI_vSetTrigger(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(TogLed, EXTI_LINE0);

    /* 5. Enable EXTI0 in NVIC (Position 6) */
    MNVIC_vEnable_Peripheral_INT(6);

    while(1)
    {
        // Super Loop
    }

    return 0;
}

void TogLed(void)
{
    MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN1);
}

```

---

### **Lab 2:** External Interrupt on STM32F1 (Cortex-M3 via AFIO)

Write an ARM Cortex-M3 (STM32F103) C program to toggle an LED on `PA1` when an external interrupt is triggered by a push button on `PB0` (Active LOW with internal pull-up).

**Key Architectural Points:**

* **Bus Architecture:** GPIO peripherals and the Alternate Function I/O controller (**AFIO**) reside on the **APB2** bus (AFIO is enabled on Bit 0 of `RCC_APB2ENR`).
* **Line Routing:** EXTI line multiplexing is handled through the **AFIO** peripheral registers (`MAFIO_vSetEXTIPort`).

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/AFIO/AFIO_int.h"

void TogLed(void);

int main(void)
{
    /* 1. Enable Clocks on APB2 Bus (GPIOA, GPIOB, and AFIO on Bit 0) */
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOB);
    MRCC_vEnableClk(RCC_APB2, 0); // Enable AFIO Clock

    /* 2. Configure Pins */
    GPIOx_PinConfig_t led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t button =
    {
        .Port     = GPIO_PORTB,
        .Pin      = GPIO_PIN0,
        .Mode     = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };

    MGPIO_vPinInit(&led);
    MGPIO_vPinInit(&button);

    /* 3. Route PB0 to EXTI Line 0 via AFIO */
    MAFIO_vSetEXTIPort(EXTI_LINE0, AFIO_PORTB);

    /* 4. Configure EXTI Line 0 */
    MEXTI_vInit();
    MEXTI_vEnableINT(EXTI_LINE0);
    MEXTI_vSetTrigger(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(TogLed, EXTI_LINE0);

    /* 5. Enable EXTI0 in NVIC (Position 6) */
    MNVIC_vEnable_Peripheral_INT(6);

    while(1)
    {
        // Super Loop
    }

    return 0;
}

void TogLed(void)
{
    MGPIO_TogPinValue(GPIO_PORTA, GPIO_PIN1);
}

```
