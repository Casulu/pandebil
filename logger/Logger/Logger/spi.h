/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			spi.h
* Description:	Header file for spi.c
* Created:		2021-05-07
************************************************************************/

#ifndef SPI_H_
#define SPI_H_

/*************** External library ***************/
#include <avr/io.h>

/*************** Other header file ***************/
#include "u8g.h"

/*************** Constants ***************/
#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_SS_SD PINB0
#define SPI_SS_OLED PINB2
#define	SPI_MOSI PINB3
#define SPI_SCK PINB5
#define SPI_RS_OLED PINB1

/*************** Global variable ***************/
u8g_t u8g;

/*************** Function declaration ***************/
/**
 @brief  Initializes SPI. Needs to be called once
 @param  void
 @return none
 */
void spiInit(void);

#endif // LCD_H_
