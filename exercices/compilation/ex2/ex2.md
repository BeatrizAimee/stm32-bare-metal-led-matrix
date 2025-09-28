# Exercise 2 - README


## Q1 - Compilation with Diferents Optimization Levels

**Instruction:** Compile the ex2.c code for ARM without linking, using a recent toolchain, with the following optimization levels: Os, O0, O1, and O2. For each optimization level, justify the sizes of the data sections that you obtain.

For starters, we'll compile  the code without linking, that is, using -c (compile only, dont link), creating an object for each optimization:

```bash
arm-none-eabi-gcc -c -O0 -o ex2_O0.o ex2.c
arm-none-eabi-gcc -c -O1 -o ex2_O1.o ex2.c
arm-none-eabi-gcc -c -O2 -o ex2_O2.o ex2.c
arm-none-eabi-gcc -c -Os -o ex2_Os.o ex2.c
```

Now, we can analyse the objects with objdump. We'll run the flag -h, in order to show the headers (lists all sections in the file, like .text, .bss, etc, along with their memory adresses and sizes), the flag -s, to show the contents of all sections, byte by byte, and the flag -d to turn the machine code into assembly instructions (disassemble). The command is:

```bash
arm-none-eabi-objdump -hsd ex2_O0.o ex2_O1.o ex2_O2.o ex2_Os.o > objdump_report.txt
```

### O0 optimization level

In the output file we can observe the following: 

- There are two "hello world!" strings stored in .rodata, indicating that the compiler treats one as belonging to `const char mesg[]` and another as part of `printf(mesg);`, showing lack of optimization. 
- In the disassembled section, the compiler replaces `printf(mesg);` with a call to `puts` According to the manual  `puts` handles less arguments, like strings terminated by a newline character, meaning it's a simpler function than `printf`, and thus an optmization was made. However, the compiler doesn´t optimize the second `printf()`, as it contains a more complex string that uses placeholders 

### O1 optimization level

In the output file we can observe the following: 

- The sizes of the sections `.text`, `.rodata.str1.4`, and `.rodata` are smaller then the optimization done before. This indicates that redundancies were removed and, in general, the code was optimized. 
- `.rodata.str1.4` is created, a section where mergeable strings can be allocated (strings that have the same value and can be logically merged in one), and is used to store the first `printf(mesg)`.
- In the `.rodata` section, exists the string that stores the const `m̀esg` array

### O2 optimization level

In the output file we can observe the following: 

- There wasn't any change in the sections size. However, the section  `.text` was emptied and the code moved to the `.text.startup` section. This place is where the startup code is stored (for exemple the main and the initialization functions), as they are run only one time at the program start.

### Os optimization level

In the output file we can observe the following: 

- The size of `.rodata.str1.1` is reduced in relation to O1/O2.That can be archieved by reducing the alignment from 2² = 4 bytes to 2⁰ = 1 byte (because there won't be any extra padding if the string is smaller then 4 bytes).
- The `.text.startup` section also gets smaller since the compiler optimizes and simplifies the generated instructions, removing redundancy

### Final Solution

| Section            | -O0 (hex / dec) | -O1 (hex / dec) | -O2 (hex / dec) | -Os (hex / dec) |  
|--------------------|-----------------|-----------------|-----------------|-----------------|
| `.text`            | 0xb4 / 180      | 0x6c / 108      | 0x00 / 0        | 0x00 / 0        |
| `.text.startup`    | N/A             | N/A             | 0x6c / 108      | 0x68 / 104      |
| `.data`            | 0x04 / 4        | 0x04 / 4        | 0x04 / 4        | 0x04 / 4        |
| `.bss`             | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        |
| `.rodata.str1.4`   | N/A             | 0x30 / 48       | 0x30 / 48       | N/A             |
| `.rodata.str1.1`   | N/A             | N/A             | N/A             | 0x2d / 45       |
| `.rodata`          | 0x40 / 64       | 0x0e / 14       | 0x0e / 14       | 0x0e / 14       |
| `.comment`         | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       |
| `.ARM.attributes`  | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       |
| **Total**    | **0x14e** / **334**       | **0x104** / **260**       | **0x104** / **260**       | **0xfd** / **253**|


The size of the sections decreases as the optimization level increases. The `.text` undergoes the most work to reduce redundancy, making it the most reduced one. As expected, -Os produces the smallest size, applying more optimization techniques, while -O0 has the largest size and no optimizations at all. One thing to note is that the biggest size drop happens from -O0 to -O1. From there, the changes are mostly structural and don’t affect the size as much.


## Q2 - Static Declaration

**Instruction:** Replace const char mesg[] with static const char mesg[]. Explain the differences in the data sections compared to the previous question.

After changing the variable and re-compiling/re-analysing the object files, we have the following:

| Section           | -O0 (hex / dec) | -O1 (hex / dec) | -O2 (hex / dec) | -Os (hex / dec) |
|-------------------|-----------------|-----------------|-----------------|-----------------|
| `.text`           | 0xb4 / 180      | 0x6c / 108      | 0x00 / 0        | 0x00 / 0        |
| `.text.startup`   | N/A             | N/A             | 0x6c / 108      | 0x68 / 104      |
| `.data`           | 0x04 / 4        | 0x04 / 4        | 0x04 / 4        | 0x04 / 4        |
| `.bss`            | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        |
| `.rodata`         | 0x40 / 64       | N/A             | N/A             | N/A             |
| `.rodata.str1.4`  | N/A             | 0x30 / 48       | 0x30 / 48       | N/A             |
| `.rodata.str1.1`  | N/A             | N/A             | N/A             | 0x2d / 45       |
| `.comment`        | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       |
| `.ARM.attributes` | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       |
| **Total**         |**0x14e** / **334**|**0xf6** / **246**|**0xf6** / **246**|**0xef** / **239**|


It is clear that the only change occured in `.rodata`, since it's not present in the -O1, -O2 and -Os. That means the compiler interpreted _static_ as an indicative that the variable will be used only in the present file. With no possibility of external linking, the variable can be interpreted as mergeable and passed to the `.rodata.str` section.

## Q3 - Pointer Declaration

**Instruction:** Replace const char mesg[] with const char *mesg, and then with const char * const mesg.
Explain the differences in the generated code and in the data sections compared to question 2

### const char *mesg

After changing the variable and re-compiling/re-analysing the object files, we have the following:

| Section           | -O0 (hex / dec) | -O1 (hex / dec) | -O2 (hex / dec) | -Os (hex / dec) |
|-------------------|-----------------|-----------------|-----------------|-----------------|
| `.text`           | 0xbc / 188      | 0x68 / 104      | 0x00 / 0        | 0x00 / 0        |
| `.text.startup`   | N/A             | N/A             | 0x68 / 104      | 0x64 / 100      |
| `.data`           | 0x08 / 8        | 0x08 / 8        | 0x08 / 8        | 0x08 / 8        |
| `.bss`            | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        |
| `.rodata`         | 0x30 / 48       | N/A             | N/A             | N/A             |
| `.rodata.str1.4`  | N/A             | 0x2e / 46       | 0x2e / 46       | N/A             |
| `.rodata.str1.1`  | N/A             | N/A             | N/A             | 0x2e / 46       |
| `.comment`        | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       |
| `.ARM.attributes` | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       |
| **Total**         |**0x14a** / **330**|**0xf4** / **244**|**0xf4** / **224**|**0xf0** / **240**|

In this case, the total size of almost every section is reduced, with some exceptions: the -Os version of .`.rodata.str1.1`, which increases from 0x2d to 0x2e, and the `.data` section, which grows from 0x04 to 0x08. The increase in `.data` represents the storage of a pointer. As in question 2, `.rodata` is not present in the -O1, -O2, and -Os levels. It’s also notable that, unlike in the previous cases, the compiler now uses `printf` for both print statements instead of replacing the first one with `puts`. This happens because the variable is now a pointe, and the compiler only knows that the pointer refers to some memory address, but it can’t guarantee that the data at that address is constant or even that it ends with a newline character. As a result, the `.text` section also becomes larger than in the previous question at -O0.

### const char * const mesg

After changing the variable and re-compiling/re-analysing the object files, we have the following:

| Section           | -O0 (hex / dec) | -O1 (hex / dec) | -O2 (hex / dec) | -Os (hex / dec) |
|-------------------|-----------------|-----------------|-----------------|-----------------|
| `.text`           | 0xb4 / 180      | 0x6c / 108      | 0x00 / 0        | 0x00 / 0        |
| `.text.startup`   | N/A             | N/A             | 0x6c / 108      | 0x68 / 104      |
| `.data`           | 0x04 / 4        | 0x04 / 4        | 0x04 / 4        | 0x04 / 4        |
| `.bss`            | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        | 0x05 / 5        |
| `.rodata`         | 0x44 / 68       | 0x04 / 4        | 0x04 / 4        | N/A             |
| `.rodata.str1.4`  | N/A             | 0x3e / 62       | 0x3e / 62       | N/A             |
| `.rodata.str1.1`  | N/A             | N/A             | N/A             | 0x3b / 59       |
| `.comment`        | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       | 0x27 / 39       |
| `.ARM.attributes` | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       | 0x2a / 42       |
| **Total**         |**0x152** / **338**|**0x108** / **264**|**0x108** / **264**|**0xfd** / **253**|

We can see that the `.rodata` section appears again in all cases. This happens because the compiler now knows that the pointer can´t change (it will always point to the same memory address, it's const), and the content it points to will also remain constant. Because of this, the compiler can  place the string in the .rodata section, since we can be sure that it's read-only. With that the behavior where the first print uses `puts` instead of `printf`is restored, as the content is again known and fixed. We also notice that the `.data` section returns to 0x04 in size, which means the pointer itself is stored in `.data`, while the string it points to is in `.rodata`(as explained before). 
