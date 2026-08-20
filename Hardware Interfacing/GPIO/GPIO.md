# GPIO Labs (STM32f401 RCC & GPIO Driver)

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

```
---

### **Lab 2:** Periodic LED Blinking on PA0

<img width="324" height="51" alt="image" src="https://github.com/user-attachments/assets/021928ad-ec70-4ac2-99a9-0ef5d75c9f1d" />

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#define DELAY_MS(d)   do{unsigned int i = (d) * 4000; while(i--) { asm("NOP"); }} while(0)

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

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

    while(1)
    {
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
        DELAY_MS(500);
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);
        DELAY_MS(500);

        /*
         * OR
                MGPIO_vTogPinValue(GPIO_PORTA, GPIO_PIN0);
                DELAY_MS(500);
         */

    }
}

```

---

### **Lab 3:** Push Button Controlled LED with Debouncing

<img width="500" height="64" alt="image" src="https://github.com/user-attachments/assets/0306c4f1-26d8-419b-82f0-a8c97f4e2195" />

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#define DELAY_MS(d)   do{unsigned int i = (d) * 4000; while(i--) { asm("NOP"); }} while(0)

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    GPIOx_PinConfig_t LED =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN0,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t Button =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_INPUT,
        .PullType   = GPIO_PULL_UP
    };

    MGPIO_vPinInit(&LED);
    MGPIO_vPinInit(&Button);

    while(1)
    {
        if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN1) == GPIO_LOW)
        {
            DELAY_MS(10); // Software Debounce
            if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN1) == GPIO_LOW)
            {
                MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
                DELAY_MS(50);
            }
        }
        else
        {
            MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);
        }
    }
}

```

---

### **Lab 4:** Multi-Color RGB LED State Sequencer

Write an ARM Cortex-M C program to cycle through color modes on an RGB LED (`PA0`: Red, `PA1`: Green, `PA2`: Blue) sequentially each time a button on `PA3` is pressed:


$$\text{OFF} \longrightarrow \text{RED} \longrightarrow \text{GREEN} \longrightarrow \text{BLUE} \longrightarrow \text{WHITE} \longrightarrow \text{OFF}$$

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#define DELAY_MS(d)   do{unsigned int i = (d) * 4000; while(i--) { asm("NOP"); }} while(0)

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    GPIOx_PinConfig_t Red =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN0,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t Green =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t Blue =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN2,
        .Mode       = GPIO_MODE_OUTPUT,
        .OutputType = GPIO_OT_PUSHPULL,
        .Speed      = GPIO_SPEED_LOW,
        .PullType   = GPIO_NO_PULL
    };

    GPIOx_PinConfig_t Button =
    {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN3,
        .Mode       = GPIO_MODE_INPUT,
        .PullType   = GPIO_PULL_UP
    };

    MGPIO_vPinInit(&Red);
    MGPIO_vPinInit(&Green);
    MGPIO_vPinInit(&Blue);
    MGPIO_vPinInit(&Button);

    /* Turn OFF initially */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN2, GPIO_LOW);

    u8 L_u8Color = 0;

    while(1)
    {
        if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN3) == GPIO_LOW)
        {
            DELAY_MS(20); // Debounce press

            if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN3) == GPIO_LOW)
            {
                L_u8Color++;
                if(L_u8Color > 4)
                {
                    L_u8Color = 0;
                }

                /* Clear previous outputs */
                MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_LOW);
                MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);
                MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN2, GPIO_LOW);

                switch(L_u8Color)
                {
                    case 0: /* OFF */
                        break;

                    case 1: /* RED */
                        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
                        break;

                    case 2: /* GREEN */
                        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);
                        break;

                    case 3: /* BLUE */
                        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN2, GPIO_HIGH);
                        break;

                    case 4: /* WHITE */
                        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
                        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);
                        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN2, GPIO_HIGH);
                        break;
                }

                while(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN3) == GPIO_LOW); // Wait for release
                DELAY_MS(20); // Debounce release
            }
        }
    }
}

```

---

### **Lab 5:** Push Button Incremented 7-Segment Display

<img width="532" height="75" alt="image" src="https://github.com/user-attachments/assets/ebee5285-e353-4420-8d04-8b32106164f3" />

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"

#define DELAY_MS(d)   do{unsigned int i = (d) * 4000; while(i--) { asm("NOP"); }} while(0)

//Common Cathod
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

int main(void)
{
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);

    GPIOx_PinConfig_t SegmentPins[] = {
        {GPIO_PORTA, GPIO_PIN0, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN1, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN2, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN3, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN4, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN5, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN6, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL}
    };

    GPIOx_PinConfig_t ButtonPin = {
        .Port     = GPIO_PORTA,
        .Pin      = GPIO_PIN7,
        .Mode     = GPIO_MODE_INPUT,
        .PullType = GPIO_PULL_UP
    };

    for(u8 i = 0; i < 7; i++)
    {
        MGPIO_vPinInit(&SegmentPins[i]);
    }
    MGPIO_vPinInit(&ButtonPin);

    u8 count = 0;

    while(1)
    {
        /* Display current digit */
        for(u8 pinNum = 0; pinNum < 7; pinNum++)
        {
            u8 bitValue = GET_BIT(Sevensegment[count], pinNum);
            MGPIO_vSetPinValue(GPIO_PORTA, pinNum, bitValue);
        }

        /* Check button press */
        if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN8) == GPIO_LOW)
        {
            DELAY_MS(50); // Debounce press

            if(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN8) == GPIO_LOW)
            {
                count++;
                if(count > 9)
                {
                    count = 0;
                }

                while(MGPIO_u8GetPinValue(GPIO_PORTA, GPIO_PIN8) == GPIO_LOW); // Wait for release
                DELAY_MS(50); // Debounce release
            }
        }
    }
}

```

