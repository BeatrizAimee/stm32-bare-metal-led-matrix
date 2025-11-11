#include "clocks.h"
#include "irq.h"
#include "led.h"
#include "matrix.h"
#include "button.h"
#include "uart.h"


volatile rgb_color g_frame[BYTES];  


int main(){   

    clocks_init();
    uart_init(38400);
    led_init();
    matrix_init();
    irq_init();
    button_init();
    

    while (1){
        test_display((const unsigned char*)g_frame);
    }

    return 0;
}
