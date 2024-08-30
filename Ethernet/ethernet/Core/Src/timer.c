#include <stdint.h>

#include "main.h"
#include "timer.h"

// Values taken from ioc to provide 200MHz clock
#define PLL_M	12
#define PLL_N	192
#define PLL_P	2

#define HSEON	(1U<<16)
#define HSE_RDY	(1U<<17)
uint32_t wait_counter;
uint32_t hse_status;

void config_clock(void){
	wait_counter = 0;
	hse_status = 0;
	// enable HSE
	RCC->CR |= HSEON;

	// wait for HSE to be ready
	do {
		hse_status = ((RCC->CR) & HSE_RDY);
		wait_counter++;
	} while ((hse_status == 0) && (wait_counter != 5000));


}
