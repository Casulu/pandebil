#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "spi.h"
#include "display.h"

void display_port_init(void)
{
	DDRB |= (1<<SS_DISP);
	DDRC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

void display_init(void)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	
	PORTB &=~ (1<<SS_DISP);
	
	//Function Set
	//8 bit data length, 2 lines, instruction table 1
	spi_master_transmit(0x39);
	_delay_us(30);
	
	//Bias Set
	//BS: 1/4, 3 line LCD
	spi_master_transmit(0x1D);
	_delay_us(30);
	
	//Power Control
	//booster off, contrast C5, set C4
	spi_master_transmit(0x50);
	_delay_us(30);
	
	//Follower Control
	//set voltage follower and gain
	spi_master_transmit(0x6C);
	_delay_us(30);
	
	//Contrast Set
	//set contrast C3, C2, C1
	spi_master_transmit(0x7C);
	_delay_us(30);

	//Function Set
	//switch back to instruction table 0
	spi_master_transmit(0x38);
	_delay_us(30);
	
	//Display ON/OFF
	//display on, cursor on, cursor blink
	spi_master_transmit(0x0F);
	_delay_us(30);
	
	//Clear Display
	//delete display, cursor at home
	spi_master_transmit(0x01);
	_delay_ms(1);
	
	//Entry Mode Set
	//cursor auto-increment
	spi_master_transmit(0x06);
	_delay_ms(4);
	
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

int display_put_char(char c, FILE *stream)
{
	/* Set SPI speed for display */
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	
	PORTB &=~ (1<<SS_DISP);
	
	/* Transmit to display*/
	spi_master_transmit(c);
	_delay_us(30);
	
	PORTB |= (1<<SS_DISP);
	return 0;
}

void display_char(char c)
{
	/* Set SPI speed for display */
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	
	PORTB &=~ (1<<SS_DISP);
	
	/* Transmit to display*/
	spi_master_transmit(c);
	_delay_us(30);
	
	PORTB |= (1<<SS_DISP);
	return 0;
}

void display_move_cursor(int steps)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	for (int i = 0; i < steps; i++)
	{
		spi_master_transmit(0b00010100);
		_delay_us(30);
	}
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

void display_position_cursor(int position)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	spi_master_transmit(0b00000010);
	_delay_us(30);
	for (int i = 0; i < position; i++)
	{
		spi_master_transmit(0b00010100);
		_delay_us(30);
	}
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

void display_clear()
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	spi_master_transmit(0x01);
	_delay_ms(4);
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}