    .syntax unified
    .global _start
    .thumb

    .section .bootloader, "ax", %progbits @ tell linker  to put the code in bootloader section.

    .thumb_func @ Define function in Thumb mode
    _start:
        ldr sp, =_stack @ Initialize stack pointer --> tells where the stack starts
        bl copy_isr_vectors @ Copy sections to RAM
        bl copy_text
        bl copy_data 
        bl init_bss @ Call bss initialization
        bl main @ Call main and then exit
        
    _exit:
        b _exit @ Infinite loop to protect the processor
    
