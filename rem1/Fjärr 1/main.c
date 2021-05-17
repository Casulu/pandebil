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

void timer2_init();
void clear_heart();
void btn_init();
void led_init();

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
	timer2_init();
	sei();
    /* Replace with your application code */
    while (1) 
    {
		if(!(BTN_PIN & 1<<DEADMANBTN)){
			
		}
		_delay_ms(10);
    }
}

volatile uint8_t hundreths = 0;

ISR(TIMER2_COMPA_vect){
	if(++hundreths > 100){
		LED_PORT |= 1<<LEDR2;
		hundreths = 0;
	}
}

void timer2_init(){
	/* Start 100Hz system timer with TC0 */
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
				default:
					clear_line(0);
					set_cursor_pos(0);
					write_lcd_string("From car");
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
				default:
					clear_line(0);
					set_cursor_pos(0);
					write_lcd_string("Invalid command");
			}
			break;
		default:
			clear_line(0);
			set_cursor_pos(0);
			write_lcd_string("Not for me");
	}
}

void display_message(char* message, bool urgent){
	
}

ISR(USART_RX_vect){
	uart_linebuf[uart_bufind] = UDR0;
	if(uart_linebuf[uart_bufind] == '\n'){
		uart_linebuf[uart_bufind] = '\0';
		//Do something
		perform_command(uart_linebuf[0], uart_linebuf[2], uart_linebuf+4);
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
		uart_send_line("3 1 Hej V");
	}
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1));
	LED_PORT |= 1<<LEDR2;
}

