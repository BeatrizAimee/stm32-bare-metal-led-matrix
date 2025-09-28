/*Assembler directives */
  
.syntax unified
.arch armv7e-m                              @processor architecture
.cpu cortex-m4 
.thumb

.global _start                              @declare a symbol to be used elsewhere (in this case, in the C program)

/*labels - act like variables*/

.equ start_mem, 0x10000000                  @initial address 
.equ dest_mem, 0x10000300                   @address to be moved to
.equ string_cp_addr, 0x10000A00             @address to copy the string
.equ data, 0xdeadbeef                       @data to be placed in memory
.equ data_size, 64                          @number of words to be written (4 bytes each)
.equ counter, 0

_start: @global

    ldr sp, =0x20018000                     @stack pointer, sets where the stack will be. First usable RAM address
    ldr r0, =start_mem 
    ldr r1, =dest_mem
    ldr r2, =data 
    mov r3, #counter                        @ store the number of words written, i=0
    mov r4, #data_size                  

    /* ex 1 & 2 */
    bl _write                               @branch and link (bl), which writes in the link register (lr) the memory address of the exit branch
    bl _copy

    /* ex3 - new write and copy with different parameters*/

    ldr r0, =0x10000400                     @ new start
    ldr r1, =0x10000600                     @ new dest
    ldr r2, =0x42424242                     @ new pattern
    mov r4, #32                             @ 32 words (128 bytes)

    bl _write
    bl _copy

    /*ex4 - string copy */
    ldr r0, =msg                            @load the address of the string to be copied
    ldr r1, =string_cp_addr                 @load the destination address
    bl _str_copy


    /* ex 4 - copy #2, to string_cp_addr + 0x100 */
    add r1, r1, #0x100          
    bl _str_copy

    end: 
        b end                               @infinite loop 

    /*Memory zone Initialization - ex1 */
    @Parameters: r0 - start address, r2 - data to be written, r3 - counter (i)
    _write:
        push {r0,r3}                        @save the state of registers before modification
    _write_loop:

        str r2,[r0],#4                      @ store the content of r2 at the address pointed by r0 and add 4 bytes to the address
        add r3,r3,#1                        @ #immediate value. r3=r3+1. i++
        cmp r3, r4                          @ compare with the loop limit
        blt _write_loop                     @ branch if less than (i<64)
        pop {r0,r3}                         @restore the state of registers
        bx lr                               @branch to address in register (in this case lr, which points to the origin branch)


    /*Memory zone copy - ex2 */
    @Parameters: r0 - source address, r1 - destination address, r3 - counter (i)
    _copy:
        push {r0,r1,r2,r3} 
    _copy_loop:

        ldr r2,[r0],#4                      @ load and overwrite into r2 the content of r0 and add 4 bytes to the address
        str r2,[r1],#4                      @ store the content of r2 at the address pointed by r1 (dest) and add 4 bytes to the address
        add r3,r3,#1                        
        cmp r3, r4                
        blt _copy_loop                     
        pop {r0,r1,r2,r3}                   
        bx lr                                    

    /*String copy - ex3 - bonus */
    @Parameters: r0 - source address (*msg), r1 - destination address (dest), r2 - data to be copied (msg), r3 - counter (i)
    _str_copy:
        push {r0,r1,r2,r3}                  
    _str_copy_loop:
        ldrb r2, [r0], #1                   @ load byte and overwrite into r2 the content of r0 (msg) and add 1 byte to the address (1byte=1char)
        strb r2, [r1], #1                   @ store byte the content of r2 at the address pointed by r1 (dest) and add 1 byte to the address
        cmp r2, #0                          @ compare with null terminator
        bne _str_copy_loop                  @ branch if not equal (r2 != 0)
        pop {r0,r1,r2,r3}                   
        bx lr     
.section .rodata 
    msg: .asciz "Bonjour le monde!"         @string to be printed, stored in the .rodata section with null terminator


