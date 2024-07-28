#include "stm32f4xx.h"

//Button
#define GPIOAEN            (1U<<0)
#define PIN0			   (1U<<0)

//LED
#define GPIODEN            (1U<<3)
#define PIN12              (1U<<12)

#define LED_PIN             PIN12
#define BTN_PIN				PIN0

int main(void){

	//Enable clock access to GPIOA and GPIOD
	RCC->AHB1ENR |= GPIODEN;
	RCC->AHB1ENR |= GPIOAEN;

	//Set PD12 as output pin
	GPIOD->MODER |= (1U<<24);
	GPIOD->MODER &=~(1U<<25);

	//Set PA0 as input pin
	GPIOA->MODER &=~(1U<<0);
	GPIOA->MODER &=~(1U<<1);

	while(1){
		//Check if btn is pressed
		if(GPIOA->IDR  & BTN_PIN){
			GPIOD->BSRR = LED_PIN;
		} else {
			GPIOD->BSRR = (1U<<28);
		}
	}
}
