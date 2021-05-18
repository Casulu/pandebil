#include "uart.h"
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>

void uart_send_byte(uint8_t data){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

void uart_send_line(char* line){
	uint8_t byte;
	while((byte = *(line++)) != '\0'){
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = byte;
	}
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = '\n';
}

bool uart_receive_byte(uint8_t* buffer){
	if(!( UCSR0A & (1<<RXC0))) return false;
	*buffer = UDR0;
	return true;
}

void uart_flush(){
	uint8_t dummy;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
}

void uart_init(){
	UBRR0 = 0;		//0==500000 baud rate.  12==38400 baud rate
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}