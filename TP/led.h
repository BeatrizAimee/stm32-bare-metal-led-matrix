#ifndef LED_H

typedef enum {LED_OFF, LED_YELLOW, LED_BLUE} state;

void led_init(void);
void led_g_on(void);
void led_g_off(void);
void led_g_button(void);
void led(state);

#endif