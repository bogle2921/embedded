#include "eth_driver.h"

void init_eth_gpio(void){
	// enable clock access to ethernet module - AHB1ENR bit 25
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACEN;

	// enable clock access to eth MAC TX - bit 26
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACTXEN;

	// enable clock access to eth MAC RX
	RCC->AHB1ENR |= RCC_AHB1ENR_ETHMACRXEN;

	// enable clock access to eth gpio pin ports - GPIOC A B G
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	// set pins to alt func mode - 10
	// PC 1 4 5
	GPIOC->MODER &= ~(1U<<2);
	GPIOC->MODER |= (1U<<3);

	GPIOC->MODER &= ~(1U<<8);
	GPIOC->MODER |= (1U<<9);

	GPIOC->MODER &= ~(1U<<10);
	GPIOC->MODER |= (1U<<11);

	// PA 1 2 7
	GPIOA->MODER &= ~(1U<<2);
	GPIOA->MODER |= (1U<<3);

	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	GPIOA->MODER &= ~(1U<<14);
	GPIOA->MODER |= (1U<<15);

	//PB13
	GPIOB->MODER &= ~(1U<<26);
	GPIOB->MODER |= (1U<<27);

	//PG11 13
	GPIOG->MODER &= ~(1U<<22);
	GPIOG->MODER |= (1U<<23);

	GPIOB->MODER &= ~(1U<<26);
	GPIOB->MODER |= (1U<<27);

	// set alt func type to ethernet - 0100
	// PC 1 4 5
	GPIOC->AFR[0] |= (1U<<4);
	GPIOC->AFR[0] |= (1U<<5);
	GPIOC->AFR[0] &= ~(1U<<6);
	GPIOC->AFR[0] |= (1U<<7);

	GPIOC->AFR[0] |= (1U<<16);
	GPIOC->AFR[0] |= (1U<<17);
	GPIOC->AFR[0] &= ~(1U<<18);
	GPIOC->AFR[0] |= (1U<<19);

	GPIOC->AFR[0] |= (1U<<20);
	GPIOC->AFR[0] |= (1U<<21);
	GPIOC->AFR[0] &= ~(1U<<22);
	GPIOC->AFR[0] |= (1U<<23);

	// PA 1 2 7
	GPIOA->AFR[0] |= (1U<<4);
	GPIOA->AFR[0] |= (1U<<5);
	GPIOA->AFR[0] &= ~(1U<<6);
	GPIOA->AFR[0] |= (1U<<7);

	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] &= ~(1U<<10);
	GPIOA->AFR[0] |= (1U<<11);

	GPIOA->AFR[0] |= (1U<<28);
	GPIOA->AFR[0] |= (1U<<29);
	GPIOA->AFR[0] &= ~(1U<<30);
	GPIOA->AFR[0] |= (1U<<31);

	//PB13
	GPIOB->AFR[1] |= (1U<<20);
	GPIOB->AFR[1] |= (1U<<21);
	GPIOB->AFR[1] &= ~(1U<<22);
	GPIOB->AFR[1] |= (1U<<23);

	//PG11 13
	GPIOG->AFR[1] |= (1U<<12);
	GPIOG->AFR[1] |= (1U<<13);
	GPIOG->AFR[1] &= ~(1U<<14);
	GPIOG->AFR[1] |= (1U<<15);

	GPIOG->AFR[1] |= (1U<<20);
	GPIOG->AFR[1] |= (1U<<21);
	GPIOG->AFR[1] &= ~(1U<<22);
	GPIOG->AFR[1] |= (1U<<23);
}
