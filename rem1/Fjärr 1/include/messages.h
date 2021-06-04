/*
 * messages.h
 *
 * Created: 2021-05-19 14:52:59
 *  Author: Rasmus Lyxell
 */ 


#pragma once
#include <stdbool.h>

#define MSGBUF_SIZE 8		/* Must be power of two*/
#define MAX_MSG_LEN 16		/*Max length for messages*/
#define TIMERHZ 100			/*Frequency of the timer which runs timerproc()*/
#define DISP_TIME 5			/*Amount of seconds to display messages for*/

extern volatile bool output_buf_read;						/*Says if buffer has already been read*/

void messages_timerproc();									/*Tracks time displayed. Should be called at a constant frequency of TIMERHZ*/
void messages_force(char* msg);								/*Forces a message to the front of the queue.*/
void messages_queue(char* msg);								/*Queues a message. Forces the frontmost message if queue is full*/
void messages_move_queue();									/*Moves queue along if no message is active*/
volatile char* messages_get_buffer();						/*Gets the output buffer*/