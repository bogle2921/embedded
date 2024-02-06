// Find led
// Port: D
// Pin: 12

#include <stdint.h>

#define PERIPH_BASE        (0x40000000UL)
#define AHB1PERIPH_OFFSET  (0x00020000UL)
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)
//GPIOD 0x4002 0C00
#define GPIOD_OFFSET       (0x00000C00UL)
#define GPIOD_BASE         (AHB1PERIPH_BASE + GPIOD_OFFSET)

#define RCC_OFFSET         (0x00003800UL)
#define RCC_BASE           (AHB1PERIPH_BASE + RCC_OFFSET)

#define GPIODEN            (1U<<3)

#define PIN12              (1U<<12)
#define LED_PIN             PIN12

// Each field is 4 bytes which corresponds to the addr offset
typedef struct {
	volatile uint32_t MODER;         // GPIO port mode register        Addr offset: 0x0
	volatile uint32_t DUMMY_DATA[4]; // Takes the same amount of space as the registers we are not using
	volatile uint32_t ODR;           // GPIO port output data register Addr offset: 0x14

} GPIO_TypeDef;

typedef struct {
	volatile uint32_t DUMMY_DATA[12]; // Takes the same amount of space as the registers we are not using
	volatile uint32_t AHB1ENR;        // RCC AHB1 periph clock register Addr offset: 0x30
} RCC_TypeDef;

#define RCC                ((RCC_TypeDef*) RCC_BASE)
#define GPIOD              ((GPIO_TypeDef*) GPIOD_BASE)

int main(void){
	// Enable clock access
    RCC->AHB1ENR |= GPIODEN;

    GPIOD->MODER |= (1U<<24);
    GPIOD->MODER &=~(1U<<25);
	while(1){
		// Set PD12 high
		GPIOD->ODR ^= LED_PIN;
		for(int i=0;i<100000;i++);
	}
}
