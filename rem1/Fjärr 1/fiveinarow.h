/*
 * fiveinarow.h
 *
 * Created: 2021-05-20
 *  Author: Victor Löfstedt
 */ 

#ifndef FIVEINAROW_H_
#define FIVEINAROW_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util/lcd.h"

extern volatile bool row_player_turn;

void fiveinarow_init();
void fiveinarow_setup(bool turn);
void fiveinarow_left();
void fiveinarow_right();
void fiveinarow_up();
void fiveinarow_down();
void fiveinarow_place(char* message, char topic);
void fiveinarow_recive(char* message);
void fiveinarow_render(char* row_buf);
bool fiveinarow_check_win(); 

#endif