#include <avr/io.h>
#include "spi.h"

/*
 * Initialize spi in master mode.
 */
void spi_master_init(void)
{
	DDRB |= (1<<SCK) | (1<<MOSI);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
	SPCR &=~ ((1<<CPOL) | (1<<CPHA)); /* SPI mode 0*/
}

/*
 * Sends data through spi.
 */
void spi_master_transmit(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
}