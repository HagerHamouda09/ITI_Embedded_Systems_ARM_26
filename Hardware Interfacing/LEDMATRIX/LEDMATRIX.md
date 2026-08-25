#  (LED Matrix Display Driver)

### **Lab 1:** 8x8 LED Matrix Static Frame Display

**System Specifications:**
1. **Row Pins (8 Rows):** Connected to `PA0` through `PA7` configured as low-speed push-pull outputs.
2. **Column Pins (8 Columns):** Connected to `PB0`, `PB1`, `PB2`, `PB5`, `PB6`, `PB7`, `PB8`, and `PB9` configured as low-speed push-pull outputs (skipping reserved/JTAG pins `PB3` and `PB4`).
3. **Display Operation:** Uses persistence of vision (POV) multiplexing via `HLEDMATRIX_DisplayFrame` to render an 8-byte custom pattern frame array (`arr`) for approximately **10 seconds** ($500 \text{ frames} \times 20\text{ ms/frame}$).

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"
#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/NVIC/NVIC_int.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../HAL/LEDMATRIX/LEDMATRIX_int.h"

/* Custom 8x8 bitmap data frame */
u8 arr[8] = {0x00, 0x02, 0x03, 0xB1, 0xB9, 0x0F, 0x06, 0x00};

int main(void)
{
    /* 1. Enable Clocks for GPIOA and GPIOB on AHB1 Bus */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOA);
    MRCC_vEnableCLK(RCC_AHB1, RCC_GPIOB);

    /* 2. Configure 8 Row Pins (PA0 -> PA7) */
    GPIOx_PinConfig_t rows[8];
    for(u8 i = 0; i < 8; i++)
    {
        rows[i].Port       = GPIO_PORTA;
        rows[i].Pin        = i;
        rows[i].Mode       = GPIO_MODE_OUTPUT;
        rows[i].OutputType = GPIO_OT_PUSHPULL;
        rows[i].Speed      = GPIO_SPEED_LOW;
        rows[i].PullType   = GPIO_NO_PULL;
    }

    /* 3. Configure 8 Column Pins (PB0..PB2, PB5..PB9) */
    GPIOx_PinConfig_t cols[8] = {
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN0, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN1, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN2, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN5, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN6, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN7, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN8, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
        {.Port = GPIO_PORTB, .Pin = GPIO_PIN9, .Mode = GPIO_MODE_OUTPUT, .OutputType = GPIO_OT_PUSHPULL, .Speed = GPIO_SPEED_LOW, .PullType = GPIO_NO_PULL},
    };

    /* 4. Initialize LED Matrix HAL Driver */
    HLEDMATRIX_vInit(rows, 8, cols, 8);

    /* 5. Display Frame for ~10 seconds (500 refresh cycles x ~20ms) */
    HLEDMATRIX_DisplayFrame(arr, 500);

    while(1)
    {
        // Super Loop
    }

    return 0;
}

```
