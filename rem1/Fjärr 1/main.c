/*
 * Fjärr 1.c
 *
 * Created: 2021-05-10 14:54:00
 * Author : Rasmus Lyxell
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"
#include "util/lcd.h"
#include "util/spi.h"
#include "util/adc.h"
#include "util/summer.h"


#define BTN_DDR DDRD
#define BTN_PORT PORTD
#define BTN_PIN PIND
#define HONKBTN 3
#define EXTRABTN 2
#define DEADMANBTN 4
#define LED_DDR DDRD
#define LED_PORT PORTD
#define LEDG 5
#define LEDR1 6
#define LEDR2 7

void timer_init();
void clear_heart();
void btn_init();
void led_init();
void display_next_message();
void display_message(char* message, bool urgent);
void display_sensors(char* args);


volatile uint8_t uart_linebuf[40];
volatile uint8_t uart_bufind = 0;

int main(void)
{
	spi_init();
	_delay_ms(100);
	lcd_init();
	adc_init();
	uart_init();
	uart_flush();
	led_init();
	btn_init();
	Summer_Init();
	timer_init();
	sei();
    /* Replace with your application code */
    while (1) 
    {
		if(!(BTN_PIN & 1<<DEADMANBTN)){
			
		}
		_delay_ms(10);
    }
}

volatile uint8_t heart_hundreths = 0;
volatile uint8_t heart_seconds = 0;
volatile uint8_t message_hundreths = 0;
volatile uint8_t message_seconds = 0;
volatile bool message_displaying = false;

ISR(TIMER2_COMPA_vect){
	if(++heart_hundreths > 99){
		LED_PORT |= 1<<LEDR2;
		heart_seconds++;
		heart_hundreths = 0;
	}
	if(message_displaying && ++message_hundreths > 100){
		if(message_seconds++ > 1){
			message_displaying = false;
			display_next_message();
		}
		message_hundreths = 0;
		
	}
}

void timer_init(){
	/* Start 100Hz system timer with TC2 for counting time */
	OCR2A = 77;
	TCCR2A = 1<<WGM21;
	TCCR2B = 0;
	TIMSK2 = 1<<OCIE2A;
}

void clear_heart(){
	LED_PORT &= ~(1<<LEDR2);
}

void perform_command(uint8_t topic, uint8_t command, uint8_t* args){
	switch(topic){
		case '1': //From car
			switch(command){
				case 1:
					display_message("Nödstopp!", true);
					break;
				case 2:
					display_sensors(args);
					break;
				case 3:
					display_message("PONG", false);
			}
			break;
		case '2': //To remote
			switch(command){
				case '0':
					TCCR2B = 7;
					clear_heart();
					break;
				case '1':
					display_message(args, false);
					break;
				case '2':
					Summer_PlayMelody(MELODY_HONK);
					break;
			}
			break;
	}
}

void display_message(char* message, bool urgent){
	
}

void display_next_message(){
	
}

void display_sensors(char* args){
	
}

ISR(USART_RX_vect){
	uart_linebuf[uart_bufind] = UDR0;
	if(uart_linebuf[uart_bufind] == '\n'){
		uart_linebuf[uart_bufind] = '\0';
		//Do something
		perform_command(uart_linebuf[0], uart_linebuf[1], uart_linebuf+3);
		//Done something
		uart_bufind = 0;
	} else{
		uart_bufind++;
	}
}

void btn_init(){
	BTN_DDR &= ~((1<<HONKBTN)|(1<<DEADMANBTN)|(1<<EXTRABTN));
	BTN_PORT |= (1<<HONKBTN)|(1<<DEADMANBTN)|(1<<EXTRABTN);
	EICRA = (2<<ISC00)|(2<<ISC10);
	EIMSK = 3;
}

//Extra
ISR(INT0_vect){
}

//Honk
ISR(INT1_vect){
	if(!(BTN_PIN & 1<<DEADMANBTN)){
		
	} else{
		uart_send_line("31 Hej V");
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1));
	LED_PORT |= 1<<LEDR2;
}

