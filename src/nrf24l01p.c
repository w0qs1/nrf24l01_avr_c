#include "nrf24l01p.h"
#include "spi.h"

void nrf24_ce_enable(void) {
    PORTB |= (1 << CE);
}

void nrf24_ce_disable(void) {
    PORTB &= ~(1 << CE);
}

void nrf24_writereg(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = (reg | 1 << 5);
    buf[1] = data;

    spi_select();

    spi_tranceive(buf[0]);                          // Send the register
    spi_tranceive(buf[1]);                          // Send the data

    spi_unselect();
}

void nrf24_writeregmulti(uint8_t reg, uint8_t *data, uint8_t size) {
    uint8_t buf = (reg | 1 << 5);

    spi_select();

    spi_tranceive(buf);                             // Send the register
    for(uint8_t i = 0; i < size; i++) {
        spi_tranceive(*(data + i));                 // Send the data one byte at a time
    }

    spi_unselect();
}


uint8_t nrf24_readreg(uint8_t reg) {
    uint8_t data;

    spi_select();

    spi_tranceive(reg);                             // Send the register
    data = spi_tranceive(0x00);                     // Receive the data

    spi_unselect();

    return data;
}

void nrf24_readregmulti(uint8_t reg, uint8_t *data, uint8_t size) {
    spi_select();

    spi_tranceive(reg);                             // Send the register

    for(uint8_t i = 0; i < size; i++) {
        *(data + i) = spi_tranceive(0x00);          // Receive the data one byte at a time
    }

    spi_unselect();
}

void nrf24_writecmd(uint8_t cmd) {
    spi_select();

    spi_tranceive(cmd);                             // Send the command

    spi_unselect();
}

void nrf24_init(void) {
    spi_init(2, 0, 0);                              // Setup the SPI

    nrf24_ce_disable();                             // disable the device before configuring

    nrf24_writereg(CONFIG, 0x00);
    nrf24_writereg(EN_AA, 0x00);                    // No Auto ACK

    nrf24_writereg(EN_RXADDR, 0x00);                // No RX Data pipes
    nrf24_writereg(SETUP_AW, 0x03);                 // 5 Byte Address Width

    nrf24_writereg(SETUP_RETR, 0x00);               // No Retransmission
    nrf24_writereg(RF_CH, 0x00);                    // Setup the channel

    nrf24_writereg(RF_SETUP, 0b00001110);           // Power = 0dBm, Data rate = 2 Mbps

    nrf24_ce_enable();                              // Enable the device after configuring
}

void nrf24_reset(uint8_t REG)
{
	if (REG == STATUS)
	{
		nrf24_writereg(STATUS, 0x00);
	}

	else if (REG == FIFO_STATUS)
	{
		nrf24_writereg(FIFO_STATUS, 0x11);
	}

	else {
	nrf24_writereg(CONFIG, 0x08);
	nrf24_writereg(EN_AA, 0x3F);
	nrf24_writereg(EN_RXADDR, 0x03);
	nrf24_writereg(SETUP_AW, 0x03);
	nrf24_writereg(SETUP_RETR, 0x03);
	nrf24_writereg(RF_CH, 0x02);
	nrf24_writereg(RF_SETUP, 0x0E);
	nrf24_writereg(STATUS, 0x00);
	nrf24_writereg(OBSERVE_TX, 0x00);
	nrf24_writereg(CD, 0x00);
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	nrf24_writeregmulti(RX_ADDR_P0, rx_addr_p0_def, 5);
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	nrf24_writeregmulti(RX_ADDR_P1, rx_addr_p1_def, 5);
	nrf24_writereg(RX_ADDR_P2, 0xC3);
	nrf24_writereg(RX_ADDR_P3, 0xC4);
	nrf24_writereg(RX_ADDR_P4, 0xC5);
	nrf24_writereg(RX_ADDR_P5, 0xC6);
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	nrf24_writeregmulti(TX_ADDR, tx_addr_def, 5);
	nrf24_writereg(RX_PW_P0, 0);
	nrf24_writereg(RX_PW_P1, 0);
	nrf24_writereg(RX_PW_P2, 0);
	nrf24_writereg(RX_PW_P3, 0);
	nrf24_writereg(RX_PW_P4, 0);
	nrf24_writereg(RX_PW_P5, 0);
	nrf24_writereg(FIFO_STATUS, 0x11);
	nrf24_writereg(DYNPD, 0);
	nrf24_writereg(FEATURE, 0);
	}
}

void nrf24_setuptx(uint8_t *address, uint8_t channel) {
    nrf24_ce_disable();                             // disable the device before configuring

    nrf24_writereg(RF_CH, channel);                 // Select the Channel
    nrf24_writeregmulti(TX_ADDR, address, 5);       // Write the TX address

    uint8_t config = nrf24_readreg(CONFIG);
    config &= 0xF2;                                 // 0 in PRIM_RX, 1 in PWR_UP, all other bits are masked
    nrf24_writereg(CONFIG, config);                 // Power UP the device

    nrf24_ce_enable();                              // Enable the device after configuring
}

uint8_t nrf24_transmit(uint8_t *data) {
    spi_select();
    spi_tranceive(W_TX_PAYLOAD);                    // Send command to make the device receive the TX payload
    for (uint8_t i = 0; i < 32; i++) { 
        spi_tranceive(*(data + i));                 // Send the payload
        if (*(data + i) == '\0') {
            for(uint8_t j = i; j < 31; j++) {       // Fill the rest of the payload with \0
                spi_tranceive(0x00);
            }
            break;
        }
    }
    spi_unselect();

    _delay_ms(1);                                   // Wait for the device to Transmit

    uint8_t fifostatus = nrf24_readreg(FIFO_STATUS);

    if ((fifostatus & (1 << 4)) && (!(fifostatus & (1 << 3)))) {
        nrf24_writecmd(FLUSH_TX);                   // Payload sent successfully, flush the TX FIFO buffer
        nrf24_reset(FIFO_STATUS);
        return 1;                                   // Return 1 for successful transmission
    }

    return 0;                                       // Return 0 for unsuccessful transmission
}

void nrf24_setuprx(uint8_t *address, uint8_t channel) {
    nrf24_ce_disable();                             // disable the device before configuring

    nrf24_writereg(RF_CH, channel);                 // Select the Channel

    uint8_t en_rxaddr = nrf24_readreg(EN_RXADDR);
    en_rxaddr |= (1 << 1);
    nrf24_writereg(EN_RXADDR, en_rxaddr);
    nrf24_writeregmulti(RX_ADDR_P1, address, 5);    // Write the RX address

    nrf24_writereg(RX_PW_P1, 32);                   // 32 byte payload size

    uint8_t config = nrf24_readreg(CONFIG);
    config |= ((1 << 1) | (1 << 0));
    nrf24_writereg(CONFIG, config);                 // Power UP the device

    nrf24_ce_enable();                              // Enable the device after configuring
}

uint8_t is_data_available(uint8_t pipenum) {
    uint8_t status = nrf24_readreg(STATUS);

    if((status & (1 << 6)) && (status & (pipenum << 1))) {
        nrf24_writereg(STATUS, (1 << 6));
        return 1;
    }

    return 0;
}

void nrf24_receive(uint8_t *data) {
    spi_select();
    spi_tranceive(R_RX_PAYLOAD);                    // Send command to make the device receive the RX payload
    for (uint8_t i = 0; i < 32; i++) {
        *(data + i) = spi_tranceive(0x00);          // Receive the payload
        if (*(data + i) == '\0') {
            break;
        }        
    }
    spi_unselect();

    _delay_ms(1);                                   // Wait for the device to Receive

    nrf24_writecmd(FLUSH_RX);                       // Flush the RX FIFO
}