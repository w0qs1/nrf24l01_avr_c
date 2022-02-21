#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "nrf24l01p.h"
#include "uart.h"

#define UART_BAUDRATE 9600

uint8_t rxaddress[] = {0x78, 0x78, 0x78, 0x78, 0x78};
uint8_t rxdata[32];

void uart_putstring(uint8_t data[]) {
	uint8_t l = 0;
	while(data[l] != '\0') {
		l++;
	}

	for (uint8_t i = 0; i < l; i++) {
		uart_putc(data[i]);
		_delay_ms(1);
	}
}

int main(void) {
    uart_init(UART_BAUD_SELECT(UART_BAUDRATE, F_CPU));
    sei();

    DDRD |= (1 << 2);
    nrf24_init();
    nrf24_setuprx(rxaddress, 110);

    while(1) {
        if(is_data_available(1)) {
            nrf24_receive(&rxdata[0]);
            uart_putstring(rxdata);
            PORTD |= (1 << 2);
            _delay_us(500);
        }
        PORTD &= ~(1 << 2);
        _delay_us(500);
    }
}
