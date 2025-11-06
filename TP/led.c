#include "led.h"
#include <stdint.h>


#define GPIOB_PIN 14 // pin number where the green led is connected
#define GPIOC_PIN 9 // pin number where the blue/yellow led is connected
#define RCC_BASE 0x40021000 // base address of the RCC registers
#define RCC_OFFSET 0x4C // offset of the RCC_AHB2ENR, first register that has GPIOXEN bits
#define GPIOB_BIT 1 // bit index to enable GPIOB clock in RCC_AHB2ENR
#define GPIOC_BIT 2 // bit index to enable GPIOC clock in RCC_AHB2ENR
#define GPIOB_BASE 0x48000400 // base address of GPIOB registers. Offset of GPIOB_MODER is 0x00
#define GPIOX_BSSR_OFFSET 0x18 // offset of the GPIOx_BSRR register
#define GPIOC_BASE 0x48000800 // base address of GPIOC registers. Offset of GPIOC_MODER is 0x00


// The compiler doesnt know the registers mapped are not RAM, so it might skip reading/writing them to optimize
// To prevent that, we use volatile pointers (the memory has to be accessed each time)
volatile uint32_t* const GPIO_CLOCK_ENABLE = (volatile uint32_t*) (RCC_BASE + RCC_OFFSET);
volatile uint32_t* const GPIOB_MODER = (volatile uint32_t*) GPIOB_BASE;
volatile uint32_t* const GPIOB_BSRR  = (volatile uint32_t*) (GPIOB_BASE + GPIOX_BSSR_OFFSET);

volatile uint32_t* const GPIOC_MODER = (volatile uint32_t*) 0x48000800;
volatile uint32_t* const GPIOC_BSRR  = (volatile uint32_t*) (GPIOC_BASE + GPIOX_BSSR_OFFSET);


//Enables the clocks. The GPIOC pin mode is set to input (off) and the GPIOB pin mode is set to output (on)
void led_init(void){
    *GPIO_CLOCK_ENABLE |= (1u << GPIOB_BIT); // mask to enable GPIOB clock
    *GPIO_CLOCK_ENABLE |= (1u << GPIOC_BIT); // mask to enable GPIOC clock
    *GPIOB_MODER = (*GPIOB_MODER & ~(3u << (2*GPIOB_PIN))) | (1u << (2*GPIOB_PIN)); // clean and set pin as output (01)
    *GPIOC_MODER &= ~(3u << (2*GPIOC_PIN)); // set pin as input (00), high impedance, initial state
}

void led_g_on(void){
    *GPIOB_BSRR = (1u << GPIOB_PIN); //set the pin (0 to 15)
}

void led_g_off(void){
    *GPIOB_BSRR = (1u << (GPIOB_PIN + 16u)); //reset the pin (16 to 31)
}

void led(state s){
    switch (s) {

        case LED_OFF:
            *GPIOC_MODER &= ~(3u << (2*GPIOC_PIN)); // set pin as input (00), high impedance
            break;
        case LED_YELLOW:
            *GPIOC_MODER |= (1u << (2*GPIOC_PIN)); //set pin as output (01) counts on LED_OFF being called before
            *GPIOC_BSRR = (1u << GPIOC_PIN); // light the pin (light on when set)
            break;
        case LED_BLUE:
            *GPIOC_MODER |= (1u << (2*GPIOC_PIN)); 
            *GPIOC_BSRR = (1u << (GPIOC_PIN + 16u)); // light the pin (light on when reset)
            break;
    }

}