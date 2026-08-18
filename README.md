# STM32F411RE UART Bootloader

A learning project implementing a custom bootloader for the **STM32F411RE** microcontroller.

The purpose of this project is to understand how a microcontroller can receive new firmware through **UART**, program that firmware into internal Flash memory, and then transfer execution from the bootloader to the application.

---

## What is a Bootloader?

A **bootloader** is a small piece of firmware that executes before the main application firmware when a microcontroller starts or resets.

Its main responsibility is to initialize the minimum hardware required to decide how the device should boot.

A bootloader can:

* Check whether a valid application exists.
* Receive new firmware.
* Erase application Flash sectors.
* Program the new firmware into Flash.
* Verify the programmed firmware.
* Jump to the application firmware.

In a typical embedded system:

```text
              Power ON / Reset
                     |
                     v
              +--------------+
              |  Bootloader  |
              +--------------+
                     |
             Check boot mode
                     |
          +----------+----------+
          |                     |
          v                     v
    Firmware Update        Application Valid
          |                     |
          v                     |
    Receive Firmware            |
          |                     |
          v                     |
    Erase / Program             |
          |                     |
          +----------+----------+
                     |
                     v
              Start Application
```

---

## Why Do We Need a Bootloader?

Without a bootloader, firmware is commonly programmed using a debugger/programmer such as **ST-LINK** through the SWD interface.

That is useful during development, but it is not always practical for a deployed product.

For example, imagine a device installed inside a machine.

Without a suitable firmware-update mechanism:

```text
PC
 |
 | SWD / ST-LINK
 v
STM32
```

The device may need physical access to its programming/debug interface.

With a bootloader:

```text
PC
 |
 | UART
 v
+-------------+
| Bootloader  |
+-------------+
       |
       v
Internal Flash
       |
       v
Application
```

The firmware can potentially be updated without using the debugger.

Bootloaders are therefore commonly used for:

* Firmware updates
* Field upgrades
* Factory programming
* Device recovery
* Firmware version management
* Communication-based programming
* Supporting different update interfaces

Depending on the product, the bootloader may communicate through:

* UART
* USB
* CAN
* Ethernet
* SPI
* I2C
* Wireless interfaces

---

## Target Microcontroller

This project targets the:

**STM32F411RE**

Key memory used by this project:

| Memory | Start Address |   Size |
| ------ | ------------: | -----: |
| Flash  |  `0x08000000` | 512 KB |
| SRAM   |  `0x20000000` | 128 KB |

The internal Flash is divided into sectors.

### Flash Sector Layout

| Sector   | Start Address |   Size |
| -------- | ------------: | -----: |
| Sector 0 |  `0x08000000` |  16 KB |
| Sector 1 |  `0x08004000` |  16 KB |
| Sector 2 |  `0x08008000` |  16 KB |
| Sector 3 |  `0x0800C000` |  16 KB |
| Sector 4 |  `0x08010000` |  64 KB |
| Sector 5 |  `0x08020000` | 128 KB |
| Sector 6 |  `0x08040000` | 128 KB |
| Sector 7 |  `0x08060000` | 128 KB |

Flash ends at:

```text
0x08080000
```

---

## Memory Partition

The Flash memory is divided between the bootloader and application.

The current project uses the following conceptual layout:

```text
STM32F411RE Flash

0x08000000
+---------------------------+
|                           |
|       BOOTLOADER          |
|                           |
|       Sector 0            |
|       16 KB               |
|                           |
0x08004000
+---------------------------+
|                           |
|                           |
|       APPLICATION         |
|                           |
|       Sector 1 - 7        |
|                           |
|                           |
0x08080000
+---------------------------+
```

The bootloader occupies the beginning of Flash.

The application starts at:

```text
0x08004000
```

Therefore, the application cannot use the default Flash origin of `0x08000000`.

The application linker configuration must instead place the application at:

```text
FLASH ORIGIN = 0x08004000
```

---

## Bootloader vs Application

The bootloader and application are two separate firmware components.

### Bootloader

Responsible for:

* Startup decision
* Firmware reception
* Flash erase
* Flash programming
* Firmware verification
* Application jump

### Application

Responsible for the actual product functionality.

For example:

```text
Bootloader
    |
    | UART firmware update
    |
    v
Application
    |
    +-- GPIO
    +-- UART
    +-- Sensors
    +-- OLED
    +-- FreeRTOS
    +-- Application logic
```

The bootloader should generally remain small and reliable because a corrupted bootloader can make firmware recovery difficult.

---

## Important Concepts

This project is intended to demonstrate several important embedded-systems concepts.

### 1. Flash Memory

The STM32 internal Flash stores both the bootloader and application firmware.

The bootloader must understand the Flash sector structure because Flash sectors are erased as sectors rather than treating the entire Flash as ordinary RAM.

### 2. Linker Script

The linker script determines where sections of the application are placed in memory.

Normally an STM32 application may start at:

```text
0x08000000
```

For this project it must start at:

```text
0x08004000
```

This prevents the application from overwriting the bootloader.

### 3. Vector Table

The beginning of an STM32 application contains the vector table.

It contains, among other things:

```text
Initial Stack Pointer
Reset Handler
NMI Handler
HardFault Handler
...
```

Because the application no longer starts at `0x08000000`, the vector table also moves.

The bootloader must configure the vector table appropriately before transferring control to the application.

### 4. Application Jump

After completing its work, the bootloader transfers execution to the application's reset handler.

Conceptually:

```text
Bootloader
     |
     | Read application vector table
     |
     | Set MSP
     |
     | Set VTOR
     |
     | Jump to Reset_Handler
     |
     v
Application
```

### 5. Flash Programming

The bootloader receives firmware data and programs it into the application region of Flash.

The general process is:

```text
Receive firmware
       |
       v
Erase application sectors
       |
       v
Program Flash
       |
       v
Verify firmware
       |
       v
Jump to application
```

---

## Firmware Update Concept

A basic UART update can work like this:

```text
PC
 |
 | UART
 | Firmware data
 v
STM32 Bootloader
 |
 +--> Receive data
 |
 +--> Erase application sectors
 |
 +--> Write Flash
 |
 +--> Verify
 |
 +--> Start application
```

A production bootloader would normally require additional mechanisms such as:

* Firmware size checking
* CRC/checksum
* Firmware version
* Image validation
* Timeout handling
* Communication error handling
* Watchdog recovery
* Rollback/recovery mechanism
* Authentication/signature verification

These are important for a robust production implementation.

---

## Learning Objectives

The project is intended to build practical understanding of:

* STM32 Flash architecture
* Flash sectors
* Memory mapping
* Linker scripts
* Vector tables
* `SCB->VTOR`
* Main Stack Pointer
* Reset Handler
* Firmware images
* UART communication
* Flash erase/program operations
* Bootloader/application separation
* Firmware verification
* Embedded firmware architecture

---

## Project Structure

A typical project structure is:

```text
STM32F411RE_UART_Bootloader/
|
+-- Bootloader/
|   +-- Core/
|   +-- Drivers/
|   +-- ...
|
+-- Application/
|   +-- Core/
|   +-- Drivers/
|   +-- ...
|
+-- README.md
```

The exact structure depends on the STM32CubeIDE project configuration.

---

## Boot Sequence

The expected execution flow is:

```text
RESET
  |
  v
Bootloader
  |
  +---- Check update request
  |
  +---- Receive firmware if required
  |
  +---- Erase Flash
  |
  +---- Program application
  |
  +---- Verify application
  |
  v
Configure application environment
  |
  v
Set application vector table
  |
  v
Set Main Stack Pointer
  |
  v
Jump to application Reset_Handler
  |
  v
APPLICATION
```

---

## Why This Project Matters

A bootloader is a good intermediate project between basic STM32 programming and more advanced embedded firmware development.

It requires understanding how software interacts directly with the MCU memory architecture rather than simply writing application code using HAL APIs.

The project connects concepts such as:

```text
C Programming
     |
     v
STM32 Registers / HAL
     |
     v
Flash Memory
     |
     v
Linker Script
     |
     v
Vector Table
     |
     v
Startup Code
     |
     v
Bootloader
     |
     v
Firmware Update
```

---

## Future Improvements

Possible future improvements include:

* UART command protocol
* Firmware packetization
* CRC-32 verification
* Firmware size validation
* Firmware versioning
* Bootloader timeout
* User-controlled boot mode
* GPIO-based update mode
* Watchdog integration
* Error codes
* Application validity flag
* USB DFU
* CAN bootloader
* Dual-image firmware update
* Firmware encryption
* Cryptographic signature verification
* Rollback/recovery mechanism

---

## Disclaimer

This project is primarily intended for **learning and experimentation**.

A production bootloader should include significantly stronger validation, fault handling, security, recovery, and update-integrity mechanisms before being used in a real product.

---

## Hardware

Target MCU:

**STM32F411RE**

Communication interface:

**UART**

Programmer/debugger during development:

**ST-LINK / SWD**

---

## Author

**Rohit Kumar**

Embedded Systems / Electronics & Communication Engineering

This project is part of my embedded firmware learning journey.
