#include "uart.h"
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>

//printf override
static FILE uart_stdout_override = FDEV_SETUP_STREAM(uart_send_byte,NULL,_FDEV_SETUP_WRITE);

void uart_init(){
	UBRR0 = 12;		//12==38400 baud rate
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void uart_set_stdout(){
	stdout = &uart_stdout_override;
}

int uart_send_byte(uint8_t data, FILE *stream){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
	return 0;
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

void uart_send_topic(uint8_t topic, uint8_t sub_topic, char* line){
	uint8_t byte;
	//Send topic byte
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = topic+48;
	//Send subtopic byte
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = sub_topic+48;
	//Send message
	while((byte = *(line++)) != '\0'){
		while ( !( UCSR0A & (1<<UDRE0)) );
		UDR0 = byte;
	}
	//Send '\n' to end message
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = '\n';
}

bool uart_receive_byte(uint8_t* buffer){
	if(!( UCSR0A & (1<<RXC0))) return false;
	*buffer = UDR0;
	return true;
}