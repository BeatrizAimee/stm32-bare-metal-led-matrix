#include "led.h"
#include "clocks.h"
#include "uart.h"


void uart_test_letter(){
    //test one: sending a letter to the PC
    uart_putchar('B');
    uart_putchar('\r'); 
    uart_putchar('\n');
}

void uart_test_string(){
    //test two: sending a string to the PC
    uart_puts("Hello World!");
}

void uart_test_echo(){
    char buffer[100];
    //test three: echoing characters received from the PC
    uart_puts("Echo: type a char");
    uint8_t c = uart_getchar();
    uart_puts("You typed: ");
    uart_putchar(c);  
    uart_puts("");
    
    uart_puts("Now type a string and press Enter:");
    uart_gets(buffer, 100);
    uart_puts("You typed:");
    uart_puts(buffer);
  
}

void sum(){
    unsigned int total = 0;
    unsigned int counter = 0;

    uart_puts("Starting to receive bytes and calculate sum...");

    // Receive 1000 bytes (default set by checksum.py) and calculate the sum
    while(counter < 1000){
        total += uart_getchar();
        counter++;
    }
    uart_puts("Finished summing! You can check the total!"); 
}


void main()
{   
    clocks_init();
    led_init();
    uart_init();

    switch (4)
    {
    case 1:
        uart_test_letter();
        break;
    case 2:
        uart_test_string();
        break;
    case 3:
        uart_test_echo();
        break;
    case 4:
        sum();
        break;

    default:
        break;
    }
    
    
}