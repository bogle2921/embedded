#include "stm32f4xx.h"

#define GPIODEN            (1U<<3)
#define PIN12              (1U<<12)
#define LED_PIN             PIN12

int main(void){
	RCC->AHB1ENR |= GPIODEN;

	GPIOD->MODER |= (1U<<24);
	GPIOD->MODER &=~(1U<<25);
	while(1){
		GPIOD->ODR ^= LED_PIN;
		for(int i=0; i<100000; i++);
	}
}
