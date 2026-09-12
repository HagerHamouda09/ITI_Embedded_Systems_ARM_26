#  (Bootloader: Full Firmware Flashing)

#  Firmware Generation Pipeline (Linker Relocation & HEX Conversion)

The firmware array (`arr`) included in `out.h` is generated from a separate user application (such as a test program that turns an LED ON via `PA0`) through a specialized compilation and conversion workflow.

### **Pipeline Workflow:**

* **Linker Script Relocation (`.ld`):** The application project's linker script is modified so that the flash origin starts at `0x08008000` (matching Sector 2 on STM32F4 or Page 32 on STM32F103). The available flash size is reduced by subtracting the bootloader's reserved offset (e.g., subtracting $32\text{ KB}$, leaving $224\text{ KB}$ for standard M4 builds).
* **Binary Compilation & Hex Export:** Compiling this relocated project produces an Intel HEX file (`testled.hex`) containing the application's machine code mapped to its new memory boundaries.
* **Batch Conversion (`HEX_TO_ARRAY.bat`):** Placing the conversion script and the compiled hex file in the same directory, the raw Intel HEX strings are processed via the command line:
```cmd
HEX_TO_ARRAY.bat testled.hex out.h

```


* **C Array Integration (`out.h`):** The conversion script outputs a structured C array (`arr`), which is included by the bootloader project to flash the application binary sequentially into target memory at runtime.

---

### **Lab 1:** In-Application Programming (IAP) Firmware Flashing & Jump on STM32F4 (Cortex-M4)
Write an ARM Cortex-M4 (STM32F4) C bootloader program that erases target application flash memory at base address `0x08008000` (**Sector 2**), parses and flashes an array of 372 Intel HEX records (`out.h`), and branches execution to the user application.

**Key Architectural Points:**
* **Flash Architecture:** Sector 2 spans $16\text{ KB}$ (`0x08008000` to `0x0800BFFF`), which provides sufficient contiguous erased space for the entire 372-record firmware payload.
* **Vector Table Structure:**
  * **Offset `+0x00`:** Contains the initial Main Stack Pointer (**MSP**) address.
  * **Offset `+0x04`:** Contains the application **Reset Handler** entry point address.
* **Application Jump Sequence:**
  1. Read the MSP value at `0x08008000` and write it to the CPU core register via inline assembly (`MSR MSP, %0`).
  2. Read the Reset Handler address at `0x08008004`.
  3. Cast the address to a function pointer `void (*APP_CALL)(void)` and invoke it to transfer control.

---

### Expected Output
Led on PA1 should turn on then off and after that Led on PA0 turns on

----


**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/FMI/FMI_int.h"
#include "../MCAL/USART/USART_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"

#include "HexParser/HexParser_int.h"
#include "HexParser/out.h"

#define APP_BASE_ADDR    0x08008000

void JumpToApp(void)
{
    void (*APP_CALL)(void);

    /* 1. Fetch & Initialize Main Stack Pointer (MSP) */
    u32 MSPAddress = *((volatile u32*)(APP_BASE_ADDR));
    __asm volatile("MSR MSP, %0" :: "r"(MSPAddress));

    /* 2. Fetch Reset Handler address & jump to application */
    u32 ResetHandlerAddress = *((volatile u32*)(APP_BASE_ADDR + 4));
    APP_CALL = (void (*)(void))ResetHandlerAddress;

    APP_CALL();
}

int main(void)
{
    /* 1. Initialize System Clock & Enable GPIOA */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);

    /* 2. Configure Status LED on PA1 */
    GPIOx_PinConfig_t led = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&led);

    /* Turn LED ON to signal bootloading / flashing in progress */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);

    /* 3. Erase Target Application Sector (Sector 2 = 16 KB) */
    MFMI_vSectorErase(Sector2);

    /* 4. Parse and flash Intel HEX firmware records */
    for(u16 i = 0; i < 372; i++)
    {
        AHexParser_vParseRecord(arr[i]);
    }

    /* Turn LED OFF to signal flashing completion */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);

    /* 5. Transfer execution to user application */
    JumpToApp();

    while(1)
    {

    }

    return 0;
}

```

---

### **Lab 2:** In-Application Programming (IAP) Firmware Flashing & Jump on STM32F103 (Cortex-M3)

Write an ARM Cortex-M3 (STM32F103) C bootloader program to parse and flash the identical firmware array starting at base address `0x08008000` and branch to the user application.

**Key Architectural Points:**

* **Flash Page Boundaries:** Medium-density STM32F1 devices use $1\text{ KB}$ ($1024\text{ bytes}$) pages. Because 372 Intel HEX data records exceed $1\text{ KB}$ ($\approx 6\text{ KB}$ of binary code), multiple consecutive pages starting from **Page 32** (`0x08008000` to `0x080097FF`, 6 pages) must be erased prior to flashing.
* **Clock Routing:** `GPIOA` clock resides on the **APB2** peripheral bus.

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/FMI/FMI_int.h"

#include "HexParser/HexParser_int.h"
#include "HexParser/out.h"

#define APP_BASE_ADDR    0x08008000
#define NUM_PAGES_ERASE  6 // 6 KB allocated for user application

void JumpToApp(void)
{
    void (*APP_CALL)(void);

    /* 1. Fetch & Initialize Main Stack Pointer (MSP) */
    u32 MSPAddress = *((volatile u32*)(APP_BASE_ADDR));
    __asm volatile("MSR MSP, %0" :: "r"(MSPAddress));

    /* 2. Fetch Reset Handler address & jump to application */
    u32 ResetHandlerAddress = *((volatile u32*)(APP_BASE_ADDR + 4));
    APP_CALL = (void (*)(void))ResetHandlerAddress;

    APP_CALL();
}

int main(void)
{
    /* 1. Initialize System Clock & Enable GPIOA on APB2 */
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);

    /* 2. Configure Status LED on PA1 */
    GPIOx_PinConfig_t led = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_2MHZ,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&led);

    /* Turn LED ON to signal flashing phase */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);

    /* 3. Erase consecutive 1 KB pages starting at Page 32 (0x08008000) */
    for(u8 page = 0; page < NUM_PAGES_ERASE; page++)
    {
        MFMI_vPageErase(APP_BASE_ADDR + (page * 1024));
    }

    /* 4. Parse and flash Intel HEX records */
    for(u16 i = 0; i < 372; i++)
    {
        AHexParser_vParseRecord(arr[i]);
    }

    /* Turn LED OFF to signal flashing completion */
    MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_LOW);

    /* 5. Transfer execution to user application */
    JumpToApp();

    while(1)
    {
    }

    return 0;
}

```
