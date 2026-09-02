#  (SPI Peripheral: Cortex-M4 vs Cortex-M3)

# Cortex-M4

**Key Architectural Points:**
* **Bus Architecture:** `GPIOA` clock resides on the **AHB1** bus; `SPI1` peripheral clock is enabled on **APB2** (Bit 12).
* **Alternate Function Multiplexing:** `SCK` (`PA5`), `MISO` (`PA6`), and `MOSI` (`PA7`) are configured in Alternate Function mode (`GPIO_MODE_ALF`) routed through Alternate Function 5 (`GPIO_AF5`).

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

int main(void)
{
    /* 1. Enable Clocks (AHB1 for GPIOA, APB2 Bit 12 for SPI1) */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);
    MRCC_vEnableCLK(RCC_APB2, 12);

    /* 2. Configure Status LED Pin (PA1) */
    GPIOx_PinConfig_t led = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&led);

    /* 3. Configure SPI1 Pins (PA5 -> SCK, PA6 -> MISO, PA7 -> MOSI) with AF5 */
    GPIOx_PinConfig_t SCK = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN5,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF5
    };
    MGPIO_vPinInit(&SCK);

    GPIOx_PinConfig_t MISO = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN6,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF5
    };
    MGPIO_vPinInit(&MISO);

    GPIOx_PinConfig_t MOSI = {
        .Port    = GPIO_PORTA,
        .Pin     = GPIO_PIN7,
        .Mode    = GPIO_MODE_ALF,
        .AltFunc = GPIO_AF5
    };
    MGPIO_vPinInit(&MOSI);

    /* 4. Initialize SPI1 Peripheral Driver */
    MSPI_vInit();

    /* 5. Transceive Data Byte & Verify Response */
    u8 data = MSPI_u8Transceive('T');
    if(data == 'T')
    {
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);
    }

    while(1)
    {
        // Super Loop
    }

    return 0;
}

```

---

# (Cortex-M3)


**Key Architectural Points:**

* **Bus Architecture:** Both `GPIOA` and `SPI1` peripheral clocks reside on the **APB2** bus (`SPI1` enabled on Bit 12 of `RCC_APB2ENR`).
* **GPIO Pin Modes (F1 Style):**
* `SCK` (`PA5`) & `MOSI` (`PA7`): Alternate Function Push-Pull (`GPIO_MODE_ALF`, `GPIO_SPEED_HIGH`).
* `MISO` (`PA6`): Input Floating / Input with No Pull (`GPIO_MODE_INPUT`, `GPIO_NO_PULL`).



**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/SPI/SPI_int.h"

int main(void)
{
    /* 1. Enable Peripheral Clocks on APB2 Bus */
    MRCC_vInit();
    MRCC_vEnableClk(RCC_APB2, RCC_GPIOA);
    MRCC_vEnableClk(RCC_APB2, 12); // SPI1 Clock Enable (Bit 12)

    /* 2. Configure Status LED Pin (PA1) */
    GPIOx_PinConfig_t led = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN1,
        .Mode       = GPIO_MODE_OUTPUT,
        .Speed      = GPIO_SPEED_LOW,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&led);

    /* 3. Configure SPI1 Pins (PA5 -> SCK, PA6 -> MISO, PA7 -> MOSI) */
    GPIOx_PinConfig_t SCK = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN5,
        .Mode       = GPIO_MODE_ALF,
        .Speed      = GPIO_SPEED_HIGH,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&SCK);

    GPIOx_PinConfig_t MISO = {
        .Port     = GPIO_PORTA,
        .Pin      = GPIO_PIN6,
        .Mode     = GPIO_MODE_INPUT,
        .PullType = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&MISO);

    GPIOx_PinConfig_t MOSI = {
        .Port       = GPIO_PORTA,
        .Pin        = GPIO_PIN7,
        .Mode       = GPIO_MODE_ALF,
        .Speed      = GPIO_SPEED_HIGH,
        .OutputType = GPIO_OT_PUSHPULL,
        .PullType   = GPIO_NO_PULL
    };
    MGPIO_vPinInit(&MOSI);

    /* 4. Initialize SPI1 Peripheral Driver */
    MSPI_vInit();

    /* 5. Transceive Data Byte & Verify Response */
    u8 data = MSPI_u8Transceive('T');
    if(data == 'T')
    {
        MGPIO_vSetPinValue(GPIO_PORTA, GPIO_PIN1, GPIO_HIGH);
    }

    while(1)
    {
        // Super Loop
    }

    return 0;
}

```

