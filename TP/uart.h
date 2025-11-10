#ifndef UART_H

typedef unsigned int size_t;
typedef unsigned char uint8_t;

#define SPEED 115200
#define FREQ 80000000


void uart_init();
void uart_putchar(uint8_t c);
uint8_t uart_getchar();
void uart_puts(const char *s);
void uart_gets(char *s, size_t size);


#endif