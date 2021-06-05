/*
 * messages.c
 *
 * Created: 2021-05-19 14:41:21
 *  Author: Rasmus Lyxell
 */
#include <stdbool.h>
#include <string.h>
#include "messages.h"

/*Bitmask and macro for performing mod on buffer size*/
#define MSGBUF_BITMASK MSGBUF_SIZE-1
#define MOD_BUFSIZE(x) ((x) & (MSGBUF_BITMASK))

/*For counting time in timerproc*/
static volatile uint16_t message_parts = 0;
static volatile uint8_t message_seconds = 0;

/*Ooutput buffer*/
volatile char output_buf[MAX_MSG_LEN+1];
volatile bool output_buf_read = true;

/*Message buffer with extraction and insertion indeces and counter for amount of messages*/
static volatile char message_buf[MSGBUF_SIZE][MAX_MSG_LEN+1];
static volatile uint8_t message_insert_ind = 0;
static volatile uint8_t message_extract_ind = 0;
static volatile uint8_t message_count = 0;
/*bool for tracking if any message is active*/
static volatile bool message_displaying = false;

/*Keeps time running and clears buffer on deactivation of a message*/
void messages_timerproc(){
	if(message_displaying && ++message_parts > 100){
		if(++message_seconds > DISP_TIME){
			message_seconds = 0;
			message_displaying = false;
			strcpy((char*)output_buf, "");
			output_buf_read = false;
		}
		message_parts = 0;
	}
}

/*Forces a message to be copied into buffer and marks buffer as changed*/
void messages_force(char* msg){
	message_displaying = true;
	/*Reset time counter to display for full time*/
	message_parts = 0;
	message_seconds = 0;
	strcpy((char*)output_buf, msg);
	output_buf_read = false;
}

/*Queues a message. Forces front most message out of queue if queue is full*/
void messages_queue(char* msg){
	if(message_count == MSGBUF_SIZE){
		messages_force((char*)(message_buf)[MOD_BUFSIZE(message_extract_ind++)]);
	} else if(++message_count > MSGBUF_SIZE) message_count = MSGBUF_SIZE;
	strcpy((char*)(message_buf)[MOD_BUFSIZE(message_insert_ind++)], msg);
}

/*Moves queue along if no message is active*/
void messages_move_queue(){
	if(message_count > 0 && !message_displaying){
		/*Copy next message to output buffer and increment/decremnt index and counter*/
		strcpy((char*)output_buf, (char*)(message_buf)[MOD_BUFSIZE(message_extract_ind++)]);
		output_buf_read = false;
		message_displaying = true;
		message_count--;
	}
}

/*Mark buffer as read and return pointer to buffer*/
volatile char* messages_get_buffer(){
	output_buf_read = true;
	return output_buf;
}