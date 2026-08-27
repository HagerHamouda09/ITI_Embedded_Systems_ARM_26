# Labs (FreeRTOS on STM32 ARM Cortex-M)

### **Lab 1:** Concurrent Independent LED Tasks with FreeRTOS
Write an ARM Cortex-M4 (STM32F4) C program using **FreeRTOS** to create and schedule two independent concurrent tasks that blink LEDs with different periodicities.

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"

/* Task Prototypes */
void Task_LED1(void *A_Pv);
void Task_LED2(void *A_Pv);

int main(void)
{
    /* 1. Enable System Clock & GPIOA Peripheral Clock on AHB1 */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure PA0 and PA1 as Push-Pull Outputs */
    GPIOx_PinConfig_t leds[2] = {
        {
            .Port       = GPIO_PORTA,
            .Pin        = GPIO_PIN0,
            .Mode       = GPIO_MODE_OUTPUT,
            .OutputType = GPIO_OT_PUSHPULL,
            .Speed      = GPIO_SPEED_LOW,
            .PullType   = GPIO_NO_PULL
        },
        {
            .Port       = GPIO_PORTA,
            .Pin        = GPIO_PIN1,
            .Mode       = GPIO_MODE_OUTPUT,
            .OutputType = GPIO_OT_PUSHPULL,
            .Speed      = GPIO_SPEED_LOW,
            .PullType   = GPIO_NO_PULL
        }
    };

    for(u8 i = 0; i < 2; i++)
    {
        MGPIO_vPinInit(&leds[i]);
    }

    /* 3. Create FreeRTOS Tasks */
    xTaskCreate(Task_LED1, (const char * const)"LED1", 128, NULL, 1, NULL);
    xTaskCreate(Task_LED2, (const char * const)"LED2", 128, NULL, 3, NULL);

    /* 4. Start FreeRTOS Scheduler */
    vTaskStartScheduler();

    while(1)
    {
        // Idle loop 
    }

    return 0;
}

/* ================= FreeRTOS Task Implementations ================= */

void Task_LED1(void *A_Pv)
{
    while(1)
    {
        MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN0);
        vTaskDelay(200);
    }
}

void Task_LED2(void *A_Pv)
{
    while(1)
    {
        MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN1);
        vTaskDelay(1000);
    }
}

```
