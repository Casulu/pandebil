/*
 * spi.c
 *
 * Created: 2021-04-22 14:30:49
 *  Author: Rasmus Lyxell
 */
#include <avr/io.h>
#include <stdint.h>
#include "spi.h"

#define MISO 4
#define MOSI 3
#define SCK 5
#define SS 2

/************************************************************************/
/* Starts a SPI communication with a given data in SPI buffer			*/
/* and receives incoming data                                           */
/* @param data The data to send											*/
/************************************************************************/
uint8_t spi_txrx(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

/************************************************************************/
/* Initiates the MCU SPI as master with 16 as prescaler factor           */
/************************************************************************/
void spi_init(){
	// Set MOSI and SCK as out
	DDRB |= (1<<MOSI) | (1<<SCK) | (1<<SS);
	DDRB &= ~(1<<MISO);
	//Enable SPI in master mode with fck/4 as clock
	SPCR = (1<<SPE) | (1<<MSTR);
}