#include <avr/io.h>
#include "spi.h"

void spi_master_init(void)
{
	DDRB |= (1<<SCK) | (1<<MOSI);
	SPCR |= (1<<SPE) | (1<<MSTR);
	SPSR |= (1<<SPI2X);
	SPCR &=~ ((1<<CPOL) | (1<<CPHA)); /* SPI mode 0*/
}

void spi_master_transmit(char data)
{
	SPDR = data;
	while (!(SPSR & (1<<SPIF)));
}