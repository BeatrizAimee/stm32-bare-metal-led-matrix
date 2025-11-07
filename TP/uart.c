#include "CMSIS/Device/ST/STM32L4xx/Include/stm32l475xx.h"
#include "uart.h"


void uart_init(){

    USART1->CR1 &= ~USART_CR1_UE; // (UE (USART enable)=0) to configure USART1

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enable USART1 clock

    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE6_Msk) | GPIO_MODER_MODE6_1; //clear and then set PB6 (Tx) to alternate function 10 (UART) 
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE7_Msk) | GPIO_MODER_MODE7_1; //clear and then set PB6 (Rx) to alternate function 10 (UART) 

    //we write 0111 (7) in the AFRL6 and AFRL7 fields to select AF7 (USART1) for both pins
    //to that, we set the 4bit fields to 1111, then clear the top bit -> 0111
    GPIOB->AFR[0] |=  (GPIO_AFRL_AFSEL6_Msk | GPIO_AFRL_AFSEL7_Msk); // GPIOB->AFR[0] is AFRL (pins 0-7) and GPIOB->AFR[1] is AFRH (pins 8-15)
    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_3   | GPIO_AFRL_AFSEL7_3);    

    RCC->CCIPR &= ~RCC_CCIPR_USART1SEL_Msk; // select PCLK as USART1 clock source (00)

    RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST; //reset USART1
    RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST; //release reset

    //BRR = clock / baudrate = 80 000 000 Hz / 115200 bps = 694.444
    USART1->BRR = 694u;

    USART1->CR1 &= ~USART_CR1_OVER8; // 0 = oversampling by 16
    USART1->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1); // bits M0 and M1 00 = 8 data bits
    USART1->CR1 &= ~USART_CR1_PCE; // PCE (parity control enable) = 0 -> no parity (N)
    USART1->CR2 &= ~USART_CR2_STOP; // STOP bits 00 = 1 stop bit


    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable transmitter, receiver and USART

}

void uart_putchar(uint8_t c){
    //condition for the USART be ready to transmit: TXE (TDR empty) = 1, safe to write new data
    while ((USART1->ISR & USART_ISR_TXE)==0); //wait until TXE=1
    USART1->TDR = c; //write data to transmit
}

uint8_t uart_getchar(){
    //condition for the USART to have received data: RXNE (RDR (Receive Data Register) not empty) = 1, data ready to be read
    while ((USART1->ISR & USART_ISR_RXNE)==0); //wait until RXNE=1
    return (uint8_t)(USART1->RDR); //read received data
}


//puts takes a c string (terminated with '\0') and prints all char, then appends a '\n' at the end
void uart_puts(const char *s){
    if (!s) return; 
    
    while (*s != '\0') { //send each char until null terminator
        uart_putchar((uint8_t)*s);
        s++;
    }
    uart_putchar((uint8_t)'\r'); uart_putchar((uint8_t)'\n');
}


//fgets reads chars from a stream into a buffer and stops when a "\n" is read, the buffer is full. theen writes a '\0' at the end 
void uart_gets(char *s, size_t size){

    if (size == 0 || !s ) return; 

    while (size-1){  //while there is space in the buffer (size-1 to leave space for '\0')
    
        *s = uart_getchar();
        if (*s == '\n' || *s == '\r') break; //read until newline
        s++; //advance buffer pointer
        size --; 
    }
    *s='\0';
}