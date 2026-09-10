#  (Bootloader - Intel HEX Record Parsing & In-Application Programming)

### **Lab:** Intel HEX Record Parsing and Flash Programming on STM32F4 (Cortex-M4)
Write an ARM Cortex-M4 (STM32F4) C program to parse an incoming Intel HEX data record string (`:1080000041030008450300088D03000800000000AC`) and flash its payload into target memory.

**Key Execution Details:**
* **Erase Architecture:** Flash is organized in variable-sized sectors. Erasing target flash base `0x08008000` requires erasing **Sector 2** (`MFMI_vSectorErase(Sector2)`).
* **Record Parsing:** `AHexParser_vParseRecord` extracts the byte count, 16-bit address offset, record type, data bytes, and checksum, then commits the half-words directly to flash memory via the FMI driver.

**Code:**
```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/FMI/FMI_int.h"

#include "HexParser/HexParser_int.h"

int main(void)
{
    /* 1. Initialize System Clock */
    MRCC_vInit();

    /* Intel HEX Data Record (16 bytes payload at offset 0x8000) */
    u8 arr[] = ":1080000041030008450300088D03000800000000AC";

    /* 2. Erase Sector 2  */
    MFMI_vSectorErase(Sector2);

    /* 3. Parse Record & Program Extracted Half-Words to Flash */
    AHexParser_vParseRecord(arr);

    while(1)
    {
    }

    return 0;
}

```

---

### **Lab:** Intel HEX Record Parsing and Flash Programming on STM32F103 (Cortex-M3)

Write an ARM Cortex-M3 (STM32F103) C program to parse the identical Intel HEX record string and commit the payload to flash at address `0x08008000`.

**Key Execution Details:**

* **Erase Architecture:** Flash is organized into fixed-size pages ($1\text{ KB}$ per page on medium-density devices). Target address `0x08008000` is the start boundary of **Page 32** ($32 \times 1024 = 32768 = 0x8000$), erased via `MFMI_vPageErase(0x08008000)`.
* **Record Parsing:** The parser decodes the record and commits the half-words using the STM32F1 FMI page-programming sequence.

**Code:**

```c
#include "../LIB/STD_TYPES.h"
#include "../LIB/BIT_MATH.h"

#include "../MCAL/RCC/RCC_int.h"
#include "../MCAL/GPIO/GPIO_int.h"
#include "../MCAL/FMI/FMI_int.h"

#include "HexParser/HexParser_int.h"

int main(void)
{
    /* 1. Initialize System Clock */
    MRCC_vInit();

    /* Intel HEX Data Record (16 bytes payload at offset 0x8000) */
    u8 arr[] = ":1080000041030008450300088D03000800000000AC";

    /* 2. Erase Page 32 (1 KB page starting at 0x08008000) */
    MFMI_vPageErase(0x08008000);

    /* 3. Parse Record & Program Extracted Half-Words to Flash */
    AHexParser_vParseRecord(arr);

    while(1)
    {
    }

    return 0;
}

```
