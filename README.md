# ITI ARM Cortex-M Embedded Systems (STM32)

A modular embedded firmware repository containing layered device drivers (MCAL & HAL), an in-house RTOS scheduler, FreeRTOS ports, bootloader utilities, and IoT interfacing modules developed for ARM Cortex-M (STM32) microcontrollers.

---

##  Repository Architecture

The project follows a layered architecture with clean separation across low-level peripherals, external hardware abstractions, operating systems, and application utilities:

```text
├── APP/
│   └── HexParser/          # Intel HEX format parsing logic for bootloader updates
├── Drivers/
│   ├── LIB/                # Core bitwise definitions and standard data types
│   │   ├── BIT_MATH.h      # Bit manipulation macros (SET, CLR, TOG, GET)
│   │   └── STD_TYPES.h     # Standard platform-independent types
│   ├── MCAL/               # Microcontroller Abstraction Layer (Core Peripherals)
│   │   ├── DMA/            # Direct Memory Access Controller
│   │   ├── EXTI/           # External Interrupt / Event Controller
│   │   ├── FMI/            # Flash Memory Interface (In-Application Programming)
│   │   ├── GPIO/           # General Purpose I/O Driver
│   │   ├── NVIC/           # Nested Vectored Interrupt Controller
│   │   ├── RCC/            # Reset and Clock Control Driver
│   │   ├── SPI/            # Serial Peripheral Interface
│   │   ├── SYSCFG/         # System Configuration Controller (EXTI line mapping)
│   │   ├── SYSTICK/        # ARM Cortex-M Core SysTick Timer
│   │   └── USART/          # Universal Synchronous/Asynchronous Receiver-Transmitter
│   ├── HAL/                # Hardware Abstraction Layer (External Interfacing)
│   │   ├── DAC/            # Digital-to-Analog (R-2R audio / voice synthesis)
│   │   ├── ESP8266/        # Wi-Fi module driver (AT commands & TCP client)
│   │   ├── LEDMATRIX/      # Multiplexed LED Matrix display driver
│   │   ├── S2P/            # Serial-to-Parallel (Shift Register / 74HC595)
│   │   └── TFT/            # Thin-Film Transistor graphic display driver
│   └── OS/                 # Custom SysTick-based preemptive/periodic task scheduler
├── FreeRTOS/               # FreeRTOS kernel source and Cortex-M port files
└── Web/                    # Web demo scripts and IoT server interfacing

```

---

##  Layered Driver Structure

Every hardware driver adheres to the 5-file architecture:

* **`*_prv.h`:** Internal helper definitions, default bit-masks, and private function declarations.
* **`*_cfg.h`:** Static configuration options (clocks, prescalers, pin routing, and modes).
* **`*_int.h`:** Public API declarations, parameter enumerations, and user-facing structures.
* **`*_prg.c`:** Core driver logic, peripheral initializations, and ISR handling.

---

##  Environment & Toolchain

* **Target Microcontroller:** ARM 32-bit Cortex-M3 / Cortex-M4 (STM32F103 / STM32F401)
* **SDK & Toolchain:** IMT SDK (`WinARM / ArmGCC`) utilizing the `arm-none-eabi` cross-compiler
* **Hardware Debuggers / Programmers:** ST-LINK V2 

