#ifndef LED_H
#define LED_H

#include "main.h"

#define LED_1			(1U<<0)  // Green
#define LED_GREEN		LED_1

#define LED_2			(1U<<7)  // Blue
#define LED_BLUE		LED_2

#define LED_3			(1U<<14) // Red
#define LED_RED			LED_3

#define PORT_LED		GPIOB

void init_leds(void);
void led_turn_on(uint32_t led);
void led_turn_off(uint32_t led);

#endif
