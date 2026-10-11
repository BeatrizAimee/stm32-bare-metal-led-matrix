# STM32 Bare-Metal LED Matrix

Bare-metal C and ARM assembly firmware for an STM32L475VG that receives a
pre-rendered scrolling phrase over UART and displays it on an 8×8 RGB LED matrix.

The project was built without an RTOS, vendor HAL or C standard library. It includes
the startup path, memory initialization, interrupt vector table, linker script,
peripheral configuration and display driver.

## What it does

1. A host renders a phrase as a sequence of 8×8 RGB frames.
2. The frames are sent to the board through USART1.
3. The USART receive interrupt stores the newest frame in memory.
4. A TIM2 interrupt refreshes the display at 60 Hz by multiplexing its eight rows.
5. Repeating the frame sequence makes the phrase scroll across the matrix.

The repository includes `one_frame.bin` and `many_frames.bin`, which contain
one frame and a complete sequence of frames respectively.

## Hardware and peripherals

- **Target MCU:** STM32L475VG, ARM Cortex-M4 at 80 MHz
- **Display:** 8×8 RGB LED matrix controlled through GPIO bit-banging
- **Serial input:** USART1 on PB6 (TX) and PB7 (RX), configured at 38,400 baud
- **Refresh timer:** TIM2, configured for a 60 Hz update interrupt
- **User input:** PC13 button through EXTI13
- **Debug interface:** SEGGER J-Link over SWD

## UART frame format

Each UART packet has 193 bytes: a one-byte synchronization marker followed by a
192-byte display frame.

```text
0xFF | 64 RGB pixels
     | pixel 0: R G B
     | pixel 1: R G B
     | ...
     | pixel 63: R G B
```

The `0xFF` byte synchronizes reception. The firmware then stores the following
192 bytes as a row-major RGB framebuffer. On every timer interrupt, the display
driver reads this framebuffer and scans the matrix one row at a time.

## Firmware architecture

| Area | Files | Responsibility |
| --- | --- | --- |
| Boot and memory | `crt0.s`, `init.c`, `memfuncs.c`, `ld_ram.lds` | Sets the stack, copies vectors/`.text`/`.data` from flash to SRAM, clears `.bss`, and starts `main`. |
| Interrupts | `irq.c`, `timer.c`, `button.c`, `uart.c` | Defines the vector table and handles TIM2, USART1 and EXTI13 interrupts. |
| Platform setup | `clocks.c`, `led.c` | Configures the 80 MHz system clock and board LEDs. |
| Matrix driver | `matrix.c`, `matrix.h` | Configures GPIO, shifts RGB data, latches it and multiplexes the eight rows. |
| Build and debug | `Makefile`, `stm32-4se.gdb` | Cross-compiles the firmware and provides a J-Link/GDB workflow. |

### Matrix driver

The driver configures the matrix control and row-select pins as high-speed GPIOs.
For each refresh cycle it:

- shifts B, G and R bytes for eight pixels, most-significant bit first;
- latches the transferred values into the LED controller;
- enables one row, then disables it before moving to the next one.

It also contains visual hardware tests: the letters **B**, **E** and **A** in blue,
green and red, plus an RGB intensity gradient.

### Interrupt-driven runtime

`main` performs initialization and then remains idle. Runtime work happens in
interrupt handlers:

- **USART1 IRQ** synchronizes on `0xFF` and receives a full RGB frame.
- **TIM2 IRQ** scans the latest framebuffer to keep the display refreshed.
- **EXTI13 IRQ** handles the user button and toggles the board's green LED.

This keeps display refresh independent of serial reception.

## Build

The firmware is in `TP/`.

```bash
cd TP
make
```

Requirements:

- `arm-none-eabi-gcc` and related binutils
- `gdb-multiarch`
- SEGGER J-Link tools
- An STM32L475VG connected through SWD

## Flash and debug

In one terminal, start the J-Link GDB server:

```bash
cd TP
make connect
```

In a second terminal, build and open GDB:

```bash
cd TP
make startgdbserver
```

Inside GDB:

```gdb
flash
cont
```

The `stm32-4se.gdb` script also provides layouts for assembly/source debugging and
an `armex` helper for inspecting a Cortex-M exception stack frame.

## Send a demo sequence

Configure the serial device at the same baud rate as the firmware:

```bash
cd TP
sh stty.sh
```

Then send a bundled frame sequence to the board:

```bash
cat many_frames.bin > /dev/ttyACM0
```

Adjust the serial-device path if your board is exposed elsewhere.

## Additional low-level exercises

The [`exercices/`](exercices) directory contains supporting work on:

- ARMv7E-M assembly: memory initialization, copying blocks and copying C strings;
- memory sections and compiler optimization analysis;
- Makefiles for multi-file cross-compilation.
