#  ARM Session 17 Labs (Flash Memory Interface - FMI: Programming & Erasing)

### **Lab 1:** In-Application Flash Programming & Sector/Page Erasing (M4 vs M3)
Write an ARM C program using the Flash Memory Interface (**FMI**) driver to demonstrate in-application flash memory modification, sector/page erasing, and half-word array flashing at designated base addresses (`0x08008000` on STM32F4 and `0x08001400` on STM32F103).

---

### **Test & Verification Flow:**
1. **Direct Write (Clean/Erased State):** Program the initial array (`arr[3] = {0x31, 0x32, 0x33}`) directly to target flash memory to verify half-word write functionality.
2. **Overwrite Without Erase Test:** Attempt writing modified values (`{0x34, 0x35, 0x36}`) over previously programmed locations without an erase cycle. NOR flash can clear bits ($1 \to 0$) but cannot set them ($0 \to 1$), leading to corrupted data bitwise ANDing.
3. **Appending to Unwritten Flash:** Append an extra half-word element (`0x37`) at the next untouched boundary; verify the write succeeds because unwritten flash bits remain in the erased `$0\text{xFFFF}$` state.
4. **Erase Cycle Verification:** Execute an erase operation to return target locations to `$0\text{xFFFF}$`:
   * **STM32F4 (Cortex-M4):** Erase by **Sector** (`Sector2` at `0x08008000`).
   * **STM32F103 (Cortex-M3):** Erase by **Page** (`Page 5` at `0x08001400`).
5. **Post-Erase Reprogramming:** Flash the complete 4-element array (`arr[4]`) into the cleared block and inspect memory to confirm data integrity.

---

### **C Implementation (STM32F4 - Sector-Based):**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/FMI/FMI_int.h"

int main(void)
{
    MRCC_vInit();

    u16 arr[4] = {0x34, 0x35, 0x36, 0x37};

    /* Erase Sector 2 (Address range: 0x08008000 - 0x0800BFFF) */
    MFMI_vSectorErase(Sector2);

    /* Program half-word data array to flash */
    MFMI_vProgramFlash(0x08008000, arr, 4);

    while(1)
    {
    }
}

```

---

### **C Implementation (STM32F103 - Page-Based):**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/FMI/FMI_int.h"

int main(void)
{
    MRCC_vInit();

    u16 arr[4] = {0x34, 0x35, 0x36, 0x37};

    /* Erase flash page at base address 0x08001400 (Page 5) */
    MFMI_vPageErase(0x08001400);

    /* Program half-word data array to flash */
    MFMI_vProgramFlash(0x08001400, arr, 4);

    while(1)
    {
    }
}

```

```

```
