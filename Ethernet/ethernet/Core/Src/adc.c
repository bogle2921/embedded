#include "adc.h"

#define GPIOAEN		(1U<<0)
#define ADC1EN		(1U<<8)
#define CONT_MODE	(1U<<1)
#define ADON		(1U<<0)
#define SEQ_LEN_1	0
#define CHNNL_RANK1	(1U<<2)
#define START_CONV  (1<<30)
#define CONV_COMP	(1U<<1)

// PA4
void init_adc1(void){
	// enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	// set pin as analog - bit 9-8 to 11
	GPIOA->MODER |= (1U<<8);
	GPIOA->MODER |= (1U<<9);

	// enable clock access to adc module - bit 8
	RCC->APB2ENR |= ADC1EN;

	// set adc mode to continuous conversion
	ADC1->CR2 |= CONT_MODE;

	// set seq len to 1 - bit 23-20 to 0000 - whole register is irrelevant at this time
	ADC1->SQR1 = SEQ_LEN_1;

	// set adc channel - SQR3
	ADC1->SQR3 |= CHNNL_RANK1;

	// enable adc module
	ADC1->CR2 |= ADON;

	// start conversion
	ADC1->CR2 |= START_CONV;
}

uint32_t get_adc_data(void){
	// EOC bit 1
	while(!(ADC1->SR & CONV_COMP)){}

	return (ADC1->DR);
}
