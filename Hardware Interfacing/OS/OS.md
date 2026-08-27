### **Lab 1:** 

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../OS/OS_int.h"

/* Task Prototypes */
void led0(void);
void led1(void);
void led2(void);

int main(void)
{
    /* 1. Enable System Clock and GPIOA Clock on AHB1 */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure LED Pins on PA0, PA1, PA2 */
    GPIOx_PinConfig_t leds[3] = {
        {GPIO_PORTA, GPIO_PIN0,  GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN1,  GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN2, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL}
    };

    for(u8 i = 0; i < 3; i++)
    {
        MGPIO_vPinInit(&leds[i]);
    }

    /* 3. Create OS Periodic Tasks: (TaskFunc, Periodicity, Priority, FirstDelay) */
    OS_u8CreateTask(led0, 5,  0, 0);
    OS_u8CreateTask(led1, 1,  1, 0);
    OS_u8CreateTask(led2, 10, 2, 0);

    /* 4. Initialize SysTick & Start OS Scheduler */
    OS_vStart();

    while(1)
    {
        // Idle loop
    }

    return 0;
}

/* ================= Task Implementations ================= */

void led0(void)
{
    MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN0);
}

void led1(void)
{
    MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN1);
}

void led2(void)
{
    MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN2);
}
