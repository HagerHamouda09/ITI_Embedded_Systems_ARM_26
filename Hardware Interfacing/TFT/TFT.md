# (TFT Display - Cortex-M4 vs Cortex-M3)

**Key Architectural Points:**
* **Clocks:** `GPIOA` on **AHB1**; `SPI1` on **APB2** (Bit 12).
* **Alternate Functions:** `SCK` (`PA5`) and `MOSI` / `SDA` (`PA7`) routed through **AF5** (`GPIO_AF5`).

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../MCAL/SPI/SPI_int.h"
#include "../HAL/TFT/TFT_int.h"
#include "../HAL/TFT/TFT_Image.h"

int main(void)
{
    /* 1. Enable Peripheral Clocks */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_APB2, 12); // SPI1 Clock Enable

    /* 2. Configure SPI1 Pins (PA5 -> SCK, PA7 -> MOSI/SDA) */
    GPIOx_PinConfig_t MOSI = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN7,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF5
    };
    MGPIO_vPinInit(&MOSI);

    GPIOx_PinConfig_t SCK = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN5,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF5
    };
    MGPIO_vPinInit(&SCK);

    /* 3. Initialize TFT Controller */
    HTFT_vInit();

    /* 4. Display 16-Bit RGB565 Image Array */
    u16 size = sizeof(arr) / sizeof(arr[0]);
    HTFT_vShowImage(arr, size);

//HTFT_vFillBackgroundColor(0x0000);
//
//HTFT_vSetXPos(20, 70);
//HTFT_vSetYPos(20,90);
//
//HTFT_vFillRectangle(0xFFFF);

    while(1)
    {
    }

    return 0;
}

```

---

### (Cortex-M3 )

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/SPI/SPI_int.h"
#include "../HAL/TFT/TFT_int.h"
#include "../HAL/TFT/TFT_Image.h"

int main(void)
{
    /* 1. Enable Clocks on APB2 Bus */
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);
    MRCC_vEnableClk(RCC_APB2, 12); // SPI1 Clock Enable (Bit 12)

    /* 2. Configure SPI1 SCK (PA5) as Alternate Function Push-Pull */
    GPIOx_PinConfig_t SCK = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN5,
        .Mode       = GPIO_MODE_ALF,
        .Speed      = GPIO_SPEED_HIGH,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&SCK);

    /* 3. Configure SPI1 MOSI (PA7) as Alternate Function Push-Pull */
    GPIOx_PinConfig_t MOSI = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN7,
        .Mode       = GPIO_MODE_ALF,
        .Speed      = GPIO_SPEED_HIGH,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&MOSI);

    /* 4. Initialize TFT Controller */
    HTFT_vInit();

    /* 5. Display 16-Bit RGB565 Image Array */
    u16 size = sizeof(arr) / sizeof(arr[0]);
    HTFT_vShowImage(arr, size);

//HTFT_vFillBackgroundColor(0x0000);
//
//HTFT_vSetXPos(20, 70);
//HTFT_vSetYPos(20,90);
//
//HTFT_vFillRectangle(0xFFFF);

    while(1)
    {
    }

    return 0;
}

```
