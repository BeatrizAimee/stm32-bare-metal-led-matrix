#ifndef MEMFUNCS_H

void* memory_copy(void* source, void* dest, unsigned int nbytes);
void* memory_set(void* dest, int value, unsigned int nbytes);
int memory_compare(const void* source, const void* dest, unsigned int nbytes);
void* memory_move(void* source, void* dest, unsigned int nbytes);



#endif