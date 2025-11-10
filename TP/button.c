#include "button.h"
#include "led.h"
#include "CMSIS/Device/ST/STM32L4xx/Include/stm32l475xx.h"

void button_init(void){

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; // Enable GPIOC clock

    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk; //Set PC13 as input (00)

    SYSCFG->EXTICR[3] = (SYSCFG->EXTICR[3] & ~SYSCFG_EXTICR4_EXTI13_Msk) | SYSCFG_EXTICR4_EXTI13_PC; // Map EXTI13 to PC13 (0010). EXTICR[3] covers lines 12 to 15 

    // 1- Configure the corresponding mask bit in the EXTI_IMR register.
    EXTI->IMR1 |= EXTI_IMR1_IM13; 

    // 2- Configure the Trigger Selection bits of the Interrupt line (EXTI_RTSR and EXTI_FTSR).
    EXTI->RTSR1 &= ~EXTI_RTSR1_RT13; // Disable rising trigger
    EXTI->FTSR1 |= EXTI_FTSR1_FT13;  // Enable falling trigger

    // 3- Configure the enable and mask bits that control the NVIC IRQ channel mapped to the EXTI so that an interrupt coming from one of the EXTI lines can be correctly acknowledged.
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//override weak handler
void EXTI15_10_IRQHandler(void){
    if (EXTI->PR1 & EXTI_PR1_PIF13){ // Check if the pending flag for line 13 is set (button pressed - interrupt occurred)
        EXTI->PR1 |= EXTI_PR1_PIF13; // Clear the pending flag by writing 1 to it, otherwise the interrupt will keep firing
        led_g_button(); // Call the function to toggle the green LED
    }
}