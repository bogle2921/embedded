// Find led
// Port: D
// Pin: 12

#define PERIPH_BASE        (0x40000000UL)
#define AHB1PERIPH_OFFSET  (0x00020000UL)
#define AHB1PERIPH_BASE    (PERIPH_BASE + AHB1PERIPH_OFFSET)
//GPIOD 0x4002 0C00
#define GPIOD_OFFSET       (0x00000C00UL)
#define GPIOD_BASE         (AHB1PERIPH_BASE + GPIOD_OFFSET)

#define RCC_OFFSET         (0x00003800UL)
#define RCC_BASE           (AHB1PERIPH_BASE + RCC_OFFSET)

#define RCC_AHB1ENR_OFFSET (0x30UL)
#define RCC_AHB1EN_R       (*(volatile unsigned int *)(RCC_BASE + RCC_AHB1ENR_OFFSET))

#define MODE_R_OFFSET      (0x00UL)
#define GPIOD_MODE_R       (*(volatile unsigned int *)(GPIOD_BASE + MODE_R_OFFSET))

#define OD_R_OFFSET        (0x14UL)
#define GPIOD_OD_R         (*(volatile unsigned int *)(GPIOD_BASE + OD_R_OFFSET))
#define GPIODEN            (1U<<3)

#define PIN12              (1U<<12)
#define LED_PIN             PIN12

// Set 24 to 1 and 25 to 0
// (1U<<24)
// &=~(1U<<25)

int main(void){
	// Enable clock access
	RCC_AHB1EN_R |= GPIODEN; // Keep everything else the same, but set 3 to 1

	// Set PD12 as output pin
    GPIOD_MODE_R |= (1U<<24);
    GPIOD_MODE_R &=~(1U<<25);
	while(1){
		// Set PD12 high
		// GPIOD_OD_R |= LED_PIN; Sets always on
		GPIOD_OD_R ^= LED_PIN; // Blink
		for(int i=0;i<100000;i++);
	}
}
