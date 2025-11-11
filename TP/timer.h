#ifndef TIMER_H


#define FREQ 80000000 // 80 MHz timer frequency
#define FREQ_TIMER 1000000 // 1 MHz timer frequency 
#define BYTES 192 // 8x8 matrix with RGB = 8*8*3 = 192 bytes

void timer_init(int max_us);
void TIM2_IRQHandler(void);


#endif