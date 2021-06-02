#pragma once
#include <stdint.h>
uint8_t spi_txrx(uint8_t data);		/*Starts an SPI-interaction. Send given data and returns received data*/
void spi_init();					/*Initiates SPI with F_CPU/4 as clock and in master mode. Initiates associated ports as well*/