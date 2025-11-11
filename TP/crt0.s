    .syntax unified
    .global _start
    .thumb



    .thumb_func @ Define function in Thumb mode
    _start:
        ldr sp, =_stack @ Initialize stack pointer --> tells where the stack starts
        bl init_bss @ Call bss initialization
        bl main @ Call main and then exit
        
    _exit:
        b _exit @ Infinite loop to protect the processor
    
