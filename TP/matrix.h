#ifndef MATRIX_H
#include <stdint.h>

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} rgb_color;


#define RST(x) (x ? (GPIOC->BSRR = GPIO_BSRR_BS3) : (GPIOC->BSRR = GPIO_BSRR_BR3))
#define LAT(x) (x ? (GPIOC->BSRR = GPIO_BSRR_BS4) : (GPIOC->BSRR = GPIO_BSRR_BR4))
#define SB(x)  (x ? (GPIOC->BSRR = GPIO_BSRR_BS5) : (GPIOC->BSRR = GPIO_BSRR_BR5))  
#define SCK(x) (x ? (GPIOB->BSRR = GPIO_BSRR_BS1) : (GPIOB->BSRR = GPIO_BSRR_BR1))
#define SDA(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS4) : (GPIOA->BSRR = GPIO_BSRR_BR4))

#define ROW0(x) (x ? (GPIOB->BSRR = GPIO_BSRR_BS2) : (GPIOB->BSRR = GPIO_BSRR_BR2))
#define ROW1(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS15) : (GPIOA->BSRR = GPIO_BSRR_BR15))
#define ROW2(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS2) : (GPIOA->BSRR = GPIO_BSRR_BR2))
#define ROW3(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS7) : (GPIOA->BSRR = GPIO_BSRR_BR7))
#define ROW4(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS6) : (GPIOA->BSRR = GPIO_BSRR_BR6))
#define ROW5(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS5) : (GPIOA->BSRR = GPIO_BSRR_BR5))
#define ROW6(x) (x ? (GPIOB->BSRR = GPIO_BSRR_BS0) : (GPIOB->BSRR = GPIO_BSRR_BR0))
#define ROW7(x) (x ? (GPIOA->BSRR = GPIO_BSRR_BS3) : (GPIOA->BSRR = GPIO_BSRR_BR3))


void matrix_init();
void pulse_SCK();
void pulse_LAT();
void deactivate_rows();
void activate_row(int row);
void send_byte(uint8_t val);
void mat_set_row(int row, const rgb_color *val);
void init_bank0();
void test_pixels();
void test_pixels2();
static void show_letter(const uint8_t glyph[8], uint8_t R, uint8_t G, uint8_t B);

void test_display(const unsigned char *scr);

#endif