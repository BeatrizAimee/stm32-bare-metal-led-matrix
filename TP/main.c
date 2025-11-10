#include "led.h"
#include "clocks.h"
#include "matrix.h"


void main()
{   
    clocks_init();
    matrix_init();

    test_pixels();
    test_pixels2();
}