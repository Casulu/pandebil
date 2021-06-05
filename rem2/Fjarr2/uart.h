#pragma once

#include <stdio.h>
#include <stdbool.h>

#define BAUD 500000
#define MYUBRR F_CPU/16/BAUD-1

char linebuf[48];

/*
* adc_setup() - Sets relevant registers in preperations for using the uart.
*
* Returns: Nothing
*/
void uart_init();
/*
* uart_set_stdout() - Sets stdout to write to the uart.
*
* Returns: Nothing
*/
void uart_set_stdout();
/*
* uart_send_byte() - Sends a char through the uart.
* @c: The char to send
* @stream. A file-stream utilized by the stdout override
*
* Returns: 0
*/
int uart_send_byte(uint8_t data, FILE *stream);
/*
* uart_send_line() - Sends a string through the uart.
* @line: The string to send
*
* Returns: Nothing
*/
void uart_send_line(char* line);
/*
* uart_send_topic() - Sends a string through the uart and add topic/sub topic chars to the start.
* @topic: The topic number to add 0-9
* @sub_topic: The sub topic number to add 0-9
* @line: The string to send
*
* Returns: Nothing
*/
void uart_send_topic(uint8_t topic, uint8_t sub_topic, char* line);
/*
* uart_receive_byte() - Waitfor and recives a byte from the uart.
* @buffer: A pointer to the variable to put the recived byte into
*
* Returns: True
*/
bool uart_receive_byte(uint8_t* buffer);
