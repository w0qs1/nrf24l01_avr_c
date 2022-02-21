#include "spi.h"

void spi_init(uint8_t sck_speed_divide, uint8_t cpol, uint8_t cpha) {
    DDRB = (1 << SCK) | (1 << MOSI) | (1 << SS);    // Set outputs
    
    SPSR = ((sck_speed_divide & 0x4) >> 2);         // Set double speed

    // SPI enable, Master mode, Set Clock polarity, phase and rate
    SPCR = (1 << SPE) | (1 << MSTR) | ((cpol & 1) << CPOL) | ((cpha & 1) << CPHA) | (sck_speed_divide & 0x3);
}

uint8_t spi_tranceive(uint8_t data) {
    SPDR = data;                                    // Put the data in the Data Register
    while(!(SPSR & (1 << SPIF)));                   // Wait for it to be transmitted completely

    return SPDR;                                    // Send back the read value
}

void spi_select() {
    PORTB &= ~(1 << SS);                            // Pull down the SS
}

void spi_unselect() {
    PORTB |= (1 << SS);                             // Brings back SS to high
}

void spi_close() {
    spi_unselect();
    SPCR &= ~(1 << SPE);                            // Disable SPI
}