#include "uart.h"
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>

/*
 * Sends a byte to uart.
 */
void uart_send_byte(uint8_t data){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

/*
 * Sends a line to uart.
 */
void uart_send_line(char* line){
	uint8_t byte;
	while((byte = *(line++)) != '\0'){
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = byte;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = '\n';
}

/*
 * Receives a byte from uart and places it in a buffer.
 */
bool uart_receive_byte(uint8_t* buffer){
	if(!( UCSR0A & (1<<RXC0))) return false;
	*buffer = UDR0;
	return true;
}

/*
 * Initializes uart.
 */
void uart_init(){
	UBRR0 = 51;		//9600 baud rate
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	//Set frame format: 8data, 1stop bit
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}