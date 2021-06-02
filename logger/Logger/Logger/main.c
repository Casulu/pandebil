/************************************************************************
* Project:		Logger.c
* Author:		Simon Ullerstam <ullr@cs.umu.se>
* File:			main.c
* Description:	Main file for program. Software for logger.
* Created:		2021-05-07
************************************************************************/

/*************** External library's ***************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>

/*************** Other header files ***************/
#include "spi.h"
#include "sd.h"
#include "uart.h"
#include "io.h"
#include "u8g.h"

/*************** Constants ***************/
#define ASCII_CODE_0 48
#define MSG_MAX_LEN 19
#define BOARD_LEN 15
#define PLAYING_FIELD_SIZE 57
#define BUF_SIZE 256
#define BUF_MASK (BUF_SIZE - 1)
#if ( BUF_SIZE & BUF_MASK )
#error BUF_SIZE is not a power of 2
#endif

/*************** Function declaration ***************/
void draw(void);
static void setup(void);
static void mountSdIfInserted();
static void readMsg(char *msg, uint8_t offset);
static uint8_t correctTopic(char topic);
static void saveLatestMsg(uint8_t topic, char *msg);

/*************** Global variables ***************/
static volatile uint64_t ms;
static volatile uint64_t offsetBase;
static volatile bool sdMounted;
static volatile bool redHeartLed;
static volatile bool greenHeartLed;
static const char *fileNames[4] = {TO_CAR_LOG, FROM_CAR_LOG, TO_REM1_LOG, TO_REM2_LOG};
static volatile unsigned char buffer[BUF_SIZE];
static volatile uint8_t bufHead = 0;
static volatile uint8_t bufTail = 0;
static volatile unsigned char message[MSG_MAX_LEN + 1];
static volatile uint8_t messageInd;
static volatile unsigned char last_topic_msg[4][MSG_MAX_LEN];
static volatile unsigned char last_text_msg[MSG_MAX_LEN];
static volatile uint8_t playingField[PLAYING_FIELD_SIZE];
static volatile uint8_t lastPlaced;

/*************** Interrupts ***************/
/*
 * USART Rx interrupt.
 * For communication with ESP32.
 */
ISR(USART_RX_vect) {
	// Read data from RX
	unsigned char data = UDR0;
	
	// Check if there is space in buffer
	uint8_t tmpHead = (bufHead + 1) & BUF_MASK;
	if (tmpHead != bufTail) {
		bufHead = tmpHead;
		// End of message time also needs to be added
		if (data == '\n') {
			// Check if there is space in buffer
			tmpHead = (bufHead + 1) & BUF_MASK;
			if (tmpHead != bufTail) {
				// Add time
				buffer[bufHead] = (unsigned char) ((ms - offsetBase) == 0 ? 1 : (ms - offsetBase));
				bufHead = tmpHead;
				data = '\0';
			}
		}
		// Add data
		buffer[bufHead] = data;
	}
}
/*
 * Timer 0 interrupt.
 * For SD-card.
 */
ISR(TIMER0_COMPA_vect) {
	disk_timerproc(); // Drive timer procedure of low level disk I/O module
}

/*
 * Timer 1 interrupt.
 * For system clock.
 */
ISR(TIMER1_COMPA_vect) {
	if (!(ms++ % 1000)) {
		printf("90\n"); // Send heartbeat
		setLed(RED_HEART_LED, redHeartLed);
		summerSetFrequency(200 + 50*redHeartLed);
		redHeartLed = !redHeartLed;
	}
}

/*************** Function definition ***************/
/************************************************************************
Main function.

Return:	The return code of the program.
************************************************************************/
int main(void) {
	setup();
    while (1) { // Main-loop
		// Check if there is data in buffer
		if (bufTail != bufHead) {
			
			// If first message start TC1
			if (ms == 0) TCCR1B |= (1<<CS11);
			
			// Loop until buffer is empty
			do {
				// Get next index
				bufTail = (bufTail + 1) & BUF_MASK;
				if (buffer[bufTail] == '\0') { // Reached end of message
					uint8_t offset = message[--messageInd];
					message[messageInd] = '\0';
					readMsg((char *) message, offset);
					messageInd = 0;
				} else { // Reading message
					message[messageInd++] = buffer[bufTail];
					buffer[bufTail] = '\0';
				}
			} while (bufTail != bufHead);
		}
		cli();
		offsetBase = ms; // Set new base for offset
		sei();
		
		// Check button pressed, preform action
		switch (btnIsPressed(ms)) {
			case STOP_BTN:
				cli();
				printf("95\n"); // Stop message
				sei();
				setLed(STOP_LED, true);
				break;
			case UNSTOP_BTN:
				cli();
				printf("96\n"); // Unstop message
				sei();
				setLed(STOP_LED, false);
				break;
			case PING_BTN:
				cli();
				printf("07\n"); // Ping message
				sei();
				break;			
		}
		// Wakes up by sending ping (to force start the logger)
		if (ms == 0 && !(IO_PIN & (1 << PING_BTN))) printf("07\n"); // Ping message
		
		mountSdIfInserted();
		
		// Draw display
		u8g_FirstPage(&u8g);
		do {
			draw();
		} while (u8g_NextPage(&u8g));
		u8g_Delay(100);
    }
	
	return 0;
}

/************************************************************************
Handles the drawing to the OLED-display.
************************************************************************/
void draw(void) {
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_SetColorIndex(&u8g, 1);
	
	if (normalMode()) { // Normal
		// Print headers
		u8g_DrawStr(&u8g, 0, 10, "to_car  :");
		u8g_DrawStr(&u8g, 0, 22, "from_car:");
		u8g_DrawStr(&u8g, 0, 34, "to_rem1 :");
		u8g_DrawStr(&u8g, 0, 46, "to_rem2 :");
		u8g_DrawStr(&u8g, 0, 60, "text:");
		
		// Print messages
		u8g_DrawStr(&u8g, 54, 10, (char *) last_topic_msg[0]);
		u8g_DrawStr(&u8g, 54, 22, (char *) last_topic_msg[1]);
		u8g_DrawStr(&u8g, 54, 34, (char *) last_topic_msg[2]);
		u8g_DrawStr(&u8g, 54, 46, (char *) last_topic_msg[3]);
		u8g_DrawStr(&u8g, 30, 60, (char *) last_text_msg);
		
	} else { // Game
		// Get center coordinates
		uint8_t centerX = (lastPlaced % 15);
		if (centerX < 3) centerX = 3;
		else if (centerX > 11) centerX = 11;
		uint8_t centerY = (lastPlaced / 15);
		if (centerY < 3) centerY = 3;
		else if (centerY > 11) centerY = 11;
		
		// Print o and x (7*7 around last placed)
		for (int8_t y = -3; y < 4; y++) {
			for (int8_t x = -3; x < 4; x++) {
				uint8_t boxNmr = (y+centerY) * BOARD_LEN + (x+centerX);
				uint8_t cell = (playingField[boxNmr / 4] & (3 << ((boxNmr % 4) * 2))) >> ((boxNmr % 4) * 2);
				// Print x or o
				if (cell == 1) u8g_DrawStr(&u8g, 31 + 10 * (x+3), 11 + 8 * (y+3), "x");
				else if (cell == 2) u8g_DrawStr(&u8g, 31 + 10 * (x+3), 11 + 8 * (y+3), "o");
			}
		}
		
		// Horizontal lines
		u8g_DrawLine(&u8g, 29, 12, 97, 12);
		u8g_DrawLine(&u8g, 29, 20, 97, 20);
		u8g_DrawLine(&u8g, 29, 28, 97, 28);
		u8g_DrawLine(&u8g, 29, 36, 97, 36);
		u8g_DrawLine(&u8g, 29, 44, 97, 44);
		u8g_DrawLine(&u8g, 29, 52, 97, 52);
		
		// Vertical lines
		u8g_DrawLine(&u8g, 38, 5, 38, 59);
		u8g_DrawLine(&u8g, 48, 5, 48, 59);
		u8g_DrawLine(&u8g, 58, 5, 58, 59);
		u8g_DrawLine(&u8g, 68, 5, 68, 59);
		u8g_DrawLine(&u8g, 78, 5, 78, 59);
		u8g_DrawLine(&u8g, 88, 5, 88, 59);
		
		// Print players
		u8g_DrawStr(&u8g, 0, 10, "r1=x");
		u8g_DrawStr(&u8g, 0, 20, "r2=o");
		
		// Print coordinates for center in hex
		char str[5];
		str[0] = 'X';
		str[1] = centerX + ASCII_CODE_0 + 7 * (centerX + ASCII_CODE_0 > '9');
		str[2] = 'Y';
		str[3] = centerY + ASCII_CODE_0 + 7 * (centerY + ASCII_CODE_0 > '9');
		str[4] = '\0';
		u8g_DrawStr(&u8g, 0, 50, "Mid:");
		u8g_DrawStr(&u8g, 0, 60, str);
	}
}

/*************** Static function definition ***************/
/************************************************************************
Setup for program start. Initializes stuff and sets start values.
************************************************************************/
static void setup(void) {
	// Set start values
	ms = 0;
	offsetBase = 0;
	timePressed = 0;
	btnPressed = 0;
	sdMounted = false;
	redHeartLed = false;
	greenHeartLed = false;
	lastPlaced = 0;
	messageInd = 0;
	
	// Reset buffer, message
	memset((char *) buffer, '\0', BUF_SIZE);
	memset((char *) message, '\0', MSG_MAX_LEN);
	
	// Initialize stuff
	spiInit();
	sdInit();
	uartInit();
	ioInit();
	
	// Initialize 1000Hz system timer with TC1
	OCR1A = 1000;
	TCCR1B |= (1<<WGM12);
	TIMSK1 |= (1<<OCIE1A);
	
	sei(); // Set interrupt flag
}

/************************************************************************
// Mounts SD-card if inserted, show SD_LED if mounted.
************************************************************************/
static void mountSdIfInserted() {
	if (!sdMounted && sdCardInserted()) {
		sdCardMount();
		setLed(SD_LED, true);
		sdMounted = true;
	} else if (sdMounted && !sdCardInserted()) {
		setLed(SD_LED, false);
		sdMounted = false;
	}
}

/************************************************************************
Reads a message gotten from the buffer.

Input:	The message, the offset time corresponding to it.
************************************************************************/
static void readMsg(char *msg, uint8_t offset) {
	// Get topic ID
	uint8_t topic;
	if ((topic = correctTopic(msg[0])) != 10) { // Valid ID
		// Check if heartbeat
		if (topic == 0 && msg[1] - ASCII_CODE_0 == 0) {
			setLed(GREEN_HEART_LED, greenHeartLed);
			greenHeartLed = !greenHeartLed;
		}
		
		// Check if game message
		if ((topic == 2 || topic == 3) && msg[1] - ASCII_CODE_0 == 3) {
			if (msg[2] < ASCII_CODE_0) { // Start new game request
				// Reset playingField
				for (uint8_t i = 0; i < PLAYING_FIELD_SIZE; i++) playingField[i] = 0;
			} else if (msg[2] < 65) { // Move done (Ascii 'A')
				lastPlaced = (msg[2] - ASCII_CODE_0 + (msg[3] - ASCII_CODE_0) * BOARD_LEN);
				// Save val in correct place in playingField
				playingField[lastPlaced/4] |= ((1 + (topic==2)) << ((lastPlaced % 4) * 2));
			}
		}
		
		// Log data to SD-card
		if (sdMounted)
			sdCardWrite(fileNames[topic], (char *) (msg + 1), offsetBase + offset);
		
		saveLatestMsg((uint8_t) topic, (char *) msg);
		
	} else if (sdMounted) { // Non valid ID (incorrect topic)
		// Log data to SD-card
		sdCardWrite(NO_TOPIC_LOG, (char *) msg, offsetBase + offset);
	}
}

/************************************************************************
Checks if correct topic.

Input:	Topic to check in ascii form.

Return:	Topic in number form if correct otherwise 10.
************************************************************************/
static uint8_t correctTopic(char topic) {
	return (ASCII_CODE_0 <= topic && topic <= ASCII_CODE_0 + 3) ? (topic - ASCII_CODE_0) : 10;
}

/************************************************************************
Saves latest message for displaying on OLED.

Input:	Message to save.
************************************************************************/
static void saveLatestMsg(uint8_t topic, char *msg) {
	// Save latest message
	strcpy((char *) last_topic_msg[topic], msg);
	
	// If text message save it to latest
	if (topic == 0 && msg[1] == '1') strcpy((char *) last_text_msg, msg + 2);
}
