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
