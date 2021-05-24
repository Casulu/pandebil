/*
 * five.h
 *
 * Created: 2021-05-23 00:29:31
 *  Author: Rasmus Lyxell
 */ 


#ifndef FIVE_H_
#define FIVE_H_
/*
 * five.c
 *
 * Created: 2021-05-22 00:04:56
 *  Author: Rasmus Lyxell
 */ 

#include <stdint.h>
#include <stdbool.h>

void five_init();
uint8_t five_relative_cursor();
bool five_cursor_up();
bool five_cursor_down();
bool five_cursor_left();
bool five_cursor_right();
char* five_get_active_field();
char five_get_active_char();
bool five_check_win(uint8_t row, uint8_t col, char side);
bool five_receive(char* args);
bool five_cursor_valid();
bool five_send();

#endif /* FIVE_H_ */