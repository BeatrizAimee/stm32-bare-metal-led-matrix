
#include "memfuncs.h"

/*memcpy - copies a number of bytes from source to destination - receives the adress of the source and destination
as well as the number of bytes to be copied in a void pointer (means that in the adress stored exists any type of data)*/
__attribute__((section(".bootloader")))
void* memory_copy(void* source, void* dest, unsigned int nbytes) {

    if (nbytes == 0 || source == dest) return dest; // if there are no bytes to copy or source and destination are the same return the pointer
    unsigned char* c_src = (unsigned char*)source; // transforms the void pointer in a char pointer as 1 byte = 1 char
    unsigned char* c_dst = (unsigned char*)dest; 

    for (unsigned int i = 0; i < nbytes; i++) {
        c_dst[i] = c_src[i]; // copy each byte from source to destination
    }

    return dest; // return the destination address
}


/*memset - sets a number of bytes in a desired memory area to a specific value - receives the adress of the memory area,
the value to be set and the number of bytes to be set in a void pointer*/
__attribute__((section(".bootloader")))
void* memory_set(void* dest, int value, unsigned int nbytes) {
    if (nbytes == 0) return dest; // if there are no bytes to set return the pointer

    unsigned char* c_dst = (unsigned char*)dest; 

    for (unsigned int i = 0; i < nbytes; i++) {
        c_dst[i] = (unsigned char)value; // set each byte to the specified value
    }

    return dest; // return the original pointer
}

/*memory_compare - compares two memory areas byte by byte - receives the addresses of the two memory areas and the number 
of bytes to be compared in void pointers*/
__attribute__((section(".bootloader")))
int memory_compare(const void* source, const void* dest, unsigned int nbytes) {
    if (nbytes == 0 || source == dest) return 0; // if there are no bytes to compare or source and destination are the same return 0 (equal)
    const unsigned char* c_source = (const unsigned char*)source;
    const unsigned char* c_dst = (const unsigned char*)dest;

    for (unsigned int i = 0; i < nbytes; i++) {
        if (c_source[i] != c_dst[i]) {
            if (c_source[i] < c_dst[i]) {
                return -1; // if source - destination < 0 return -1 (neg number)
            } else {
                return 1; // if source - destination > 0 return 1 (pos number)
            }
        }
    }

    return 0; // if all bytes are equal return 0
}

/*memory_move - moves a number of bytes from source to destination, taking in consideration cases where memory areas overlap 
- receives the address of the source and destination as well as the number of bytes to be moved in a void pointer*/
__attribute__((section(".bootloader")))
void* memory_move(void* source, void* dest, unsigned int nbytes) {
    if (nbytes == 0 || source == dest) return dest; // if there are no bytes to move or source and destination are the same return the pointer

    unsigned char* c_src = (unsigned char*)source;
    unsigned char* c_dst = (unsigned char*)dest;

    if (c_src > c_dst || c_dst >= c_src + nbytes) { // destiny is behind source or starts after source ends -- copied bytes don't overlap, the copy forwards (normal)
        for (unsigned int i = 0; i < nbytes; i++) {
            c_dst[i] = c_src[i];
        }
    } else {
        // If they do overlap, copy backward
        for (unsigned int i = nbytes; i > 0; i--) {
            c_dst[i - 1] = c_src[i - 1];
        }
    }

    return dest;
}
