# GPIO Labs (STM32 RCC & GPIO Driver)

### **Lab 1:** 

<img width="392" height="72" alt="image" src="https://github.com/user-attachments/assets/0ece8458-5e99-4897-adc4-f54674b721b9" />

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

int main(void)
{
    /* 1. Initialize System Clock */
    MRCC_vInit();

    /* 2. Enable GPIOA Clock on AHB1 Bus */
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 3. Configure PA0 as Output Push-Pull */
    GPIOx_PinConfig_t LED =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN0,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&LED);

    /* 4. Drive PA0 HIGH to Turn ON LED */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);

    while(1)
    {
        // Super Loop
    }
}
