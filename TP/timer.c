#include "timer.h"
#include "uart.h"
#include "matrix.h"
#include "led.h"
#include "CMSIS/Device/ST/STM32L4xx/Include/stm32l475xx.h"

extern volatile rgb_color g_frame[BYTES];

void timer_init(int max_us){

    TIM2->CR1 &= ~TIM_CR1_CEN; // Disables TIM2 to configure it

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable TIM2 clock

    TIM2->SR = 0; // Clear status register
    TIM2->CNT = 0; // Set counter to 0

    TIM2->CR1 &= ~(TIM_CR1_DIR|TIM_CR1_CMS|TIM_CR1_CKD|TIM_CR1_OPM); // Set up for upcounting, edge-aligned mode, clock division = 1, not one-pulse mode

    TIM2->PSC = (FREQ / FREQ_TIMER) - 1; // Set prescaler to have the counter changing at 1 MHz (1 tick per microsecond)

    TIM2->ARR = max_us; // Set auto-reload, maximum counter value

    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt

    NVIC_SetPriority(TIM2_IRQn, 1); // Set TIM2 interrupt priority, the lower the number, the higher the priority
    NVIC_EnableIRQ(TIM2_IRQn); // Enable TIM2 interrupt in NVIC

    TIM2->CR1 |= TIM_CR1_CEN; // Enables TIM2 after configuration is done

}


void TIM2_IRQHandler(void){
    TIM2->SR = 0; // Clears the update interrupt flag
    test_display((const unsigned char*)g_frame); // Refresh the display
    led_g_button(); //toggle the green LED

}
