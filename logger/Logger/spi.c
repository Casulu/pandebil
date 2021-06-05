/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			spi.c
* Description:	File for communicating via SPI.
* Created:		2021-05-07
************************************************************************/

/*************** Header file ***************/
#include "spi.h"

/*************** Function definition ***************/
/************************************************************************
Sets the different pins to outputs and initializes the SPI interface.
************************************************************************/
void spiInit(void) {
	// Set SS_SD, SS_OLED, MOSI, SCK and RS_OLED pins to output
	SPI_DDR |= (1<<SPI_SS_SD) | (1<<SPI_SS_OLED) | (1<<SPI_MOSI) | (1<<SPI_SCK) | (1<<SPI_RS_OLED);
	
	// Enable, set master
	SPCR |= (1<<SPE) | (1<<MSTR);
	
	// Set SPI-mode 0
	SPCR &= ~((1<<CPOL) | (1<<CPHA));
	
	// Select minimal prescaler (max system speed), init u8g SPI
	CLKPR = 0x80;
	CLKPR = 0x00;
	u8g_InitHWSPI(&u8g, &u8g_dev_ssd1306_128x64_hw_spi, PN(1, SPI_SS_OLED), PN(1, SPI_RS_OLED), U8G_PIN_NONE);
}
