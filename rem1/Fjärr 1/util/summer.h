/*  FILE:           summer.h
 *  DESCRIPTION: 
 *      Declaration of functions for a piezosummer driver
 *  AUTHOR:         Filip Henningsson, c18fhn@cs.umu.se
 */

#ifndef __SUMMER_H__
#define __SUMMER_H__

#define MELODY_HONK         0
#define MELODY_HEMGLASS     1
#define MELODY_USSR         2
#define MELODY_HERR_GURKA   3
#define MELODY_TWINKLE      4
#define MELODY_COUNT        5
#define MELODY_MAX_LEN      256

#include <avr/io.h>

// Pin definitions for the summer
#define SUMMER_DDR      DDRB
#define SUMMER_PIN      PIN1

#ifdef SUMMER_USE_SW
#define SUMMER_DDR      DDRC
#define SW_SUMMER_PORT  PORTC
#define SUMMER_PIN      PIN4
#endif

// Initializes the summer
void Summer_Init(void);

// Plays a specified melody on the buzzer
void Summer_PlayMelody(uint8_t melody);

#endif // __SUMMER_H__
