#  (SysTick Timer - Core Peripheral)

### **Lab 1:** Hardware Blocking Delay Using SysTick Timer
Write an ARM Cortex-M4 (STM32F4) C program to toggle an LED on pin `PA1` every **1 second** ($1000\text{ ms}$) using the integrated **SysTick Timer** as a synchronous hardware delay source.

**Key Configuration Details:**
* **Clock Source:** `CLK_SRC_AHB_8` (AHB clock divided by 8).
* **Interrupt State:** `INT_DISABLE` (Polling-based hardware blocking delay).

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"

int main(void)
{
    /* 1. Enable System Clock and GPIOA Clock on AHB1 */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    //M3
    //	MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);

    /* 2. Configure SysTick: Polling Mode, Prescaler = AHB / 8 */
    MSYSTICK_Config_t STK_cfg = {
        .InterruptEnable = INT_DISABLE,
        .CLK_SRC         = CLK_SRC_AHB_8
    };
    MSYSTICK_vInit(&STK_cfg);

    /* 3. Configure PA1 as Output Push-Pull for LED */
    GPIOx_PinConfig_t led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&led);

    while(1)
    {
        /* LED ON */
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);
        MSYSTICK_vSetDelay_ms(1000);

        /* LED OFF */
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);
        MSYSTICK_vSetDelay_ms(1000);
    }

    return 0;
}

```
