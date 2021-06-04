#pragma once

#include <stdio.h>
#include <stdbool.h>

#define BAUD 500000
#define MYUBRR F_CPU/16/BAUD-1

void uart_init();							/*Initiate UART with 8 data bits, 1 stop bit and BAUD as baudrate*/
void uart_send_byte(uint8_t data);			/*Send a single byte*/
void uart_send_line(char* line);			/*Send each byte from a string with an added newline*/
bool uart_receive_byte(uint8_t* buffer);	/*Receive a byte. Non blocking, returns boolean showing if byte could be received*/
void uart_flush();							/*Flush the UART*/