# ITI Embedded Systems ARM (STM32 Cortex-M3 / M4) 2026

A production-grade firmware repository containing layered device drivers, RTOS kernel scheduler implementations, IoT & wireless communication modules, and in-system bootloader designs targeting ARM Cortex-M3 / M4 microcontrollers (STM32 series).

---

##  Repository Architecture

The repository adheres to a strict layered embedded architecture separating core peripherals, bus controllers, hardware modules, and system tools:

```text
├── Drivers/
│   ├── LIB/                        # Standard bitwise macros and portable type headers
│   │   ├── BIT_MATH.h
│   │   └── STD_TYPES.h
│   ├── MCAL/                       # Core Microcontroller Abstraction Layer
│   │   ├── RCC/                    # Reset and Clock Control Driver
│   │   ├── GPIO/                   # General Purpose I/O Driver
│   │   ├── NVIC/                   # Nested Vectored Interrupt Controller
│   │   ├── EXTI/                   # External Interrupt / Event Controller
│   │   ├── SYSTICK/                # ARM Core SysTick Timer
│   │   ├── DMA/                    # Direct Memory Access Controller
│   │   ├── USART/                  # Serial Communications & TTL Interfacing
│   │   └── SPI/                    # Serial Peripheral Interface
│   ├── HAL/                        # Hardware Abstraction Layer
│   │   ├── DAC_R2R/                # R-2R Ladder Audio & Voice Synthesis
│   │   ├── STP/                    # Serial-to-Parallel (Shift Register) Driver
│   │   └── ESP8266_WIFI/           # Wi-Fi Module AT Command Driver & TCP/IP Client
│   └── OS/                         # Custom SysTick-based RTOS Task Scheduler
├── Bootloader/                     # In-System Programming & Flashing Engine
│   ├── Hex_Parser/                 # Intel HEX file decoding utility
│   └── FBL/                        # Custom Flash Bootloader implementation
├── IoT_Web_Demo/                   # Web interface and cloud broker connectivity demos
└── AI_Assisted_Dev/                # Prompt engineering & generative firmware workflows

```

---

##  Layered Driver Architecture

All MCAL and HAL drivers follow the standard AUTOSAR-inspired 4-file structure:

* **`*_prv.h`:** Internal driver macros, bitfield definitions, and helper function declarations.
* **`*_cfg.h`:** Pre-build static configurations (clock sources, pin alternate functions, trigger edges).
* **`*_int.h`:** Public API function prototypes, exported structs, and configuration types.
* **`*_prg.c`:** Core API implementations and ISR handling routines.

---

##  Environment & Toolchain

* **Target Architecture:** ARM 32-bit Cortex-M3 / Cortex-M4 (STM32F103 / STM32F401 )
* **IDE & Development Tools:** IMT_SDK based on Eclipse
* **Hardware Tools & Debuggers:** ST-LINK v2

