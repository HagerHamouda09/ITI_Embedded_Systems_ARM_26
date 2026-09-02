#  (NEC IR Remote Protocol Decoder )

### **System Operations & Execution Flow:**
1. **Clock Setup & Line Routing:**
   * **Cortex-M4 (STM32F4):** Enables `GPIOA` on `AHB1` and `SYSCFG` on `APB2` (Bit 14).
   * **Cortex-M3 (STM32F1):** Enables `GPIOA` on `APB2` and `AFIO` on `APB2` (Bit 0).
2. **Interrupt & Timestamp Capture (`APP_GET_TIME`):**
   * **Start Leader Edge:** Arms a $15\text{ ms}$ single-shot SysTick callback on the first falling edge.
   * **Bit Edges:** Converts SysTick elapsed counts to microseconds ($\mu\text{s}$) with the factor $/3.125$, stores the duration into `G_u32Arr`, and rearms a $4\text{ ms}$ single-shot interval.
3. **Data Frame Decoding (`APP_DecodeBits`):**
   * Processes the 8-bit command payload located at array indices `17` through `24`.
   * **Logical `0`:** $1000\ \mu\text{s} \le \Delta t \le 1250\ \mu\text{s} \implies$ Clears bit $i$.
   * **Logical `1`:** $2000\ \mu\text{s} \le \Delta t \le 2450\ \mu\text{s} \implies$ Sets bit $i$.
   * Resets capture buffer flags and counters for the next transmission frame.

---

### **C Implementation Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"

/* Function Prototypes */
void APP_GET_TIME(void);
void APP_DecodeBits(void);

/* Global State Variables */
volatile u8  G_u8StartingFlag = 0;
volatile u32 G_u32Arr[50]     = {0};
volatile u8  G_u8Counter      = 0;
volatile u8  G_u8Value        = 0;

int main(void)
{
    /* 1. Initialize System Clock */
    MRCC_vInit();

    /* ---------------- Peripheral Clock Selection ---------------- */
    /* Cortex-M4 (STM32F4) */
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_APB2, 14); // SYSCFG Clock (Bit 14)

    /* Cortex-M3 (STM32F1) - Alternative */
    // MRCC_vEnableCLK(RCC_APB2, RCC_GPIOA);
    // MRCC_vEnableCLK(RCC_APB2, 0);  // AFIO Clock (Bit 0)

    /* 2. Configure IR Input Pin (PA0) */
    GPIOx_PinConfig_t IR = {
        .Port = GPIO_PORTA,
        .Pin  = GPIO_PIN0,
        .Mode = GPIO_MODE_INPUT
    };
    MGPIO_vPinInit(&IR);

    /* 3. Configure Status LED Pin (PA1) */
    GPIOx_PinConfig_t LED = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL
    };
    MGPIO_vPinInit(&LED);

    /* 4. Configure External Interrupt (EXTI Line 0, Falling Edge) */
    MEXTI_vInit();
    MEXTI_vEnableINT(EXTI_LINE0);
    MEXTI_vSetTrigger(EXTI_LINE0, EXTI_FALLING_EDGE);
    MEXTI_vSetCallBack(APP_GET_TIME, EXTI_LINE0);

    /* 5. Enable EXTI0 IRQ Channel in NVIC (Position 6) */
    MNVIC_vEnable_Peripheral_INT(6);

    /* 6. Configure SysTick Timer (Interrupt Enabled, AHB / 8 Clock Source) */
    MSYSTIC_Config_t STK_CFG = {
        .InterruptEnable = INT_ENABLE,
        .CLK_SRC         = CLK_SRC_AHB_8
    };
    MSYSTICK_vInit(&STK_CFG);

    while(1)
    {
    }

    return 0;
}

/* ================= EXTI0 ISR: Timestamp Capture ================= */

void APP_GET_TIME(void)
{
    if(G_u8StartingFlag == 0)
    {
        /* First Falling Edge: Start of Frame Leader Code */
        G_u8StartingFlag = 1;
        MSYSTICK_vSetIntervalSingle(15, APP_DecodeBits);
    }
    else
    {
        /* Capture bit pulse duration in microseconds */
        G_u32Arr[G_u8Counter++] = MSYSTICK_u32GetElapsedTime_SingleShot() / 3.125;

// M3
//        G_u32Arr[G_u8Counter++] = MSYSTICK_u32GetElapsedTime_SingleShot();

        /* Rearm timeout for the next incoming bit */
        MSYSTICK_vSetIntervalSingle(4, APP_DecodeBits);
    }
}

/* ================= SysTick Callback: Frame Decoder ================= */

void APP_DecodeBits(void)
{
    G_u8Value = 0;

    /* Decode Command Byte (Indices 17 to 24) */
    for(u8 i = 0; i < 8; i++)
    {
        if(G_u32Arr[17 + i] >= 1000 && G_u32Arr[17 + i] <= 1250)
        {
            CLR_BIT(G_u8Value, i); // Logical '0'
        }
        else if(G_u32Arr[17 + i] >= 2000 && G_u32Arr[17 + i] <= 2450)
        {
            SET_BIT(G_u8Value, i); // Logical '1'
        }
    }

    /* Reset State Flags and Counters */
    G_u8StartingFlag = 0;
    G_u8Counter      = 0;

    /* Clear capture buffer */
    for(u8 i = 0; i < 50; i++)
    {
        G_u32Arr[i] = 0;
    }
}

```

```

```
