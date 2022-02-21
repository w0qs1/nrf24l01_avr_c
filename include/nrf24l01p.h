#ifndef NRF24L01P_H
#define NRF24L01P_H

#include <avr/io.h>
#include "spi.h"

#define CE      1   // Port B

// Register Map
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

// Instruction mnemonics
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

void nrf24_ce_enable(void);
void nrf24_ce_disable(void);
void nrf24_writereg(uint8_t reg, uint8_t data);
void nrf24_writeregmulti(uint8_t reg, uint8_t *data, uint8_t size);
uint8_t nrf24_readreg(uint8_t reg);
void nrf24_readregmulti(uint8_t reg, uint8_t *data, uint8_t size);
void nrf24_writecmd(uint8_t cmd);
void nrf24_init(void);
void nrf24_reset(uint8_t REG);
void nrf24_setuptx(uint8_t *address, uint8_t channel);
uint8_t nrf24_transmit(uint8_t *data);
void nrf24_setuprx(uint8_t *address, uint8_t channel);
uint8_t is_data_available(uint8_t pipenum);
void nrf24_receive(uint8_t *data);

#endif