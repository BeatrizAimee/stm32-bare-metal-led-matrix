#ifndef IRQ_H

#include "CMSIS/Device/ST/STM32L4xx/Include/stm32l475xx.h"
#include "CMSIS/Include/core_cm4.h" // For VTOR

extern uint32_t _stack;   // top of stack, provided by linker
extern void _start();     // entry point, provided by the startup code

#define MAKE_DEFAULT_HANDLER(Handler) \
    void __attribute__ ((weak)) Handler(void) { \
        __disable_irq(); /* Disable interrupts */ \
        while (1); /* Halts forever */ \
    };

void irq_init(void);

#endif