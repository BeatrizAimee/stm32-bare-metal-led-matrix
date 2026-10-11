# STM32 Bare-Metal LED Matrix

My implementation for the bare-metal programming lab of Télécom Paris's
4SE03 course (formerly SE203). It targets an STM32L475VG (Cortex-M4) and drives
an 8×8 RGB LED matrix.

The lab's goal is to build a program progressively from a minimal environment:
cross-compilation, direct peripheral access, startup code and debugging, without
an OS, C standard library, vendor HAL or RTOS.

## Highlights

- GPIO-driven serial interface and row multiplexing for the LED matrix
- RGB framebuffer refresh driven by a 60 Hz TIM2 interrupt
- UART frame reception at 38,400 baud
- External button interrupt through EXTI/NVIC
- Custom startup code, interrupt vector table, linker script and memory initialization
- ARM cross-compilation and J-Link/GDB debugging setup

## Repository layout

- [`TP/`](TP) — STM32 firmware, build files, linker script and debug configuration
- [`exercices/`](exercices) — exercises on assembly, compilation and Make

## Build

The main firmware lives in `TP/`.

```bash
cd TP
make
```

The Makefile expects:

- `arm-none-eabi-gcc` and related binutils
- `gdb-multiarch`
- SEGGER J-Link tools for debugging

## Debug

Start the J-Link GDB server:

```bash
cd TP
make connect
```

Then, in another terminal:

```bash
make startgdbserver
```
