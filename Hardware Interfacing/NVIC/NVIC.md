# (Nested Vectored Interrupt Controller - NVIC)

### **Lab 1:** Software Interrupt Triggering via NVIC Pending Flag

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"

int main(void)
{
    /* 1. Initialize System Clock & Enable GPIOA Clock */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure PA0 as Output Push-Pull */
    GPIOx_PinConfig_t led =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN0,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&led);

    /* Turn OFF LED initially */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);

    /* 3. Manually trigger Pending Flag for EXTI0 (IRQ Pos 6) & Enable Interrupt */
    MNVIC_vSetPendingFlag(6);
    MNVIC_vEnable_Peripheral_INT(6);

    while(1)
    {
        // Super Loop
    }

    return 0;
}

/* ================= Interrupt Service Routine ================= */

void EXTI0_IRQHandler(void)
{
    /* Turn ON LED inside ISR */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
}
