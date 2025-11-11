#include "matrix.h"
#include "CMSIS/Device/ST/STM32L4xx/Include/stm32l475xx.h"


void matrix_init(){

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN; // Enable GPIOA, GPIOB and GPIOC clocks

    GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE5_Msk) | GPIO_MODER_MODE5_0; // Set PC5 as output - SB
    GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE4_Msk) | GPIO_MODER_MODE4_0; // Set PC4 as output - LAT
    GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE3_Msk) | GPIO_MODER_MODE3_0; // Set PC3 as output - RST
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE1_Msk) | GPIO_MODER_MODE1_0; // Set PB1 as output - SCK
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE4_Msk) | GPIO_MODER_MODE4_0; // Set PA4 as output - SDA
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE2_Msk) | GPIO_MODER_MODE2_0; // Set PB2 as output - C0
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE15_Msk) | GPIO_MODER_MODE15_0; // Set PA15 as output - C1
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE2_Msk) | GPIO_MODER_MODE2_0; // Set PA2 as output - C2
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE7_Msk) | GPIO_MODER_MODE7_0; // Set PA7 as output - C3
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE6_Msk) | GPIO_MODER_MODE6_0; // Set PA6 as output - C4
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE5_Msk) | GPIO_MODER_MODE5_0; // Set PA5 as output - C5
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE0_Msk) | GPIO_MODER_MODE0_0; // Set PB0 as output - C6
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE3_Msk) | GPIO_MODER_MODE3_0; // Set PA3 as output - C7

    //Configuring speed to highest
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED4_Msk | GPIO_OSPEEDR_OSPEED15_Msk | GPIO_OSPEEDR_OSPEED2_Msk | GPIO_OSPEEDR_OSPEED7_Msk | GPIO_OSPEEDR_OSPEED6_Msk | GPIO_OSPEEDR_OSPEED5_Msk | GPIO_OSPEEDR_OSPEED3_Msk; // Set GPIOA pins to very high speed (11)
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_Msk | GPIO_OSPEEDR_OSPEED2_Msk | GPIO_OSPEEDR_OSPEED0_Msk; // Set GPIOB pins to very high speed (11)
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_Msk | GPIO_OSPEEDR_OSPEED4_Msk | GPIO_OSPEEDR_OSPEED3_Msk; // Set GPIOC pins to very high speed (11)

    RST(0); // Set RST low
    LAT(1); // Set LAT high
    init_bank0(); // Initialize bank 0 with all LEDs on
    SB(1);  // Select bank 1 
    SCK(0); // Set SCK low
    SDA(0); // Set SDA low

    deactivate_rows();

    for (int i = 0; i < 8000000; i++) asm volatile ("nop"); // Wait for 100ms (1 nop = 1 cycle, 80MHz clock = 80 million cycles per second so 1 nop = 12.5ns)

    RST(1); // Set RST high
}


// Clock pulse
void pulse_SCK(){
    SCK(0); for (int i = 0; i < 3; i++) asm volatile ("nop"); // delay 12.5ns * 3 = 37.5ns, minimum SCK low time is 25ns
    SCK(1); for (int i = 0; i < 3; i++) asm volatile ("nop"); // delay 12.5ns * 3 = 37.5ns, minimum SCK high time is 25ns
    SCK(0); for (int i = 0; i < 3; i++) asm volatile ("nop");
}

// Negative pulse to latch data
void pulse_LAT(){
    LAT(1); for (int i = 0; i < 3; i++) asm volatile ("nop"); // delay 12.5ns * 3 = 37.5ns. minimum LAT high time is 25ns
    LAT(0); asm volatile ("nop"); // delay 12.5ns * 3 = 37.5ns. minimum LAT low time is 7ns
    LAT(1); for (int i = 0; i < 3; i++) asm volatile ("nop");
}

void deactivate_rows(){
    ROW0(0); ROW1(0); ROW2(0); ROW3(0); ROW4(0); ROW5(0); ROW6(0); ROW7(0);
}

// Activate a specific row
void activate_row(int row) {
    switch (row) {
        case 0: ROW0(1); break;
        case 1: ROW1(1); break;
        case 2: ROW2(1); break;
        case 3: ROW3(1); break;
        case 4: ROW4(1); break;
        case 5: ROW5(1); break;
        case 6: ROW6(1); break;
        case 7: ROW7(1); break;
    }
}

//Send the stream of 8 bits (one byte) to the matrix, MSB first. After sending each bit, pulse the clock to shift the register
void send_byte(uint8_t val) {
    for (int i = 0; i < 8; i++) {
        SDA(val & (1 << (7 - i))); //get the MSB, then MSB-1, MSB-2... until sending all 8 bits
        pulse_SCK();
    }
}


void mat_set_row(int row, const rgb_color *val){

    for (int i=7; i>=0; i--){ 
        send_byte(val[i].b); // Send blue component
        send_byte(val[i].g); // Send green component
        send_byte(val[i].r); // Send red component
    }

    pulse_LAT(); // Latch the data
    activate_row(row); // Activate the specified row
}

void init_bank0(){
    SB(0); //Choose bank 0
    for (int i = 0; i<24; i++){
        send_byte(0xFF); // Send 24 bytes of 0xFF (1111 1111) to set all 8 bank0 LEDs to 1
    }
    pulse_LAT(); // Latch the data
}

// Write B E A letter by letter, each in a different color (Blue, Green, Red), first letters of my name (Beatriz)
void test_pixels(){

    //bitmaps
     const uint8_t GLYPH_B[8] = {
        0b00000000,  0b11111100, 0b10000010, 0b10000010,  //col 1, col 2, col 3, col 4
        0b11111100,  0b10000010, 0b10000010, 0b11111100   //col 5, col 6, col 7, col 8
    };

    const uint8_t GLYPH_E[8] = {
        0b00000000,  0b11111110, 0b10000000, 0b10000000,
        0b10000000,  0b11111000, 0b10000000, 0b11111110
    };

    const uint8_t GLYPH_A[8] = {
        0b00000000,  0b10000010, 0b10000010, 0b11111110,
        0b10000010,  0b10000010, 0b01000100, 0b00111000
    };

    show_letter(GLYPH_B, 0,0, 255);
    show_letter(GLYPH_E, 0, 255,0);
    show_letter(GLYPH_A, 255, 0,0);
 
}

static void show_letter(const uint8_t glyph[8], uint8_t R, uint8_t G, uint8_t B){
    rgb_color row_pixels[8];

    for (int frame = 0; frame < 300; ++frame) {              // how long to show this letter 
        for (int row = 0; row < 8; ++row) {                  // for each row
            int bit_index = 7 - row;                         // bit position for this row in the bitmap stream
            uint8_t row_mask = (uint8_t)(1u << bit_index);   // mask selecting this row inside a column

            for (int col = 0; col < 8; ++col) {              // for each column in this row
                uint8_t pixel_on = (glyph[col] & row_mask) ? 1u : 0u; // check if this pixel is on or off
                row_pixels[col].r = pixel_on ? R : 0;        
                row_pixels[col].g = pixel_on ? G : 0;
                row_pixels[col].b = pixel_on ? B : 0;
            }

            mat_set_row(row, row_pixels);                    // push this row to the matrix
            for (int n = 0; n < 800; ++n) __asm volatile ("nop"); //delay for persistence of vision
           deactivate_rows(); // deactivate rows before the next iteration
        }
    }
}


// Show a color gradient row by row, first red, then green, then blue
void test_pixels2(void)
{
    rgb_color row_pixels[8];

    // color = 0 (R), 1 (G), 2 (B)
    for (int color = 0; color < 3; ++color) {
        for (int row = 0; row < 8; ++row) { // show one row at a time
            for (int hold = 0; hold < 1000; ++hold) { // hold this row for a while
                for (int col = 0; col < 8; ++col) {
                    
                    uint8_t intensity = (uint8_t)((col * col * 255) / (7 * 7)); // quadratic intensity ramp from 0 to 255

                    if (color == 0) {       // red row
                        row_pixels[col].r = intensity; row_pixels[col].g = 0; row_pixels[col].b = 0;
                    } else if (color == 1) { // green row
                        row_pixels[col].r = 0; row_pixels[col].g = intensity; row_pixels[col].b = 0;
                    } else {              // blue row
                        row_pixels[col].r = 0; row_pixels[col].g = 0; row_pixels[col].b = intensity;
                    }
                }


                mat_set_row(row, row_pixels); // push this row to the matrix

                for (volatile int d = 0; d < 600; ++d) __asm volatile ("nop"); // delay for persistence of vision
                deactivate_rows();
            }
        }
    }
}

void test_display(const unsigned char *scr){ //similar to the letter test function, but now the image or uart live transmission is passed as a parameter

    rgb_color row_pixels[8];

    for (int row = 0; row < 8; ++row) {                  // for each row
        for (int col = 0; col < 8; ++col) {              // for each column in this row
            // extract RGB from the image (position = column in the row (row*8 +col) times the number of colors plus position of the color)
            row_pixels[col].r = scr[(row*8 + col)*3 + 0]; 
            row_pixels[col].g = scr[(row*8 + col)*3 + 1]; 
            row_pixels[col].b = scr[(row*8 + col)*3 + 2];
        }

        mat_set_row(row, row_pixels);                    // push this row to the matrix
        for (int n = 0; n < 800; ++n) __asm volatile ("nop"); //delay for persistence of vision
        deactivate_rows(); // deactivate rows before the next iteration
    }
}
