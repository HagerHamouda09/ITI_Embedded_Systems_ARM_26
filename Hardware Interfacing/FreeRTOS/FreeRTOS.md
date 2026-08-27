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
---

### **Lab 2:** Task Concurrency & Preemption (Equal Priority Blinking)

# 2 Tasks with same priority on RGB LED

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"

/* Task Prototypes */
void Task_LED_Red(void *A_Pv);
void Task_LED_Blue(void *A_Pv);

int main(void)
{
    /* 1. Enable Clocks */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure PA0 (Red) and PA1 (Blue) as Output Push-Pull */
    GPIOx_PinConfig_t leds[2] = {
        {GPIO_PORTA, GPIO_PIN0, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN1, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL}
    };

    for(u8 i = 0; i < 2; i++)
    {
        MGPIO_vPinInit(&leds[i]);
    }

    /* 3. Create FreeRTOS Tasks with Equal Priority */
    xTaskCreate(Task_LED_Red,  (const char * const)"Red",  128, NULL, 4, NULL);
    xTaskCreate(Task_LED_Blue, (const char * const)"Blue", 128, NULL, 4, NULL);

    /* 4. Start Scheduler */
    vTaskStartScheduler();

    while(1)
    {
        // Idle loop
    }

    return 0;
}

void Task_LED_Red(void *A_Pv)
{
    while(1)
    {
        MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN0);
        vTaskDelay(500);
    }
}

void Task_LED_Blue(void *A_Pv)
{
    while(1)
    {
        MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN1);
        vTaskDelay(500);
    }
}

```

---

### **Lab 2-2:** Repeat Lab2 with Binary Semaphore

**Alternation Pattern:**

1. Red task takes `Red_Sem` $\rightarrow$ Turns Red LED ON, Blue LED OFF for $500\text{ ms}$ $\rightarrow$ Gives `Blue_Sem`.
2. Blue task takes `Blue_Sem` $\rightarrow$ Turns Blue LED ON, Red LED OFF for $500\text{ ms}$ $\rightarrow$ Gives `Red_Sem`.

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/semphr.h"

SemaphoreHandle_t Red_Sem;
SemaphoreHandle_t Blue_Sem;

/* Task Prototypes */
void Task_LED_Red(void *A_Pv);
void Task_LED_Blue(void *A_Pv);

int main(void)
{
    /* 1. Enable Clocks */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure Pins */
    GPIOx_PinConfig_t leds[2] = {
        {GPIO_PORTA, GPIO_PIN0, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN1, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL}
    };

    for(u8 i = 0; i < 2; i++)
    {
        MGPIO_vPinInit(&leds[i]);
    }

    /* 3. Create Binary Semaphores */
    vSemaphoreCreateBinary(Red_Sem);
    vSemaphoreCreateBinary(Blue_Sem);

    if(Red_Sem != NULL && Blue_Sem != NULL)
    {
        /* Start with Red's turn */
        xSemaphoreGive(Red_Sem);

        /* Create Tasks */
        xTaskCreate(Task_LED_Red,  (const char * const)"Red",  128, NULL, 2, NULL);
        xTaskCreate(Task_LED_Blue, (const char * const)"Blue", 128, NULL, 2, NULL);

        /* Start Scheduler */
        vTaskStartScheduler();
    }

    while(1)
    {
        // Idle loop
    }

    return 0;
}

void Task_LED_Red(void *A_Pv)
{
    while(1)
    {
        BaseType_t state = xSemaphoreTake(Red_Sem, portMAX_DELAY);
        if(state == pdPASS)
        {
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);

            vTaskDelay(500);

            /* Pass turn to Blue */
            xSemaphoreGive(Blue_Sem);
        }
    }
}

void Task_LED_Blue(void *A_Pv)
{
    while(1)
    {
        BaseType_t state = xSemaphoreTake(Blue_Sem, portMAX_DELAY);
        if(state == pdPASS)
        {
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);

            vTaskDelay(500);

            /* Pass turn back to Red */
            xSemaphoreGive(Red_Sem);
        }
    }
}

```

---

### **Lab 3:** Event Signaling via Counting Semaphore (EXTI to Task)

<img width="638" height="361" alt="image" src="https://github.com/user-attachments/assets/60b05789-cbc5-431e-8e64-d644d7b193b2" />

Use LED instead of LCD

**System Operations:**

1. Hardware button on `PA0` triggers `EXTI0` falling-edge interrupt, setting a volatile event flag.
2. `Task_Button` detects the flag, clears it, and gives the counting semaphore.
3. `Task_LED` takes tokens from the semaphore; each token turns ON the LED on `PA1` for $200\text{ ms}$ before turning it OFF.

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/semphr.h"

SemaphoreHandle_t xSemaphore;
volatile u8 buttonPressedFlag = 0;

/* Function Prototypes */
void APP_EXTI_Handler(void);
void Task_LED(void* pvParameters);
void Task_Button(void* pvParameters);

/* ISR Callback */
void APP_EXTI_Handler(void)
{
    buttonPressedFlag = 1; // Set event flag
}

int main(void)
{
    /* 1. Enable Peripheral Clocks */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableCLK(RCC_APB2, 14); // SYSCFG Clock

    /* 2. Configure Pins */
    GPIOx_PinConfig_t pins[2] = {
        {.Port = GPIO_PORTA, .Pin = GPIO_PIN0, .Mode = GPIO_MODE_INPUT,  .PullType = GPIO_PULL_UP},
        {.Port = GPIO_PORTA, .Pin = GPIO_PIN1, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL}
    };

    for(u8 i = 0; i < 2; i++)
    {
        MGPIO_vPinInit(&pins[i]);
    }

    /* 3. Configure External Interrupt (EXTI0) */
    MEXTI_vInit();
    MEXTI_vEnableINT(EXTI_LINE0);
    MEXTI_vSetTrigger(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(APP_EXTI_Handler, EXTI_LINE0);
    MNVIC_vEnable_Peripheral_INT(6); // EXTI0 IRQ

    /* 4. Create Counting Semaphore (Max Count = 5, Initial Count = 0) */
    xSemaphore = xSemaphoreCreateCounting(5, 0);

    /* 5. Create FreeRTOS Tasks */
    xTaskCreate(Task_LED,    (const char * const)"LED",    100, NULL, 1, NULL);
    xTaskCreate(Task_Button, (const char * const)"Button", 100, NULL, 2, NULL);

    /* 6. Start Scheduler */
    vTaskStartScheduler();

    while(1)
    {
        // Idle loop
    }

    return 0;
}

void Task_Button(void* A_Pv)
{
    while(1)
    {
        if(buttonPressedFlag)
        {
            buttonPressedFlag = 0; // Clear flag
            xSemaphoreGive(xSemaphore); // Post semaphore token
        }
        vTaskDelay(10);
    }
}

void Task_LED(void* A_Pv)
{
    while(1)
    {
        /* Block until a token is available */
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);
            vTaskDelay(200);
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);
        }
    }
}

```

---

### **Lab 4:** Inter-Task Communication via Message Queue (Multi-Button to 7-Segment)

<img width="640" height="364" alt="image" src="https://github.com/user-attachments/assets/bb36ca34-2333-4d29-9128-f4e27ec315bd" />

Use 3 buttons and 7 SEG 

**System Operations:**

1. **Producer Task (`Task_Button`):** Polls 3 push buttons (`PA0`: 1, `PA1`: 2, `PA2`: 3). When any button is pressed, its corresponding digit ID is pushed into `KeyQueue`.
2. **Consumer Task (`Task_7Seg`):** Blocks on `KeyQueue` indefinitely. When a value is received, it decodes the 7-segment bit pattern and updates pins `PA3` through `PA9`.

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#include "../FreeRTOS/FreeRTOS.h"
#include "../FreeRTOS/task.h"
#include "../FreeRTOS/queue.h"

/* Queue Handle */
QueueHandle_t KeyQueue;

/* Function Prototypes */
void Task_7Seg(void* pvParameters);
void Task_Button(void* pvParameters);

/* 7-Segment Common Cathode Lookup Table */
u8 Sevensegment[] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111   // 9
};

/* 7-Segment Pins Configuration (PA3 -> PA9) */
GPIOx_PinConfig_t SegmentPins[] = {
    {GPIO_PORTA, GPIO_PIN3, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    {GPIO_PORTA, GPIO_PIN4, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    {GPIO_PORTA, GPIO_PIN5, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    {GPIO_PORTA, GPIO_PIN6, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    {GPIO_PORTA, GPIO_PIN7, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    {GPIO_PORTA, GPIO_PIN8, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    {GPIO_PORTA, GPIO_PIN9, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
};

int main(void)
{
    /* 1. Enable GPIOA Clock */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    /* 2. Configure Button Pins (PA0, PA1, PA2) as Inputs with Pull-Up */
    GPIOx_PinConfig_t Buttons[3] = {
        {.Port = GPIO_PORTA, .Pin = GPIO_PIN0, .Mode = GPIO_MODE_INPUT, .PullType = GPIO_PULL_UP},
        {.Port = GPIO_PORTA, .Pin = GPIO_PIN1, .Mode = GPIO_MODE_INPUT, .PullType = GPIO_PULL_UP},
        {.Port = GPIO_PORTA, .Pin = GPIO_PIN2, .Mode = GPIO_MODE_INPUT, .PullType = GPIO_PULL_UP}
    };

    for(u8 i = 0; i < 3; i++)
    {
        MGPIO_vPinInit(&Buttons[i]);
    }

    /* 3. Configure 7-Segment Output Pins (PA3..PA9) */
    for(u8 i = 0; i < 7; i++)
    {
        MGPIO_vPinInit(&SegmentPins[i]);
    }

    /* 4. Create Message Queue (Capacity = 5 items, Item Size = 1 byte) */
    KeyQueue = xQueueCreate(5, sizeof(u8));

    /* 5. Create FreeRTOS Tasks */
    xTaskCreate(Task_7Seg,   (const char * const)"7Seg",   100, NULL, 1, NULL);
    xTaskCreate(Task_Button, (const char * const)"Button", 100, NULL, 2, NULL);

    /* 6. Start Scheduler */
    vTaskStartScheduler();

    while(1)
    {
        // Idle loop
    }

    return 0;
}

void Task_Button(void* A_Pv)
{
    u8 key;

    while(1)
    {
        /* Button 1 -> Digit 1 */
        if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN0) == GPIO_LOW)
        {
            key = 1;
            xQueueSend(KeyQueue, &key, 0);

            while(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN0) == GPIO_LOW)
            {
                vTaskDelay(10);
            }
        }

        /* Button 2 -> Digit 2 */
        if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN1) == GPIO_LOW)
        {
            key = 2;
            xQueueSend(KeyQueue, &key, 0);

            while(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN1) == GPIO_LOW)
            {
                vTaskDelay(10);
            }
        }

        /* Button 3 -> Digit 3 */
        if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN2) == GPIO_LOW)
        {
            key = 3;
            xQueueSend(KeyQueue, &key, 0);

            while(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN2) == GPIO_LOW)
            {
                vTaskDelay(10);
            }
        }

        vTaskDelay(10);
    }
}

void Task_7Seg(void* A_Pv)
{
    u8 bitValue = 0;
    u8 key;

    while(1)
    {
        /* Block until a key is received from the queue */
        if(xQueueReceive(KeyQueue, &key, portMAX_DELAY) == pdTRUE)
        {
            for(u8 pinNum = 0; pinNum < 7; pinNum++)
            {
                bitValue = GET_BIT(Sevensegment[key], pinNum);
                MGPIO_vSetPinValue(SegmentPins[pinNum].Port, SegmentPins[pinNum].Pin, bitValue);
            }
        }
    }
}

```
