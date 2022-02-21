#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <util/delay.h>

#define SS      2   // Port B
#define MOSI    3   // Port B
#define MISO    4   // Port B
#define SCK     5   // Port B

/**
 * @brief  Initialize the SPI registers
 * @param  sck_speed_divide: Set the clock speed (0 -> F_CPU/4, 1 -> F_CPU/16, 2 -> F_CPU/64, 3 -> F_CPU/128, **4 -> F_CPU/2, 5 -> F_CPU/8, 6 -> F_CPU/32, 7 -> F_CPU/64)
 * @param  cpol: Clock polarity (0 -> Base value of clock is 0, 1 -> Base value of clock is 1)
 * @param  cpha: Clock phase (0 -> Sample on Rising Edge, 1 -> Sample on Falling Edge)
 * @return None
 */
extern void spi_init(uint8_t sck_speed_divide, uint8_t cpol, uint8_t cpha);

/**
 * @brief  To send and receive data
 * @param  data: The data to be sent
 * @return an uint8_t containing the received data
 */
extern uint8_t spi_tranceive(uint8_t data);

/**
 * @brief  Close the SPI connection
 * @return None
 */
extern void spi_close(void);

/**
 * @brief  Bring the Chip Select down
 * @retval None
 */
extern void spi_select(void);

/**
 * @brief  Release the Chip Select to high
 * @retval None
 */
extern void spi_unselect(void);
#endif
