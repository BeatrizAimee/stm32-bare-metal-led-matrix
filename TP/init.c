// imports from external file (linker script)
extern int _bss_start; // start of bss section
extern int _bss_end; // end of bss section

void init_bss() {
    int* start = &_bss_start; // keeps the start address of bss section
    int* end = &_bss_end; // keeps the end address of bss section

    while (start < end) {
        *start++ = 0; // set each address in bss section to 0
    }
}
