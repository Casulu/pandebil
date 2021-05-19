/*
 * messages.c
 *
 * Created: 2021-05-19 14:41:21
 *  Author: Rasmus Lyxell
 */
#include <stdbool.h>
#include <string.h>
#include "messages.h"
#include "lcd.h"

#define MSGBUF_BITMASK MSGBUF_SIZE-1
#define MOD_BUFSIZE(x) ((x) & (MSGBUF_BITMASK))

static volatile uint16_t message_parts = 0;
static volatile uint8_t message_seconds = 0;
static volatile char message_buf[MSGBUF_SIZE][MAX_MSG_LEN+1];
static volatile uint8_t message_insert_ind = 0;
static volatile uint8_t message_extract_ind = 0;
static volatile uint8_t message_count = 0;
static volatile bool message_displaying = false;

void messages_timerproc(){
	if(message_displaying && ++message_parts > 100){
		if(++message_seconds > DISP_TIME){
			message_seconds = 0;
			message_displaying = false;
			clear_line(2);
		}
		message_parts = 0;
	}
}

void force_message(char* msg){
	message_displaying = true;
	message_parts = 0;
	message_seconds = 0;
	clear_line(2);
	set_cursor_pos(32);
	write_lcd_string(msg);
}

void queue_message(char* msg){
	if(message_count == MSGBUF_SIZE){
		force_message((char*)(message_buf)[MOD_BUFSIZE(message_extract_ind++)]);
	} else if(++message_count > MSGBUF_SIZE) message_count = MSGBUF_SIZE;
	strcpy((char*)(message_buf)[MOD_BUFSIZE(message_insert_ind++)], msg);
}

void display_next_message(){
	if(message_count > 0){
		set_cursor_pos(32);
		write_lcd_string((char*)(message_buf)[MOD_BUFSIZE(message_extract_ind++)]);
		message_displaying = true;
		message_count--;
	}
}

bool message_is_displaying(){
	return message_displaying;
}
