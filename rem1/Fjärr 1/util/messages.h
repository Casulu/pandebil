/*
 * messages.h
 *
 * Created: 2021-05-19 14:52:59
 *  Author: Rasmus Lyxell
 */ 


#pragma once
#include <stdbool.h>

#define MSGBUF_SIZE 8		/* Must be power of two*/
#define MAX_MSG_LEN 16
#define TIMERHZ 100
#define DISP_TIME 5

void messages_timerproc();
void force_message(char* msg);
void queue_message(char* msg);
void display_next_message();
bool message_is_displaying();