/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			io.c
* Description:	File for handling input and output.
* Created:		2021-05-19
************************************************************************/

/*************** Header file ***************/
#include "io.h"

/*************** Function definition ***************/
/************************************************************************
Initializes input and output.
************************************************************************/
void ioInit(void) {
	// Turn on outputs
	LED_DDR |= (1<<SD_LED) | (1<<STOP_LED) | (1<<GREEN_HEART_LED) | (1<<RED_HEART_LED);
	IO_DDR |= (1<<SUMMER);
	// Activate inputs, pull-up resistances
	IO_DDR &= ~((1<<STOP_BTN) | (1<<UNSTOP_BTN) | (1<<PING_BTN) | (1<<MODE_SWITCH));
	IO_PORT |= (1<<STOP_BTN) | (1<<UNSTOP_BTN) | (1<<PING_BTN) | (1<<MODE_SWITCH);
	// Start timer for summer
	TCCR2B |= (1<<CS21) | (1<<CS20); // Set prescaler
	TCCR2A |= (1<<COM2B0) | (1<<WGM21); // Toggle on compare match
}

/************************************************************************
Sets led for SD mounted.

Input:	The led to set (SD_LED / STOP_LED / GREEN_HEART_LED / 
		RED_HEART_LED), true or false if on or off.
************************************************************************/
void setLed(uint8_t led, bool on) {
	if (on) LED_PORT |= (1<<led);
	else LED_PORT &= ~(1<<led);
}

/************************************************************************
Checks if a button has been pressed for long enough time.

Input:	The system time in ms.

Return:	A int corresponding to which button was pressed (0 if none).
************************************************************************/
uint8_t btnIsPressed(uint32_t ms) {
	if (!(IO_PIN & (1 << STOP_BTN))) {
		// Check if just or still pressed
		if (btnPressed == STOP_BTN) {
			if (ms - timePressed >= 300) {
				// Reset time, return
				timePressed = ms;
				return STOP_BTN;
			}
		} else {
			btnPressed = STOP_BTN;
			timePressed = ms;
		}
	} else if (!(IO_PIN & (1 << UNSTOP_BTN))) {
		// Check if just or still pressed
		if (btnPressed == UNSTOP_BTN) {
			if (ms - timePressed >= 3000) {
				// Reset time, return
				timePressed = ms;
				return UNSTOP_BTN;
			}
		} else {
			btnPressed = UNSTOP_BTN;
			timePressed = ms;
		}
	} else if (!(IO_PIN & (1 << PING_BTN))) {
		// Check if just or still pressed
		if (btnPressed == PING_BTN) {
			if (ms - timePressed >= 300) {
				// Reset time, return
				timePressed = ms;
				return PING_BTN;
			}
		} else {
			btnPressed = PING_BTN;
			timePressed = ms;
		}
	} else {
		btnPressed = 0;
	}
	return 0;
}

/************************************************************************
Sets the frequency of the timer outputting to the summer.

Input:	The frequency to set, 0 turns off the summer.
************************************************************************/
void summerSetFrequency(uint8_t freq) {
	OCR2A = freq;
}

/************************************************************************
Checks which mode is selected.

Return:	True if normal mode, false otherwise.
************************************************************************/
bool normalMode(void) {
	return !(IO_PIN & (1 << MODE_SWITCH));
}
