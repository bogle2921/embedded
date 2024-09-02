#include "uart.h"
#include "stm32f7xx.h"

#define GPIODEN		(1U<<3)
#define USART3EN	(1U<<18)
#define TRANSMIT_EN	(1U<<3)

#define UART_DATA_WIDTH_8b	0x00000000U //8 bits len
#define UART_NO_PARITY		0x00000000U // disable parity
#define UART_1_STOPBITS		0x00000000U
#define BAUDRATE_SPEED		115200
#define SYS_CLOCK			200000000
#define TRANSMIT_STATUS		(1U<<7)

static void config_uart_params(USART_TypeDef *usart, uint32_t data_width, uint32_t parity, uint32_t stop_bits);
static void uart_set_baudrate(uint32_t pClk, uint32_t baudrate);
static void transmit_data(uint8_t data);

// retarget printf to write strings
int __io_putchar(int c){
	transmit_data(c);
	return c;
}

// using usart3
void init_uart_transmit(void){
	// enable clock access
	RCC->AHB1ENR |= GPIODEN;

	// PD8 to alt func mode - bit 17/16 to 10
	GPIOD->MODER &= ~(1U<<16);
	GPIOD->MODER |= (1U<<17);

	// set alt func to uart - AFR[1] bit 3-0 to 0111
	GPIOD->AFR[1] |= (1U<<0);
	GPIOD->AFR[1] |= (1U<<1);
	GPIOD->AFR[1] |= (1U<<2);
	GPIOD->AFR[1] &= ~(1U<<3);

	// enable clock access to uart3
	RCC->APB1ENR |= USART3EN;

	// config uart params
	config_uart_params(USART3, UART_DATA_WIDTH_8b, UART_NO_PARITY, UART_1_STOPBITS);

	// set transfer dir
	USART3->CR1 |= TRANSMIT_EN;

	// set baudrate
	uart_set_baudrate(SYS_CLOCK, BAUDRATE_SPEED);

	// enable uart module - bit 1 to 0
	USART3->CR1 |= (1U<<0);
}

static void transmit_data(uint8_t data){
	// make sure transmit data reg is empty - ISR bit 7
	while(!((USART3->ISR & TRANSMIT_STATUS) == TRANSMIT_STATUS)){}

	// write value to data reg - TDR
	USART3->TDR = data;
}

// usable with any uart port
static void config_uart_params(USART_TypeDef *usart, uint32_t data_width, uint32_t parity, uint32_t stop_bits){
	// pre defined functions
	MODIFY_REG(usart->CR1, USART_CR1_PS | USART_CR1_PCE | USART_CR1_M, parity | data_width);
	MODIFY_REG(usart->CR2, USART_CR2_STOP, stop_bits);
}

static uint16_t compute_uart_scalar(uint32_t clock, uint32_t baudrate){
	return ((clock + (baudrate/2U)) / baudrate);
}

static void uart_set_baudrate(uint32_t clock, uint32_t baudrate){
	USART3->BRR = compute_uart_scalar(clock, baudrate);
}
