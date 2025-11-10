#include "clocks.h"
#include "irq.h"
#include "led.h"
#include "matrix.h"
#include "button.h"

extern const uint8_t  _binary_image_raw_start[];


void main(){   
    clocks_init();
    led_init();
    matrix_init();
    irq_init();
    button_init();


    test_image(_binary_image_raw_start); //See if the press of the button doesn't interfere with the matrix display
}
