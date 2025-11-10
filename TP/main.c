#include "clocks.h"
#include "matrix.h"

extern const uint8_t  _binary_image_raw_start[];


void main()
{   
    clocks_init();
    matrix_init();
    
    test_image(_binary_image_raw_start);
    
    
    
}