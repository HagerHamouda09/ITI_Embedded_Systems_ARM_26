#  (USART Peripheral: Cortex-M4 vs Cortex-M3)

### **Lab 1:** Periodic Character Sequence Transmission on STM32F4 (Cortex-M4)
Write an ARM Cortex-M4 (STM32F4) C program to initialize **USART1** and transmit characters (`'A'`, `'B'`, `'C'`) periodically every **500 ms** using the SysTick timer for blocking delays.

**Key Architectural Points:**
* **Bus Architecture:** `GPIOA` clock is on the **AHB1** bus; `USART1` peripheral clock is enabled on **APB2** (Bit 4).
* **Alternate Function Routing:** Pin `PA9` (TX) and Pin `PA10` (RX) must be configured in Alternate Function mode (`GPIO_MODE_ALF`) with Alternate Function 7 (`GPIO_AF7` for USART1/USART2).

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../MCAL/USART/USART_int.h"

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_APB2, 4);

    MSYSTICK_Config_t STK_cfg = {
        .InterruptEnable = INT_DISABLE,
        .CLK_SRC         = CLK_SRC_AHB_8
    };
    MSYSTICK_vInit(&STK_cfg);

    GPIOx_PinConfig_t tx = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN9,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF7
    };
    GPIOx_PinConfig_t rx = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN10,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF7
    };
    MGPIO_vPinInit(&tx);
    MGPIO_vPinInit(&rx);

    MUSART_vInit();

    while(1)
    {
        /* Transmit 'A' */
        MUSART_vSendData('A');
        MSYSTICK_vSetDelay_ms(500);

        /* Transmit 'B' */
        MUSART_vSendData('B');
        MSYSTICK_vSetDelay_ms(500);

        /* Transmit 'C' */
        MUSART_vSendData('C');
        MSYSTICK_vSetDelay_ms(500);
    }

    return 0;
}

```

---

### **Lab 2:** Periodic Character Sequence Transmission on STM32F103 (Cortex-M3)

Write an ARM Cortex-M3 (STM32F103) C program to initialize **USART1** and transmit characters (`'A'`, `'B'`, `'C'`) periodically every **500 ms**.

**Key Architectural Points:**

* **Bus Architecture:** Both `GPIOA` and `USART1` peripheral clocks reside entirely on the **APB2** bus (`USART1` enabled on Bit 14 of `RCC_APB2ENR`).
* **GPIO Mode Definitions (F1 Style):**
* `PA9` (TX): Alternate Function Push-Pull (`GPIO_MODE_ALF`, `GPIO_OT_PUSHPULL`).
* `PA10` (RX): Input Floating / Input with No Pull (`GPIO_MODE_INPUT`, `GPIO_NO_PULL`).



**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../MCAL/USART/USART_int.h"

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);
    MRCC_vEnableClk(RCC_APB2, 14); // Enable USART1 Clock (Bit 14)

    GPIOx_PinConfig_t tx = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN9,
        .Mode       = GPIO_MODE_ALF,
        .Speed      = GPIO_SPEED_2MHZ,
        .OutputType = GPIO_OT_PUSHPULL
    };
    MGPIO_vPinInit(&tx);

    GPIOx_PinConfig_t rx = {
        .Port     = GPIO_PORTA,
        .Pin      = GPIO_PIN10,
        .Mode     = GPIO_MODE_INPUT,
        .PullType = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&rx);

    MUSART_vInit();

    while(1)
    {
        /* Transmit 'A' */
        MUSART_vSendData('A');
        MSYSTICK_vSetDelay_ms(500);

        /* Transmit 'B' */
        MUSART_vSendData('B');
        MSYSTICK_vSetDelay_ms(500);

        /* Transmit 'C' */
        MUSART_vSendData('C');
        MSYSTICK_vSetDelay_ms(500);
    }

    return 0;
}

```
