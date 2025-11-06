    .syntax unified
    .global _start
    .thumb

    _start:
        ldr sp, =_stack @ Initialize stack pointer --> tells where the stack starts
        bl init_bss @ Call bss initialization
        bl main @ Call main
        bl _exit @ Call exit
    _exit:
        b _exit @ Infinite loop to protect the processor
    
