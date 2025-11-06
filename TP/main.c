#include "led.h"

void main()
{   

    led_init();

    while(1){
        led_g_on();

        for(int i = 0; i < 99999; i++){ //delay
            asm volatile("nop");
        }

        led_g_off();

        led(LED_YELLOW);

        for(int i = 0; i < 99999; i++){ //delay
            asm volatile("nop");
        }

        led(LED_OFF);
        led(LED_BLUE);

        for(int i = 0; i < 99999; i++){ //delay
            asm volatile("nop");
        }

        led(LED_OFF);

    }
    
    
}