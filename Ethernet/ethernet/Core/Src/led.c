#include "led.h"

#define GPIOBEN		(1U<<1)

void init_leds(void){
	// enable clk access - AHB1 -> GPIOB
	RCC->AHB1ENR |= GPIOBEN;

	// set pins to output - 01
	PORT_LED->MODER |= (1U<<0);
	PORT_LED->MODER &= ~(1U<<1);

	PORT_LED->MODER |= (1U<<14);
	PORT_LED->MODER &= ~(1U<<15);

	PORT_LED->MODER |= (1U<<28);
	PORT_LED->MODER &= ~(1U<<29);
}

void led_turn_on(uint32_t led){
	switch(led){
	// set ODR to high
	case LED_GREEN:
		PORT_LED->ODR |= LED_GREEN;
		break;
	case LED_BLUE:
		PORT_LED->ODR |= LED_BLUE;
		break;
	case LED_RED:
		PORT_LED->ODR |= LED_RED;
		break;
	default:
		break;
	}
}

void led_turn_off(uint32_t led){
	switch(led){
	// set ODR to high
	case LED_GREEN:
		PORT_LED->ODR &= ~LED_GREEN;
		break;
	case LED_BLUE:
		PORT_LED->ODR &= ~LED_BLUE;
		break;
	case LED_RED:
		PORT_LED->ODR &= ~LED_RED;
		break;
	default:
		break;
	}
}
