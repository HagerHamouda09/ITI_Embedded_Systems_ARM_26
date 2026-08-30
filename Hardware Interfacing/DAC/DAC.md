# (Digital-to-Analog Converter - DAC & Audio Playback)

### **Lab 1:** 8-Bit Audio Sample Playback via R-2R Ladder DAC & SysTick

**System Specifications:**
1. **DAC Hardware Interface:** An 8-bit R-2R resistor ladder DAC connected to pins `PA0` through `PA7` configured as low-speed push-pull outputs.
2. **Audio Sampling Rate:** 
   $$\text{Sampling Period } (T_s) = 125\ \mu\text{s} \implies f_s = \frac{1}{125\ \mu\text{s}} = 8000\text{ Hz } (8\text{ kHz})$$
3. **Execution Model:** The SysTick timer generates a periodic interrupt every **$125\ \mu\text{s}$** (`MSYSTICK_vSetIntervalMulti_us(125, DAC_handler)`). The ISR (`DAC_handler`) feeds consecutive 8-bit sound samples from the audio header (`voice.h`) to the DAC pins and loops back when reaching `voice_len`.


### **Hardware Connections & Audio Wiring:**
1. **DAC Bits (Digital Outputs):**
   * `Bit 0` $\rightarrow$ `PA0` (LSB)
   * `Bit 1` $\rightarrow$ `PA1`
   * `Bit 2` $\rightarrow$ `PA2`
   * `Bit 3` $\rightarrow$ `PA3`
   * `Bit 4` $\rightarrow$ `PA4`
   * `Bit 5` $\rightarrow$ `PA5`
   * `Bit 6` $\rightarrow$ `PA6`
   * `Bit 7` $\rightarrow$ `PA7` (MSB)
2. **Audio Jack / Speaker Interface:**
   * **Analog Output:** Connect the combined output of the R-2R Ladder to the **L / R** pin of the Audio Jack / Speaker module.
   * **Ground Reference:** Connect the **GND** of the Audio Jack directly to the MCU **GND**.
  
**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/SYSTICK/SYSTICK_int.h"
#include "../HAL/DAC/DAC_int.h"

#include "../HAL/DAC/voice.h"

void DAC_handler(void);

int main(void)
{
    /* 1. Initialize System Clock & Enable GPIOA Clock on AHB1 */
    MRCC_vInit();
    MRCC_vEnableCLK(RCC_AHB1, GPIO_PORTA);

    /* 2. Configure 8 DAC Pins on PA0 -> PA7 (8-bit R-2R Ladder) */
    GPIOx_PinConfig_t dac_pins[8] = {
        {GPIO_PORTA, GPIO_PIN0, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN1, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN2, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN3, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN4, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN5, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN6, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
        {GPIO_PORTA, GPIO_PIN7, GPIO_MODE_OUTPUT, GPIO_OT_PUSHPULL, GPIO_SPEED_LOW, GPIO_NO_PULL},
    };

    /* 3. Initialize DAC HAL Driver */
    HDAC_vInit(dac_pins, 8);

    /* 4. Start Periodic SysTick Interrupt at 8 kHz (Every 125 us) */
    MSYSTICK_vSetIntervalMulti_us(125, DAC_handler);

    while(1)
    {
    }

    return 0;
}


void DAC_handler(void)
{
    static u32 iterator = 0;

    HDAC_vSendSample(voice, iterator);

    iterator++;

    if (iterator == voice_len)
    {
        iterator = 0;
    }
}

```
