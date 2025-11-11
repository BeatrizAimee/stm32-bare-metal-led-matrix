#include "clocks.h"
#include "irq.h"
#include "led.h"
#include "matrix.h"
#include "button.h"
#include "uart.h"
#include "timer.h"


volatile rgb_color g_frame[BYTES];  


int main(){   

    clocks_init();
    uart_init(38400);
    led_init();
    matrix_init();
    irq_init();
    button_init();
    timer_init(1000000/60); // Refresh at 60 Hz
    

    while (1){
        // Main loop does nothing, all work is done in interrupts
    }

    return 0;
}
