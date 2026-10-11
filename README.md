# STM32 Bare-Metal LED Matrix

A personal bare-metal firmware project for an 8×8 RGB LED matrix, built for the
STM32L475VG (Cortex-M4).

## Highlights

- GPIO-driven serial interface and row multiplexing for the LED matrix
- RGB framebuffer refresh driven by a 60 Hz TIM2 interrupt
- UART frame reception at 38,400 baud
- External button interrupt through EXTI/NVIC
- Custom startup code, interrupt vector table, linker script and memory initialization
- ARM cross-compilation and J-Link/GDB debugging setup

## Repository layout

- [`TP/`](TP) — STM32 firmware, build files, linker script and debug configuration
- [`exercices/`](exercices) — preparatory exercises in assembly, compilation and Make

## Preparatory exercises

- [`assembleur/`](exercices/assembleur) — ARMv7E-M routines for memory
  initialization, block copying and null-terminated string copying, with J-Link/GDB
  build and debug configuration for the STM32.
- [`compilation/ex1/`](exercices/compilation/ex1) — C experiment mapping the
  `.text`, `.rodata`, `.data`, `.bss`, heap and stack regions of a Linux process.
- [`compilation/ex2/`](exercices/compilation/ex2) — ARM object-file analysis across
  `-O0`, `-O1`, `-O2` and `-Os`, including the effects of array and pointer
  declarations on generated sections. The [report](exercices/compilation/ex2/ex2.md)
  contains the observations.
- [`make/`](exercices/make) — a multi-file C build using explicit targets,
  dependencies, pattern rules, target-specific flags and a separate library directory.

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
