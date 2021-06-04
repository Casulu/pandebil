#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>
#include <stdbool.h>
#include "spi.h"
#include "display.h"

volatile char buff[49]; //Buffer for the display 48 sections on the display +1 for \0.
volatile bool buff_uppdated;

/*
 * Initializes the ports for the display.
 */
void display_port_init(void)
{
	DDRB |= (1<<SS_DISP);
	DDRC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

/*
 * Initializes the display
 */
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
	spi_master_transmit(0x0C);
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
	
	for (int i = 0; i < 48; i++)
	{
		buff[i] = ' ';
	}
	buff[48] = '\0';
	buff_uppdated = false;
}

/*
 * Function meant to be used by printf to print on the display.
 */
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

/*
 * Prints a character to the display.
 */
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
}

/*
 * Moves the cursor a number of steps.
 */
void display_move_cursor(uint8_t steps)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	for (uint8_t i = 0; i < steps; i++)
	{
		spi_master_transmit(0b00010100);
		_delay_us(30);
	}
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

/*
 * Sets an absolut position for the cursor.
 */
void display_position_cursor(uint8_t position)
{
	SPCR |= (1<<SPR0);
	SPCR &=~ (1<<SPR1);
	PORTC &=~ (1<<RS_DISP);
	PORTB &=~ (1<<SS_DISP);
	spi_master_transmit(0b00000010);
	_delay_us(30);
	for (uint8_t i = 0; i < position; i++)
	{
		spi_master_transmit(0b00010100);
		_delay_us(30);
	}
	PORTC |= (1<<RS_DISP);
	PORTB |= (1<<SS_DISP);
}

/*
 * Clears the display.
 */
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

/*
 * Displays whats in the display buffer if it has changed.
 */
void display_buff(void)
{
	if (buff_uppdated == true)
	{
		display_clear();
		printf("%s", buff);
		buff_uppdated = false;
	}
}

/*
 * Adds a string to the display buffer at a position.
 */
void display_add_to_buff(const char* string, uint8_t length, uint8_t position)
{
	strncpy((char*)buff + position, string, length);
	buff_uppdated = true;
}