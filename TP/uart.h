#ifndef UART_H

typedef unsigned int size_t;
typedef unsigned char uint8_t;

#define FREQ 80000000
#define BYTES 192 // 8x8 matrix with RGB = 8*8*3 = 192 bytes


void uart_init(int baudrate);
void uart_putchar(uint8_t c);
uint8_t uart_getchar();
void uart_puts(const char *s);
void uart_gets(char *s, size_t size);
void USART1_IRQHandler(void);


#endif