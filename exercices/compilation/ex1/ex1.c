#include <stdio.h>
#include <stdlib.h>

//global inicialized variable goes to: .data
int global_var = 42;
//global uninitialized variable goes to: .bss
int global_uninit_var;
//global const variable goes to: .rodata and .text
const int global_const_var = 142;


//function goes to: .text
void function(const int* param) { //pointer to a const variable goes to: stack
   int local_var = 21; //local initialized variable goes to: stack

   printf("Function param (comes from main): %p\n", (void*)&param); //prints address of the parameter 
   printf("Local variable:  %p\n", (void*)&local_var); //void used to tell local var can be any type

  
}


int main() {
   


    int* dynamic_varA = (int*) malloc(sizeof(int)); //dynamic variable (malloc) goes to: heap
    int* dynamic_varB = (int*) malloc(sizeof(int)); 
    

    int var_to_function = 84; //local initialized variable goes to: stack
    function(&var_to_function);


    //print where each section is located in memory
    printf("Address of global initialized variable (.data):    %p\n", (void*)&global_var);
    printf("Address of global uninitialized variable (.bss):   %p\n", (void*)&global_uninit_var);
    printf("Address of global const variable (.rodata):        %p\n", (void*)&global_const_var);
    printf("Address of func (.text):                           %p\n", (void*)&function); 
    printf("Address of heap variable A:                        %p\n", (void*)dynamic_varA);
    printf("Address of heap variable B:                        %p\n", (void*)dynamic_varB);

    free(dynamic_varA);
    free(dynamic_varB);
  
    return 0;
}

