#pragma once

#include <stdio.h>
#include <stdbool.h>

#define BAUD 500000
#define MYUBRR F_CPU/16/BAUD-1

char linebuf[48];

void uart_init();
void uart_send_byte(uint8_t data);
void uart_send_line(char* line);
bool uart_receive_byte(uint8_t* buffer);
bool uart_receive_line(char* buffer);