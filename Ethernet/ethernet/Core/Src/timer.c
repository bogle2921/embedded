#include "main.h"
#include "timer.h"

// Values taken from ioc to provide 200MHz clock
// Sys Clock (HCLK) = 200MHz
// APB1 Clock (PCLK1) = 50MHz
// APB2 Clock (PCLK2) = 100MHz
#define PLLM	4
#define PLLN	200
#define PLLP	2

#define HSE_ON	(1U<<16)
#define HSE_RDY	(1U<<17)
#define PWR_EN	(1U<<28)

#define CYCLES_PER_MS	200000
#define CTRL_EN			(1U<<0)
#define CTRL_TICKINT	(1U<<1)
#define CTRL_CLKSRC		(1U<<2)
#define CTRL_CNTFLAG	(1U<<16)

#define MAX_DELAY		0xFFFFFFFFU //max 32 bits
uint32_t wait_counter;
__IO uint32_t hse_status;

uint32_t curr_tick;
uint32_t tick_freq = 1;

void increment_tick(void){
	curr_tick += tick_freq;
}

uint32_t get_tick(void){
	return curr_tick;
}

void init_timebase(void){
	// reload timer with num clocks per ms
	// 200MHz = 200M cycles per sec = 200T cycles per ms
	SysTick->LOAD = CYCLES_PER_MS - 1;

	// clear SysTick curr value reg
	SysTick->VAL = 0;

	// enable systick and select internal clock src
	// bit 0 - 1 enable counter
	// bit 1 - 1 enable tickint
	// bit 2 - 1 processor clock
	// bit 16 - count flag
	SysTick->CTRL = CTRL_EN | CTRL_TICKINT | CTRL_CLKSRC;
}

void delay(uint32_t d_ms){
	uint32_t start = get_tick();

	if(d_ms < MAX_DELAY){
		d_ms += (uint32_t)(tick_freq);
	}

	while((get_tick() - start) < d_ms){}
}
void SysTick_Handler(void){
	increment_tick();
}

uint8_t config_clock(void){
	wait_counter = 0;
	hse_status = 0;
	// enable HSE
	RCC->CR |= HSE_ON;

	// wait for HSE to be ready
	do {
		hse_status = ((RCC->CR) & HSE_RDY);
		wait_counter++;
	} while ((hse_status == 0) && (wait_counter != 5000));

	if(hse_status){
		// enable pwr module apb1 -> bit 28
		RCC->APB1ENR |= PWR_EN;
		// configure voltage regulator -> bit 15/14 VOS bits - Scale 1 mode
		PWR->CR1 &= ~(1U<<14);
		PWR->CR1 &= ~(1U<<15);

		//Set AHB prescaler to 1 aka doesnt divide (SYSCLK == HCLK) -> bit 7-4 = 0xxx
		RCC->CFGR &= ~(1U<<7);

		// Set PCLK1 prescaler to 4 - ABP1 CLK max 54MHz bit 12-10 = 101 => Sets PLCK1 = MHz
		PWR->CR1 |= (1U<<10);
		PWR->CR1 &= ~(1U<<11);
		PWR->CR1 |= (1U<<12);

		// Set PCLK2 prescaler to 2 - ABP2 CLK max 108MHz bit 15-13 = 100 => Sets PLCK2 = 100MHz
		PWR->CR1 &= ~(1U<<13);
		PWR->CR1 &= ~(1U<<14);
		PWR->CR1 |= (1U<<15);

		// configure PLL params
		// algorithm taken from docs - finds positions and shifts as needed with given PLL params to provide 200MHz
		RCC->PLLCFGR = PLLM | (PLLN << RCC_PLLCFGR_PLLN_Pos) | (((PLLP >> 1) - 1) << RCC_PLLCFGR_PLLP_Pos) | (RCC_PLLCFGR_PLLSRC_HSE);

		// enable PLL
		RCC->CR |= RCC_CR_PLLON;

		// wait until PLL is locked - bit 25
		while((RCC->CR & RCC_CR_PLLRDY) == 0){}

		// set flash latency to 7 wait states
		FLASH->ACR = FLASH_ACR_LATENCY_7WS;

		// make PLL sys clock - system clock switch bit 1-0 = 10
		RCC->CFGR &= ~(1U<<0);
		RCC->CFGR |= (1U<<1);

		// wait until PLL is selected bit 3-2 = 10
		while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){}

		return 1;
	}
	// hse_status was not ready
	return 0;
}
