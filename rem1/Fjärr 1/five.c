/*
 * five.c
 *
 * Created: 2021-05-22 00:04:56
 *  Author: Rasmus Lyxell
 */ 

#include "five.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>

static char field[15][17];
static uint8_t topmost_row;
static uint8_t selected_row;
static uint8_t selected_col;
static char sendbuf[6] = "33";

void five_init(){
	for(uint8_t i = 0; i < 15; i++){
		/*Initialise field with row-numbers 1-E*/
		 field[i][0] = (i < 9) ? ('0'+i+1) : ('A' + (i-9));
		 field[i][16] = '\0';
		 /*Spaces for rest of field*/
		 for(uint8_t j = 1; j < 16 ; j++){
			 field[i][j] = ' ';
		 }
		 char idk[40];
		 idk[0] = '-';
		 strcpy(idk+1, field[i]);
		 uart_send_line(idk);
	}
	selected_row = 0;
	topmost_row = 0;
	selected_col = 1;
	
} 

uint8_t five_relative_cursor(){
	return (selected_row - topmost_row)*16+selected_col;
}

bool five_cursor_up(){
	if(selected_row > 0 && selected_row-- == topmost_row) return true;
	return false;
}

bool five_cursor_down(){
	if(selected_row < 14 && selected_row++ == topmost_row+3) return true;
	return false;
}

bool five_cursor_left(){
	if(selected_col > 1) selected_col--;
	return false;
}

bool five_cursor_right(){
	if(selected_col < 1) selected_col++;
	return false;
}

char* five_get_active_field(){
	return field[topmost_row];
}

char five_get_active_char(){
	return field[selected_row][selected_col];
}

bool five_check_win(uint8_t row, uint8_t col, char side){
	//Check vertical
	uint8_t win_counter = 0;
	for(uint8_t i = (row >= 5) ? row-4 : 0; i < row+4 && i < 15; i++){
		if(field[i][col] == 'O'){
			if(++win_counter) return true;
		}
		else win_counter = 0;
	}
	//Check horizontal
	win_counter = 0;
	for(uint8_t i = (col >= 6) ? col-4 : 0; i < col+4 && i < 16; i++){
		if(field[i][col] == 'O'){
			if(++win_counter) return true;
		}
		else win_counter = 0;
	}
	//Check diagonal
	win_counter = 0;
	int8_t offset;
	if(row >= 4 && col >= 5) offset = -4;
	else if(row > col-1) offset = -row;
	else offset = -col;
	
	for(int8_t i = offset; i < 5 && row+i < 15 && col+i < 16; i++){
		if(field[row+i][col+i] == 'O'){
			if(++win_counter) return true;
		}
		else win_counter = 0;
	}
	return false;
}

bool five_receive(char* args){
	uint8_t pos = atoi(args);
	uint8_t row = pos/15;
	uint8_t col = pos-row+1;
	field[row][col] = 'O';
	return five_check_win(row, col, '0');
}

bool five_cursor_valid(){
	return field[selected_row][selected_col] == ' ';
}

bool five_send(){
	field[selected_row][selected_col] = 'X';
	uart_send_line(itoa((selected_row*15+selected_col-1)+'0' ,sendbuf+2, 10));
	return five_check_win(selected_row, selected_col, 'X');
}