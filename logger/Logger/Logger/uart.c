/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			uart.c
* Description:	File for communicating via uart.
* Created:		2021-05-10
************************************************************************/

/*************** Header file ***************/
#include "uart.h"

/*************** Static function declaration ***************/
static int uartPutc(char c, FILE *stream);

/*************** stdout ***************/
static FILE uartstdout = FDEV_SETUP_STREAM(uartPutc,
NULL,_FDEV_SETUP_WRITE);

/*************** Function definition ***************/
/************************************************************************
Initializes the uart, activates interrupt on recive and sets stdout.
************************************************************************/
void uartInit(void) {
	// Set baudrate to 38400
	UBRR0 = 12;

	// Enable transmitter and receiver
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	
	// Set form to 8 data bits, 1 stop bit
	UCSR0C |= (3<<UCSZ00);
	UCSR0C &= ~(1<<USBS0);
	
	// Set stdout
	stdout = &uartstdout;
}

/************************************************************************
Function for writing one byte to uart.
************************************************************************/
static int uartPutc(char c, FILE *stream) {
	// wait for transmit buffer to be empty
	while(!(UCSR0A & (1<<UDRE0)));

	// load data into transmit register
	UDR0 = c;
	return 0;
}
