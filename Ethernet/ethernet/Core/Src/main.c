#include "timer.h"
#include "led.h"
#include "uart.h"
#include "adc.h"

uint32_t sensor_data;

int main(void) {

	config_clock();
	init_timebase();
	init_leds();
	init_uart_transmit();
	init_adc1();

	while(1){
		printf("Hello from board\n\r");
		sensor_data = get_adc_data();
	}
	return 0;
}

