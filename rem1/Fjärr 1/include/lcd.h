#pragma once
#include <stdint.h>
#define RS 0	//The number for the pin on PORTB connected to RS
#define CSB 2	//The number for the pin on PORTB connected to CSB

void write_lcd_char(uint8_t char_code);						/*Writes a single character code to the LCD at the current cursor position*/
void write_lcd_string(char* string);						/*Writes a given string to the LCD at the current cursor position*/
void write_lcd_progmem_string(const char* progmem_string);	/*Writes a given string in program memory to the LCD at the current cursor position*/
void lcd_init();											/*Initiates the LCD with 3 rows and invisible incrementing cursor*/
void clear_LCD();											/*Clears the LCD*/
void clear_line(uint8_t linenum);							/*Clears the given line on the LCD*/
void set_cursor_pos(uint8_t pos);							/*Sets the position of the LCD cursor*/
uint8_t get_cursor_pos();									/*Gets the current cursor position*/
void set_character(uint8_t adress, uint8_t rows[8]);		/*Sets the given adress in CGRAM to the given 8 rows of bits*/
void set_character_pgm(uint8_t adress, const uint8_t rows[8]);	/*Sets the given adress in CGRAM to the given 8 rows of bits from pgm*/