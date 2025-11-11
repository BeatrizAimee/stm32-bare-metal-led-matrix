#include "memfuncs.h"


// imports from external file (linker script)
extern char _bss_start; // start of bss section. char garanteed alignment, as its a byte
extern char _bss_end; // end of bss section
extern char _start_vector; // start of isr_vector section
extern char _end_vector; // end of isr_vector section
extern char _start_text; // start of text section
extern char _end_text; // end of text section
extern char _data_start; // start of data section
extern char _end_data; // end of data section

//load addresses in FLASH
extern char _vector_load;
extern char _text_load;
extern char _data_load;


// Place these functions in the .bootloader section

__attribute__((section(".bootloader")))
void copy_isr_vectors() {
    memory_copy(&_vector_load, &_start_vector, &_end_vector - &_start_vector);
} //copy the isr_vector from flash to ram

__attribute__((section(".bootloader")))
void copy_text() {
    memory_copy(&_text_load, &_start_text, &_end_text - &_start_text);
} //copy the text section from flash to ram

__attribute__((section(".bootloader")))
void copy_data() {
    memory_copy(&_data_load, &_data_start, &_end_data - &_data_start);
} //copy the data section from flash to ram

__attribute__((section(".bootloader")))
void init_bss(){
    memory_set(&_bss_start, 0, &_bss_end - &_bss_start);
} //set the bss section to zero