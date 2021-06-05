#ifndef DISPLAY_H_
#define DISPLAY_H_

/*
* Code for controlling a display using spi.
*
* Author: Victor Löfstedt (c19vlt@cs.umu.se)
*/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

//Display instruction constants
#define FUNCTION_SET 0b00111001
#define BIAS_SET 0b00011101
#define POWER_CONTROL 0b01010000
#define FOLLOWER_CONTROL 0b01101100
#define CONTRAST_SET 0b01111000
#define FUNCTION_SET2 0b00111000
#define DISPLAY_ON_OFF 0b00001111
#define CLEAR_DISPLAY 0b00000001
#define ENTRY_MODE_SET 0b00000110
#define DISABLE_CURSOR 0b00001100


/*
* display_setup() - Sends a series of setup instruction to the display.
* OBS! Does NOT setup the spi
*
* Returns: Nothing
*/
void display_setup();

/*
* display_mode_instruction() - Switches the display to instruction mode.
*
* Returns: Nothing
*/
void display_mode_instruction();
/*
* display_mode_write() - Switches the display to write mode.
*
* Returns: Nothing
*/
void display_mode_write();
/*
* display_mode_write() - Sets stdout to write to the display.
*
* Returns: Nothing
*/
void display_set_stdout();
/*
* spi_send_display() - Sends a byte fo data to the display using the spi.
* This data will be interpreted as either an instruction or a character to write depending on what mode the display is in.
* @send: The byte of data so send 
*
* Returns: Nothing
*/
void spi_send_display(uint8_t send);
/*
* display_write_char() - Writes a char to the display.
* OBS! The display MUST be in write mode before this function is called.
* @c: The char to write
* @stream. A file-stream utilized by the stdout override
*
* Returns: 0
*/
int display_write_char(char c, FILE *stream);
/*
* display_set_cursor_pos() - Set the position of the cursor on the display.
* @pos: The position to set the cursor to
*
* Returns: Nothing
*/
void display_set_cursor_pos(uint8_t pos);
/*
* display_clear() - Clears the display.
*
* Returns: Nothing
*/
void display_clear();
/*
* display_clear_line() - Clears a line on the display.
* @line: The line to clear 0-2
*
* Returns: Nothing
*/
void display_clear_line(uint8_t line);
/*
* display_clear_top_rows() - Clears the top two rows of the display.
*
* Returns: Nothing
*/
void display_clear_top_rows();

/*
* display_turnoff() - Turns of the display.
*
* Returns: Nothing
*/
void display_turnoff();

/*
* display_set_character_pgm() - Creates a custom character
* @char_code: Id of the character
* @rows: The shape of the character
*
* Returns: Nothing
*/
void display_set_character_pgm(uint8_t char_code, const uint8_t rows[8]);
#endif