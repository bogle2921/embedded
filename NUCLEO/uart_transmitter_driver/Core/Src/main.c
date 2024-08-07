#include <stdint.h>
#include "stm32f4xx.h"

#define GPIOAEN     (1U<<0)
#define UART2EN     (1U<<17)
#define SYS_FREQ    16000000
#define APB1_CLOCK  SYS_FREQ
#define BAUDRATE    115200
#define CR1_TE     (1U<<3)
#define CR1_UE     (1U<<13)
#define SR_TXE     (1U<<7)

static void set_baudrate(USART_TypeDef *usartx, uint32_t clock, uint32_t baudrate);
static uint16_t compute_baudrate_freq(uint32_t clock, uint32_t baudrate);
void uart2tx_init(void);
void uart2_write(int data);

int main(void){

	uart2tx_init();
	while(1){
		uart2_write('Y');
	}
}

void uart2tx_init(void){
	// *** configure uart gpio pin***
	// enable clock access to gpioa
	RCC->AHB1ENR |= GPIOAEN;

	// set PA2 mode to alternate function mode - reg. 5 4 -> 1 0
	GPIOA->MODER &=~(1U<<4); // set bit 4 to 0
	GPIOA->MODER |= (1U<<5); // 5 to 1

	// set PA2 alternate function type to uart_tx - AF07
	//AFRL -> index 0 and AFRH -> index 1
    GPIOA->AFR[0] |= (1U<<8);
    GPIOA->AFR[0] |= (1U<<9);
    GPIOA->AFR[0] |= (1U<<10);
    GPIOA->AFR[0] &=~(1U<<11);

	//*** configure uart module***
	// enable clock access to uart2
    RCC->APB1ENR |= UART2EN;

	// configure baudrate
    set_baudrate(USART2, APB1_CLOCK, BAUDRATE);
	// configure transfer direction
    USART2->CR1 = CR1_TE; //set directly to clean everything else to 0
    // enable module
    USART2->CR1 |= CR1_UE;
}

void uart2_write(int data){
	// transmit data register must be empty
    while (!(USART2->SR & SR_TXE)) {}
	// write to transmit data register
	USART2->DR = (data & 0xFF);
}

static void set_baudrate(USART_TypeDef *usartx, uint32_t clock, uint32_t baudrate){
	usartx->BRR = compute_baudrate_freq(clock, baudrate);
}

static uint16_t compute_baudrate_freq(uint32_t clock, uint32_t baudrate){
	return ((clock + (baudrate/2U)) / baudrate);
}
