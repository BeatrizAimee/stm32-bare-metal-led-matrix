#EXERCISE 1 - README

## The Code

We must find out in which order the data sections appear in memory, as well as determine whether the stack grows upwards or downwards in a Linux x86_64 system (that is, if new data is stored at a higher or lower address than the data inserted before). 

With that in mind, we can write a C program in which we declare a variable or function for each section. That means the code will have a global initialized variable to represent .data, a global uninitialized one to represent .bss, a global constant for .rodata, and a function that will be allocated in .text. We also declare two local variables to track the stack, as well as dynamic ones to track the heap. By printing the address of each variable, we can figure out the order of the sections in memory.

For the stack growth direction, we have two local variables, one declared in _main_ and another in our function. The one declared in _main_ is pushed onto the stack first. By comparing which address is greater, we can determine if the second variable was inserted at a higher or lower address, that is, whether the stack grows upwards or downwards.


## Answers

By running the code, we get the following:


| Description                                       | Address           |
|---------------------------------------------------|-------------------|
| Function param (comes from main)                  | 0x7ffee01e3fd8    |
| Local variable                                    | 0x7ffee01e3fec    |
| Global initialized variable (.data)               | 0x5634e5f39030    |
| Global uninitialized variable (.bss)              | 0x5634e5f39038    |
| Global const variable (.rodata)                   | 0x5634e5f37008    |
| Function (.text)                                  | 0x5634e5f36169    |
| Heap variable A                                   | 0x5635008e82a0    |
| Heap variable B                                   | 0x5635008e82c0    |


Analysing and comparing the adresses, the section order in memory is : .text, .rodata, .data, .bss, heap and stack.

For the stack, since the adress of the local variable, pushed into the stack last, is lower then the adress of the variable declared in the main, we can be sure that it grows downwards.



