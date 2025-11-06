/*Linker Script*/


/*Memory Layout -- where each part starts and size. Code+data are on sram1 and 
stack on sram2 --> separeted so that if a stack overflow happens, the program access a 
retricted area and crashes automatically*/

MEMORY
        {
            FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 1M
            SRAM1(rwx): ORIGIN = 0x20000000, LENGTH = 96K
            SRAM2(rwx): ORIGIN = 0x10000000, LENGTH = 32K
        }


_stack = ORIGIN(SRAM2) + LENGTH(SRAM2) ;

/*Each .o has the same sections as the executable (each has one .text, .data, etc) and the linker 
"connects" them in the executable in SECTIONS*/

SECTIONS
        {
            /* Program code */
            .text : {
                *(.text*)  /*taking all .text from the .o and put them here */
            } > SRAM1 /*placing the .text in SRAM1 (VMA) -- if we put at > [LMA] we are mapping it to the physical address space (where it is in the flash)*/

            /* Read only data */
            .rodata : {
                *(.rodata*) /*taking all .rodata from the .o and put them here */
            } > SRAM1 /*placing the .rodata in SRAM1 (VMA)*/

            /* Initialized data */
            .data : {
                *(.data*) /*taking all .data from the .o and put them here */
            } > SRAM1 /*placing the .data in SRAM1 (VMA)*/

            /* Uninitialized data */
            .bss : {
                _bss_start = .; /*defining a label to know where the .bss starts*/
                *(.bss*) /*taking all .bss from the .o and put them here */
                *(COMMON) /*taking all COMMON from the .o and put them here */
                _bss_end = .; /*defining a label to know where the .bss ends*/
            } > SRAM1 /*placing the .bss in SRAM1 (VMA)*/


        }


/*Without a elf loader (as its the case), the entry point is not needed, the processor will
search for the adress 0x0000_0004 in the flash and start from there. But as the program is in
the sram, we would need to write in the adress 0x0000_0004 a code that jumps to the sram adress.
In the end we are using the gbd as a "loader", inserting an entry point label in the executable*/
ENTRY(_start)
