#ifndef PIEZOSUMMER_H_
#define PIEZOSUMMER_H_

/*
* Code for controlling a piezosummer.
*
* Author: Victor Löfstedt (c19vlt@cs.umu.se)
*/

#include <avr/io.h>
#include <util/delay.h>

/*
* piezosummer_setup() - Sets relevant registers in preperations for using the piezosummer.
*
* Returns: Nothing
*/
void piezosummer_setup();
/*
* piezosummer_enable() - Enables the piezosummer by turning it's asigned pin to output.
*
* Returns: Nothing
*/
void piezosummer_enable();
/*
* piezosummer_disable() - Enables the piezosummer by turning it's asigned pin to input.
*
* Returns: Nothing
*/
void piezosummer_disable();
/*
* piezosummer_set_frequency() - Sets the frequency of the piezosummer.
* @frequency: A value representing the frequency emitted by the piezosummer.
* OBS! This is not a direct representation.
*
* Returns: Nothing
*/
void piezosummer_set_frequency(uint8_t frequency);
/*
* piezosummer_play_laser() - Plays a "laser" sound using the piezosummer.
* OBS! The summer must be enabled before calling this function.
*
* Returns: Nothing
*/
void piezosummer_play_laser();
/*
* piezosummer_play_swoop() - Plays a "swoop" sound using the piezosummer.
* OBS! The summer must be enabled before calling this function.
*
* Returns: Nothing
*/
void piezosummer_play_swoop();

#endif