/*
 * lcd.c
 *
 * Created: 2021-04-22 14:16:58
 *  Author: Rasmus Lyxell
 */ 
#include "lcd.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

static uint8_t cursor_pos;
static const uint8_t initseq[9] PROGMEM = {0x39, 0x15, 0x55, 0x6E, 0x72, 0x38, 0x0C, 0x01, 0x06};

/************************************************************************/
/* Transmits and receives via SPI										*/
/* @param pos The position to set to									*/
/************************************************************************/
static void spi_tx(uint8_t data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}

/************************************************************************/
/* Initiate the LCD-panel with non-blinking incrementing cursor.		*/
/************************************************************************/
void lcd_init(){
	DDRB |= (1<<RS) | (1<<CSB);		//Set pins for RS and CSB to out
	PORTB &= ~((1<<RS) | (1<<CSB));	//Set RS and CSB low for writing commands
	for(int i = 0; i < 9; i++){
		spi_tx(pgm_read_byte(&(initseq[i])));
		_delay_ms(1.5);
	}
	cursor_pos = 0;
	PORTB |= 1<<CSB;
}

/************************************************************************/
/* Sends a character code to the LCD to display at the current cursor pos*/
/* @param char_code The character code to send							*/
/************************************************************************/
void write_lcd_char(uint8_t char_code){
	PORTB |= 1<<RS;
	PORTB &= ~(1<<CSB);   //Clear CSB
	spi_tx(char_code);
	_delay_us(30);
	PORTB &= ~(1<<RS);
	PORTB |= 1<<CSB;
	cursor_pos++;
}

/************************************************************************/
/* Sends a string of character codes to the LCD to display at the current*/ 
/* cursor pos															*/
/* @param string A pointer to the zero-terminated string to print		*/
/************************************************************************/
void write_lcd_string(char* string){
	PORTB |= 1<<RS;
	PORTB &= ~(1<<CSB);   //Clear CSB
	uint8_t charbuf;
	while((charbuf = (uint8_t)*(string++)) != '\0'){
		spi_tx(charbuf);
		_delay_us(30);
		cursor_pos++;
	}
	PORTB &= ~(1<<RS);
	PORTB |= 1<<CSB;
}

/************************************************************************/
/* Sends a string of character codes from program memory to the LCD to  */
/* @param string A pointer to the zero-terminated string to print		*/
/************************************************************************/
void write_lcd_progmem_string(const char* progmem_string){
	PORTB |= 1<<RS;
	PORTB &= ~(1<<CSB);   //Clear CSB
	uint8_t i = 0;
	uint8_t charbuf;
	while((charbuf = pgm_read_byte(&(progmem_string[i++]))) != '\0'){
		spi_tx(charbuf);
		_delay_us(30);
		cursor_pos++;
	}
	PORTB &= ~(1<<RS);
	PORTB |= 1<<CSB;
}

/************************************************************************/
/* Clears the LCD                                                       */
/************************************************************************/
void clear_LCD(){
	PORTB &= ~(1<<CSB);   //Clear CSB
	spi_tx(0x01);
	_delay_ms(1.5);
	cursor_pos = 0;
}

/************************************************************************/
/* Clears the given line on the LCD                                     */
/* @param linenum The line to clear (0-2)								*/
/************************************************************************/
void clear_line(uint8_t linenum){
	set_cursor_pos(linenum * 16);
	PORTB |= 1<<RS;
	PORTB &= ~(1<<CSB);
	for (uint8_t i = 0; i < 16; i++){
		spi_tx(' ');
		_delay_us(30);
		cursor_pos++;
	}
	PORTB &= ~(1<<RS);
	PORTB |= 1<<CSB;
}

/************************************************************************/
/* Sets the LCD cursors position.										*/
/* @param pos The position to set to									*/
/************************************************************************/
void set_cursor_pos(uint8_t pos){
	PORTB &= ~(1<<CSB);   //Clear CSB
	spi_tx(pos|0x80);
	_delay_us(30);
	cursor_pos = pos;
}

/************************************************************************/
/* Get the LCD cursors position.										*/
/* @return The position of the cursor									*/
/************************************************************************/
uint8_t get_cursor_pos(){
	return cursor_pos;
}

void set_character(uint8_t char_code, uint8_t rows[8]){
	PORTB &= ~(1<<CSB);   //Clear CSB
	for (uint8_t i = 0; i < 8; i++){
		//Set CGRAM address
		PORTB &= ~(1<<RS);
		spi_tx(0x40|(char_code<<3)|(i));
		_delay_us(30);
		//Write row to character	
		PORTB |= 1<<RS;
		spi_tx(rows[i]);
		_delay_us(30);
	}
	PORTB &= ~(1<<RS);
	PORTB |= 1<<CSB;
}

/************************************************************************/
/* Set a given address in CGRAM to a given bitmap stored in pgm_space	*/
/* @param char_code The address or char code in CGRAM to write to		*/
/* @param rows An array of 8 bytes where each bytes b0-b4 is			*/
/*	      a row of the character. Must be stored in program memory		*/
/************************************************************************/
void set_character_pgm(uint8_t char_code, const uint8_t rows[8]){
	PORTB &= ~(1<<CSB);   //Clear CSB
	for (uint8_t i = 0; i < 8; i++){
		//Set CGRAM address
		PORTB &= ~(1<<RS);
		spi_tx(0x40|(char_code<<3)|(i));
		_delay_us(30);
		//Write row to character
		PORTB |= 1<<RS;
		spi_tx(pgm_read_byte(&(rows[i])));
		_delay_us(30);
	}
	PORTB &= ~(1<<RS);
	PORTB |= 1<<CSB;
}