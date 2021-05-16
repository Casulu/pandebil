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

void btn_init();
void led_init();

volatile uint8_t uart_linebuf[40];
volatile uint8_t uart_bufind = 0;

static int uart_put_char(char c, FILE* stream){
	while ( !( UCSR0A & (1<<UDRE0)) );
	UDR0 = c;
	return 0;
}

static FILE mystdout = FDEV_SETUP_STREAM(uart_put_char, NULL, _FDEV_SETUP_WRITE);

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
	sei();
	stdout = &mystdout;
    /* Replace with your application code */
    while (1) 
    {
		if(!(BTN_PIN & 1<<DEADMANBTN)){
			
		}
		_delay_ms(10);
    }
}

ISR(USART_RX_vect){
	uart_linebuf[uart_bufind] = UDR0;
	if(uart_linebuf[uart_bufind] == '\n'){
		uart_linebuf[uart_bufind] = '\0';
		//Do something
		clear_line(0);
		set_cursor_pos(0);
		if(uart_linebuf[0] == '2'){
			if(uart_linebuf[2] == '0'){
				write_lcd_string("Heartbeat");
			} else if(uart_linebuf[2] == '1'){
				write_lcd_string("Meddelande");
				clear_line(2);
				set_cursor_pos(32);
				write_lcd_string(uart_linebuf + 4);
			} else if(uart_linebuf[2] == '2'){
				write_lcd_string("Honk");
				Summer_PlayMelody(MELODY_HONK);	
			} else{
				write_lcd_string("Wtf");
			}
		} else if(uart_linebuf[0] == '1'){
			
		}
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
	uart_send_line("3 2");
}

void led_init(){
	LED_DDR |= (1<<LEDG)|(1<<LEDR1)|(1<<LEDR2);
	LED_PORT &= ~((1<<LEDG)|(1<<LEDR1)|(1<<LEDR2));
}

