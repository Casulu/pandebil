#include "display.h"

//printf override
static FILE display_stdout_override = FDEV_SETUP_STREAM(display_write_char,NULL,_FDEV_SETUP_WRITE);


void display_setup() {
	display_mode_instruction();
	spi_send_display(FUNCTION_SET);
	_delay_ms(2);
	spi_send_display(BIAS_SET);
	_delay_ms(2);
	spi_send_display(POWER_CONTROL);
	_delay_ms(2);
	spi_send_display(FOLLOWER_CONTROL);
	_delay_ms(2);
	spi_send_display(CONTRAST_SET);
	_delay_ms(2);
	spi_send_display(FUNCTION_SET2);
	_delay_ms(2);
	spi_send_display(DISPLAY_ON_OFF);
	_delay_ms(2);
	spi_send_display(CLEAR_DISPLAY);
	_delay_ms(2);
	spi_send_display(ENTRY_MODE_SET);
	_delay_ms(2);
	spi_send_display(DISABLE_CURSOR);
	_delay_ms(2);
	display_mode_write();
	stdout = &display_stdout_override;
}

void display_mode_instruction() {
	PORTB &=~ (1<<PINB1);
}
void display_mode_write() {
	PORTB |= (1<<PINB1);
}

void display_set_stdout(){
	stdout = &display_stdout_override;
}

void spi_send_display(uint8_t message) {
	PORTB &=~ (1<<PINB2);
	SPDR = message;
	while((SPSR & (1<<SPIF)) == 0);
	PORTB |= (1<<PINB2);
}

int display_write_char(char c, FILE *stream) {
	spi_send_display(c);
	_delay_us(30);
	return 0;
}

void display_set_cursor_pos(uint8_t pos){
	display_mode_instruction();
	spi_send_display(0x80|pos);
	display_mode_write();
	_delay_us(30);
}

void display_clear(){
	display_mode_instruction();
	spi_send_display(0b00000001);
	display_mode_write();
	_delay_ms(2);
}

void display_clear_line(uint8_t line){
	display_set_cursor_pos(line<<4);
	for(uint8_t i = 0; i < 16; i++){display_write_char(' ', NULL);}
}

void display_clear_top_rows(){
	display_set_cursor_pos(0);
	for(uint8_t i = 0; i < 32; i++){display_write_char(' ', NULL);}
	display_set_cursor_pos(0);
}

void display_turnoff(){
	PORTB &= 0b11111001;
	spi_send_display(0b00001000);
	_delay_us(30);
}

void display_set_character_pgm(uint8_t char_code, const uint8_t rows[8]){
	for (uint8_t i = 0; i < 8; i++){
		//Set CGRAM address
		display_mode_instruction();
		spi_send_display(0x40|(char_code<<3)|(i));
		_delay_us(30);
		//Write row to character
		display_mode_write();
		spi_send_display(pgm_read_byte(&(rows[i])));
		_delay_us(30);
	}
}