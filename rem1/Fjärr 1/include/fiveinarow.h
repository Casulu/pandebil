/*
 * fiveinarow.h
 *
 * Created: 2021-05-20
 *  Author: Victor L�fstedt
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
#include "lcd.h"

extern volatile bool row_player_turn; /*Exposed bool for showing which players turn it is*/

/*
* fiveinarow_init() - Sets the necessary custom characters for the game.
*
* Returns: Nothing
*/
void fiveinarow_init();
/*
* fiveinarow_setup() - Sets everyting up for a new game.
* (Can also be used as a reset)
* @turn: A bool representing if you are the starting player. 
*
* Returns: Nothing
*/
void fiveinarow_setup(bool turn);
/*
* fiveinarow_left() - Moves the cursor left.
*
* Returns: Nothing
*/
void fiveinarow_left();
/*
* fiveinarow_right() - Moves the cursor right.
*
* Returns: Nothing
*/
void fiveinarow_right();
/*
* fiveinarow_up() - Moves the cursor up.
*
* Returns: Nothing
*/
void fiveinarow_up();
/*
* fiveinarow_down() - Moves the cursor down.
*
* Returns: Nothing
*/
void fiveinarow_down();
/*
* fiveinarow_place() - Places a piece on the board and generates a message to send to the other player.
* @message: A 5 long char array to save the message to. If the message is an empty string(message[0] == '\0') then a piece could not be placed.
* @topic: The topic char to start the message with.
*
* Returns: Nothing
*/
void fiveinarow_place(char* message, char topic);
/*
* fiveinarow_recive() - Places the other players piece on the board.
* @message: The message received from the other player.
*
* Returns: Nothing
*/
void fiveinarow_recive(char* message);
/*
* fiveinarow_render() - Generates a string representing a part on the game board that can then be writen to a 16*3 character display.
* @row_buf: A 49 loch char array to put the resulting string into.
*
* Returns: Nothing
*/
void fiveinarow_render(char* row_buf);
/*
* fiveinarow_check_win() - Checks if the last placed piece by ether player won the game.
*
* Returns: Whether or not the last placed piece won the game.
*/
bool fiveinarow_check_win(); 

#endif