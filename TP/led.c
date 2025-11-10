#include "led.h"
#include "CMSIS/Device/ST/STM32L4xx/Include/stm32l475xx.h"



//Enables the clocks. The GPIOC pin mode is set to input (off) and the GPIOB pin mode is set to output (on)
void led_init(void){
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // mask to enable GPIOB clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // mask to enable GPIOC clock
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE14_Msk) | GPIO_MODER_MODE14_0; // clean and set pin as output (0)
    GPIOC->MODER &= ~GPIO_MODER_MODE9_Msk; // set pin as input, initial state
}

void led_g_on(void){
    GPIOB->BSRR = GPIO_BSRR_BS14; //set the pin 
}

void led_g_off(void){
    GPIOB->BSRR = GPIO_BSRR_BR14; //reset the pin 
}

void led(state s){
    switch (s) {

        case LED_OFF:
             GPIOC->MODER &= ~GPIO_MODER_MODE9_Msk; // set pin as input (00), high impedance
            break;
        case LED_YELLOW:
            GPIOC->MODER |= GPIO_MODER_MODE9_0; //set pin as output (01) counts on LED_OFF being called before
            GPIOC->BSRR = GPIO_BSRR_BS9; // light the pin (light on when set)
            break;
        case LED_BLUE:
            GPIOC->MODER |= GPIO_MODER_MODE9_0;
            GPIOC->BSRR = GPIO_BSRR_BR9; // light the pin (light on when reset)
            break;
    }

}