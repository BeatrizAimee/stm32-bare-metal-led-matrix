#ifndef MEMFUNCS_H

__attribute__((section(".bootloader"))) void* memory_copy(void* source, void* dest, unsigned int nbytes);
__attribute__((section(".bootloader"))) void* memory_set(void* dest, int value, unsigned int nbytes);
__attribute__((section(".bootloader"))) int memory_compare(const void* source, const void* dest, unsigned int nbytes);
__attribute__((section(".bootloader"))) void* memory_move(void* source, void* dest, unsigned int nbytes);



#endif