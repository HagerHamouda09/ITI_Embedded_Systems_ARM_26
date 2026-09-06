# (Direct Memory Access - DMA: Cortex-M4 vs Cortex-M3)

### **Lab 1:** High-Speed Memory-to-Memory Transfer on STM32F4 (Cortex-M4 via DMA2 Stream 0)
Write an ARM Cortex-M4 (STM32F4) C program to perform a hardware-accelerated **Memory-to-Memory (M2M)** block transfer of 10 32-bit Words from `arr1` to `arr2` using **DMA2 Stream 0**. Upon transfer completion, the DMA Transfer Complete interrupt triggers `LEDON` via callback, asserting status LED pin `PA0` `HIGH`.

**Key Architectural Points:**
* **Bus Architecture:** Both `GPIOA` and `DMA2` clocks reside on the **AHB1** bus (`DMA2` enabled on Bit 22 of `RCC_AHB1ENR`).
* **DMA Architecture:** STM32F4 features a **Stream/Channel matrix** with integrated FIFO buffers. Stream 0 uses IRQ position **56** (`DMA2_Stream0_IRQn`) and threshold configuration (`FIFO_FULL`).

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/DMA/DMA_int.h"

void LEDON(void);

u32 arr1[10] = {1, 4, 2, 6, 7, 33, 55, 25, 13, 5};
u32 arr2[10] = {0};

int main(void)
{
    /* 1. Initialize System Clock */
    MRCC_vInit();

    /* 2. Enable Peripheral Clocks on AHB1 Bus (GPIOA and DMA2 on Bit 22) */
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_AHB1, 22);

    /* 3. Configure PA0 as Output Push-Pull for Status LED */
    GPIOx_PinConfig_t LED = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN0,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL
    };
    MGPIO_vPinInit(&LED);

    /* 4. Enable DMA2 Stream 0 Interrupt in NVIC (Position 56) */
    MNVIC_vEnable_Peripheral_INT(56);

    /* 5. Initialize DMA2 Stream 0 */
    MDMA2_vInit(0);

    /* 6. Configure Stream 0: Source, Destination, Data Widths, Block Size, FIFO */
    MDMA2_vSetStreamCfg(0,
                        arr1,
                        arr2,
                        Word,
                        Word,
                        10,
                        FIFO_FULL);

    /* 7. Register Transfer Complete Callback Function */
    MDMA2_vCallBack(0, LEDON);

    /* 8. Enable DMA2 Stream 0 to Start Transfer */
    MDMA2_vEnableStream(0);

    while(1)
    {
    }

    return 0;
}

/* ================= DMA2 Stream 0 Transfer Complete Callback ================= */

void LEDON(void)
{
    /* Drive PA0 HIGH upon transfer completion */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
}

```

---

### **Lab 2:** Memory-to-Memory Transfer on STM32F103 (Cortex-M3 via DMA1 Channel 1)

Write an ARM Cortex-M3 (STM32F103) C program to execute a **Memory-to-Memory (M2M)** transfer of 10 32-bit Words from `arr1` to `arr2` using **DMA1 Channel 1** (ID 0). Upon transfer completion, the channel interrupt executes `func`, driving `PA0` `HIGH`.

**Key Architectural Points:**

* **Bus Architecture:** DMA1 clock resides on the **AHB** bus (Bit 0 of `RCC_AHBENR`), while GPIO ports are clocked via the APB2 bus or AHB bridge depending on the MCAL driver mapping.
* **DMA Architecture:** STM32F1 uses a simplified **Channel-based** structure without configurable FIFOs. DMA1 Channel 1 maps to IRQ position **11** (`DMA1_Channel1_IRQn`).

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/DMA/DMA_int.h"

void func(void);

u32 arr1[10] = {1, 4, 2, 6, 7, 33, 55, 25, 13, 5};
u32 arr2[10] = {0};

int main(void)
{
    /* 1. Initialize System Clock */
    MRCC_vInit();

    /* 2. Enable Peripheral Clocks (GPIOA and DMA1 on AHB Bit 0) */
    MRCC_vEnableClk(RCC_APB2, GPIO_PORTA);
    MRCC_vEnableClk(RCC_AHB, 0);

    /* 3. Configure PA0 as Output Push-Pull for Status LED */
    GPIOx_PinConfig_t Led = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN0,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_2MHZ,
        .OutputType = GPIO_OT_PUSHPULL
    };
    MGPIO_vPinInit(&Led);

    /* 4. Enable DMA1 Channel 1 Interrupt in NVIC (Position 11) */
    MNVIC_vEnable_Peripheral_INT(11);

    /* 5. Initialize DMA1 Channel 1 (Index 0) */
    MDMA_vInit(0);

    /* 6. Configure Channel 1: Source, Destination, Widths, Length */
    MDMA_vSetChannelCfg(0,
                        arr1,
                        arr2,
                        Word,
                        Word,
                        10);

    /* 7. Register Transfer Complete Callback */
    MDMA1_vCallBack(0, func);

    /* 8. Enable Channel to Start Transfer */
    MDMA1_vEnableChannel(0);

    while(1)
    {
    }

    return 0;
}

/* ================= DMA1 Channel 1 Transfer Complete Callback ================= */

void func(void)
{
    /* Drive PA0 HIGH upon transfer completion */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN0, GPIO_HIGH);
}

